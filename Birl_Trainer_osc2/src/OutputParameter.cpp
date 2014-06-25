#include "OutputParameter.h"

//-------
OutputParameter::OutputParameter(int idx, string name, float minValue, float maxValue){
    this->idx = idx;
    this->name = name;
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->value = 0.5 * (minValue + maxValue);
    oscAddress = ofToString("/birl/"+name);
    
    
    guiTrain1 = new ofxUISuperCanvas("Parameter");
    guiTrain1->setPosition(
        TRAIN_GUI_X + (idx<5 ? 4 : 4+0.5*TRAIN_GUI_W),
        TRAIN_GUI_Y + 12 + 146 * (idx%5) );
    guiTrain1->setWidth(0.49*TRAIN_GUI_W);
    guiTrain1->setHeight(130);
    guiTrain1->setColorOutline(ofColor(0,220,50));
    guiTrain1->setColorOutlineHighlight(ofColor(255, 0, 0));
    guiTrain1->setDrawOutline(true);
    
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain1->addButton("delete", false);
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiTrain1->addSpacer();
    guiTrain1->addLabel("Name");
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain1->addTextInput("name", name, 280.0f)->setAutoClear(false);
    
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiTrain1->addLabel("Minimum");
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain1->addTextInput("minimum", ofToString(minValue), 84.0f)->setAutoClear(false);;
    guiTrain1->addLabel("Maximum");
    guiTrain1->addTextInput("maximum", ofToString(maxValue), 84.0f)->setAutoClear(false);;
    
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiTrain1->addLabel("OSC");
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain1->addTextInput("osc", oscAddress, 290.0f)->setAutoClear(false);;
    
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiTrain1->addLabel("Inputs   ");
    guiTrain1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain1->addToggle("keys", &inputKeys);
    guiTrain1->addToggle("keys (discrete)", &inputKeysDiscrete);
    guiTrain1->addToggle("pressure", &inputPressure);
    guiTrain1->addToggle("embouchure", &inputEmbouchure);
    
    
    guiTrain2 = new ofxUISuperCanvas(name);    
    guiTrain2->setPosition(
       TRAIN_GUI_X + (idx<5 ? 4 : 4+0.5*TRAIN_GUI_W),
       TRAIN_GUI_Y + 4 + 112 * (idx%5) );

    guiTrain2->setColorOutline(ofColor(0,220,50));
    guiTrain2->setColorOutlineHighlight(ofColor(255, 0, 0));
    guiTrain2->setDrawOutline(true);
    guiTrain2->setWidth(0.48*TRAIN_GUI_W);
    guiTrain2->setHeight(107);
    
    guiTrain2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain2->addToggle("train", false);
    guiTrain2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    guiTrain2->addSpacer();
    guiTrain2->addSlider("value", minValue, maxValue, &value, 240.0f, 22.0f)->setLabelVisible(false);
    guiTrain2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain2->addTextInput("valueTxt", ofToString(value), 50.0f, 22.0f)->setAutoClear(false);
    guiTrain2->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiTrain2->addTextArea("oscAddress", ofToString("osc: "+oscAddress))->setColorFill(ofColor(255, 160));
    guiTrain2->addTextInput("numinstances", "0 instances", 125.0f);
    guiTrain2->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiTrain2->addButton("view", false);
    
    
    ofAddListener(guiTrain1->newGUIEvent, this, &OutputParameter::guiEvent);
    ofAddListener(guiTrain2->newGUIEvent, this, &OutputParameter::guiEvent);
}

//-------
void OutputParameter::reindex(int idx) {
    this->idx = idx;
    guiTrain1->setPosition(
        TRAIN_GUI_X + (idx<5 ? 4 : 4+0.5*TRAIN_GUI_W),
        TRAIN_GUI_Y + 12 + 146 * (idx%5) );
    guiTrain2->setPosition(
        TRAIN_GUI_X + (idx<5 ? 4 : 4+0.5*TRAIN_GUI_W),
        TRAIN_GUI_Y + 4 + 112 * (idx%5) );
}

//-------
void OutputParameter::destroy() {
    delete guiTrain1;
    delete guiTrain2;
}

//-------
void OutputParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "delete") {
        if (((ofxUIButton *) guiTrain1->getWidget("delete"))->getValue()) {
            return;
        }
        string action = ofSystemTextBoxDialog("WARNING: This will erase this output parameter permanently. Type OK to execute.");
        if (action == "OK") {
            toDelete = true;
        }
    }
    else if (e.getName() == "keys") {
        if (e.getToggle()->getValue()) {
            inputKeysDiscrete = false;
        }
    }
    else if (e.getName() == "keys (discrete)") {
        if (e.getToggle()->getValue()) {
            inputKeys = false;
        }
    }
    else if (e.getName() == "name") {
        name = ((ofxUITextInput *) guiTrain1->getWidget("name"))->getTextString();
        oscAddress = ofToString("/birl/"+name);
        ((ofxUITextInput *) guiTrain1->getWidget("osc"))->setTextString(oscAddress);
    }
    else if (e.getName() == "minimum") {
        if (((ofxUITextInput *) guiTrain1->getWidget("minimum"))->getInputTriggerType() == 1) {
            return;
        }
        if (minValue == ofToFloat( ((ofxUITextInput *) guiTrain1->getWidget("minimum"))->getTextString())) {
            return;
        }
        if (isTrained) {
            string action = ofSystemTextBoxDialog("WARNING: this will erase the classifier for this parameter. Type OK if you're cool with this.");
            if (action != "OK") {
                ((ofxUITextInput *) guiTrain1->getWidget("minimum"))->setTextString(ofToString(minValue));
                return;
            } else {
                isTrained = false;
            }
        }
        minValue = ofToFloat(((ofxUITextInput *) guiTrain1->getWidget("minimum"))->getTextString());
        value = 0.5 * (minValue + maxValue);
        ((ofxUISlider *) guiTrain2->getWidget("value"))->setMin(minValue);
        ((ofxUISlider *) guiTrain2->getWidget("value"))->setValue(value);
        ((ofxUITextInput *) guiTrain2->getWidget("valueTxt"))->setTextString(ofToString(value));
    }
    else if (e.getName() == "maximum") {
        if (((ofxUITextInput *) guiTrain1->getWidget("maximum"))->getInputTriggerType() == 1) {
            return;
        }
        if (maxValue == ofToFloat( ((ofxUITextInput *) guiTrain1->getWidget("maximum"))->getTextString())) {
            return;
        }
        if (isTrained) {
            string action = ofSystemTextBoxDialog("WARNING: this will erase the classifier for this parameter. Type OK if you're cool with this.");
            if (action != "OK") {
                ((ofxUITextInput *) guiTrain1->getWidget("maximum"))->setTextString(ofToString(maxValue));
                return;
            } else {
                isTrained = false;
            }
        }
        maxValue = ofToFloat(((ofxUITextInput *) guiTrain1->getWidget("maximum"))->getTextString());
        value = 0.5 * (minValue + maxValue);
        ((ofxUISlider *) guiTrain2->getWidget("value"))->setMax(maxValue);
        ((ofxUISlider *) guiTrain2->getWidget("value"))->setValue(value);
        ((ofxUITextInput *) guiTrain2->getWidget("valueTxt"))->setTextString(ofToString(value));
    }
    
    else if (e.getName() == "value") {
        ((ofxUITextInput *) guiTrain2->getWidget("valueTxt"))->setTextString(ofToString(value));
    }
    else if (e.getName() == "valueTxt") {
        value = ofToFloat(((ofxUITextInput *) guiTrain2->getWidget("valueTxt"))->getTextString());
    }
    
    else if (e.getName() == "train") {
        training = !training;
        if (training) {
            guiTrain2->setColorBack(ofColor(90, 10, 20));
        } else {
            guiTrain2->setColorBack(ofColor(30));
        }
    }
}

//-------
void OutputParameter::setTrainState(TrainState trainState) {
    switch (trainState) {
        case TRAINING_SELECT_OUTPUTS:
            guiTrain1->setVisible(true);
            guiTrain2->setVisible(false);
            break;
            
        case TRAINING_RECORD:
            guiTrain1->setVisible(false);
            guiTrain2->setVisible(true);
            break;
            
        case TRAINING_PLAY:
            guiTrain1->setVisible(false);
            guiTrain2->setVisible(true);            
            break;
            
        default:
            break;
    }
}

//-------
void OutputParameter::addInstance(vector<double> instance) {
    classifier.addTrainingInstance(instance, value);
    ((ofxUITextInput *) guiTrain2->getWidget("numinstances"))->setTextString(ofToString(classifier.getNumberTrainingInstances())+" instances");
}

//-------
void OutputParameter::trainClassifierFast() {
    classifier.trainRegression(FAST);
    isTrained = true;
}

//-------
void OutputParameter::trainClassifierAccurate() {
    classifier.trainRegression(ACCURATE);
    isTrained = true;
}

//-------
void OutputParameter::classifyInstance(vector<double> instance) {
    value = classifier.predict(instance);
    ((ofxUITextInput *) guiTrain2->getWidget("valueTxt"))->setTextString(ofToString(value));
}



