#include "SynthParameter.h"

//-------
SynthParameter::SynthParameter(string name, float minVal, float maxVal) {
    this->name = name;
    this->minVal = minVal;
    this->maxVal = maxVal;
    value = 0.5 * (minVal + maxVal);
    
    gui = new ofxUISuperCanvas(name);
    gui->setColorBack(ofColor(60));
    gui->setWidth(540);
    gui->setHeight(SYNTH_PARAMETER_HEIGHT);
    gui->setDrawPadding(true);
    gui->addSpacer();
    sliderValue = gui->addSlider("value", minVal, maxVal, &value);
    
    gui->addLabel("min");
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    textMinVal = gui->addTextInput("textMin", ofToString(minVal), 80.0f);
    
    gui->addLabel("max");
    textMaxVal = gui->addTextInput("textMax", ofToString(maxVal), 80.0f);
    
    gui->addLabel("value");
    textValue  = gui->addTextInput("textValue", ofToString(value), 80.0f);
    
    textMinVal->setAutoClear(false);
    textMaxVal->setAutoClear(false);
    textValue->setAutoClear(false);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addLabel("OSC");
    oscAddress = gui->addTextInput("osc address", ofToString("/birl/"+name));
    oscAddress->setAutoClear(false);
    
    gui->addToggle("training", &training);
    
    ofAddListener(gui->newGUIEvent, this, &SynthParameter::guiEvent);
}

//-------
void SynthParameter::clear() {
    delete gui;
}

//-------
void SynthParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "textMin") {
        minVal = ofToFloat(textMinVal->getTextString());
        sliderValue->setMin(minVal);
        sliderValue->setValue(ofToFloat(textValue->getTextString()));
    }
    else if (e.getName() == "textMax") {
        maxVal = ofToFloat(textMaxVal->getTextString());
        sliderValue->setMax(maxVal);
        sliderValue->setValue(ofToFloat(textValue->getTextString()));
    }
    else if (e.getName() == "textValue") {
        value = ofToFloat(textValue->getTextString());
    }
    else if (e.getName() == "value") {
        textValue->setTextString(ofToString(value));
    }
    else if (e.getName() == "training") {
        if (training) {
            gui->setColorBack(ofColor(150, 5, 15));
        }
        else {
            gui->setColorBack(ofColor(60));
        }
    }
}

//-------
void SynthParameter::addInstance(vector<double> instance) {
    classifier.addTrainingInstance(instance, value);
}

//-------
void SynthParameter::trainClassifier() {
    classifier.trainRegression();
}

//-------
void SynthParameter::classifyInstance(vector<double> instance) {
    value = classifier.predict(instance);
}



