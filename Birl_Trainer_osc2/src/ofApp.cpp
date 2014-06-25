#include "ofApp.h"


//-------
void ofApp::setup(){
    geneva.loadFont("Geneva", 32);

    // setup birl
    birl.setup(BIRL_PORT);
    
    // setup output parameters
    OutputParameter *p1 = new OutputParameter(0, "freq", 40, 1000);
    OutputParameter *p2 = new OutputParameter(1, "amp", 0, 1);
    OutputParameter *p3 = new OutputParameter(2, "timbre1", 0, 1);
//    OutputParameter *p4 = new OutputParameter(3, "timbre2", 0, 1);
//    OutputParameter *p5 = new OutputParameter(4, "timbre3", 0, 1);
//    OutputParameter *p6 = new OutputParameter(5, "timbre4", 0, 1);

    outputParameters.push_back(p1);
    outputParameters.push_back(p2);
    outputParameters.push_back(p3);
//    outputParameters.push_back(p4);
//    outputParameters.push_back(p5);
//    outputParameters.push_back(p6);
    
    
    // setup gui
    guiSelect = new ofxUISuperCanvas("Select");
    guiSelect->setPosition(TRAIN_GUI_X+TRAIN_GUI_W-92, TRAIN_GUI_Y+TRAIN_GUI_H-92);
    guiSelect->setWidth(90);
    guiSelect->setHeight(90);
    guiSelect->addButton("add new", false);
    guiSelect->addButton("next", false);

    guiMonitor = new ofxUISuperCanvas("Monitor");
    guiMonitor->setPosition(TRAIN_GUI_X+TRAIN_GUI_W-112, TRAIN_GUI_Y+TRAIN_GUI_H-112);
    guiMonitor->setWidth(110);
    guiMonitor->setHeight(110);
    guiMonitor->addButton("back", false);
    guiMonitor->addButton("train fast", false);
    guiMonitor->addButton("train accurate", false);
    
    guiRecord = new ofxUISuperCanvas("Record");
    guiRecord->setGlobalButtonDimension(64);
    guiRecord->setDrawOutline(true);
    guiRecord->setPosition(TRAIN_GUI_X+4, TRAIN_GUI_Y+TRAIN_GUI_H-172);
    guiRecord->setWidth(0.49*TRAIN_GUI_W);
    guiRecord->setHeight(165);
    guiRecord->addToggle("record", false)->setLabelVisible(false);
    guiRecord->addLabel("Countdown");
    guiRecord->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiRecord->addTextInput("countdown", ofToString(countdown), 50.0f)->setAutoClear(false);;
    guiRecord->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiRecord->addLabel("Duration");
    guiRecord->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiRecord->addTextInput("duration", ofToString(duration), 50.0f)->setAutoClear(false);

    guiPlay = new ofxUISuperCanvas("Classifier trained!");
    guiPlay->setGlobalButtonDimension(72);
    guiPlay->setPosition(TRAIN_GUI_X+4, TRAIN_GUI_Y+TRAIN_GUI_H-172);
    guiPlay->setWidth(0.49*TRAIN_GUI_W);
    guiPlay->setHeight(165);
    guiPlay->addToggle("predict", &predicting)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPlay->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiPlay->addButton("save", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiPlay->addButton("back", false)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);

    ofAddListener(guiSelect->newGUIEvent, this, &ofApp::guiSelectEvent);
    ofAddListener(guiMonitor->newGUIEvent, this, &ofApp::guiMonitorEvent);
    ofAddListener(guiRecord->newGUIEvent, this, &ofApp::guiRecordEvent);
    ofAddListener(guiPlay->newGUIEvent, this, &ofApp::guiPlayEvent);
    
    // set initial mode
    setTrainState(TRAINING_SELECT_OUTPUTS);
}

void ofApp::guiSelectEvent(ofxUIEventArgs &e) {
    if (e.getName() == "add new") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        int idx = outputParameters.size();
        if (idx >= MAX_OUTPUT_PARAMETERS)  return;
        OutputParameter *p = new OutputParameter(idx, "param"+ofToString(idx), 0, 10);
        p->setTrainState(trainState);
        outputParameters.push_back(p);
    }
    else if (e.getName() == "next") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        for (int i=0; i<outputParameters.size(); i++) {
            if (!outputParameters[i]->hasInput()) {
                ofSystemAlertDialog("You must select at least one input for every output parameter before proceeding. Also say sorry.");
                return;
            }
        }
        setTrainState(TRAINING_RECORD);
    }    
}

void ofApp::guiMonitorEvent(ofxUIEventArgs &e) {
    if (e.getName() == "back") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        setTrainState(TRAINING_SELECT_OUTPUTS);
    }
    else if (e.getName() == "train fast") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        ofSystemAlertDialog("About to train classifiers. This may take a few minutes. Try going outside.");
        for (int i=0; i<outputParameters.size(); i++) {
            outputParameters[i]->trainClassifierFast();
        }
        cout << "finished training"<< endl;
        setTrainState(TRAINING_PLAY);
    }
    else if (e.getName() == "train accurate") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        ofSystemAlertDialog("About to train classifiers. This may take a few minutes. Try going outside.");
        for (int i=0; i<outputParameters.size(); i++) {
            outputParameters[i]->trainClassifierAccurate();
        }
        cout << "finished training"<< endl;
        setTrainState(TRAINING_PLAY);
    }
}

void ofApp::guiRecordEvent(ofxUIEventArgs &e) {
    if (e.getName() == "countdown") {
        countdown = ofToFloat(((ofxUITextInput *) e.widget)->getTextString());
    }
    else if (e.getName() == "duration") {
        duration = ofToFloat(((ofxUITextInput *) e.widget)->getTextString());
    }
    else if (e.getName() == "record") {
        if (((ofxUIToggle *) e.widget)->getValue()==1) {
            timerLast = ofGetElapsedTimef();
            countingDown = true;
        } else {
            timer = 0;
            countingDown = false;
            recording = false;
            guiRecord->setColorOutline(ofColor(255));
            ((ofxUIToggle *) guiRecord->getWidget("record"))->setValue(false);
        }
    }
}

void ofApp::guiPlayEvent(ofxUIEventArgs &e) {
    if (e.getName() == "back") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        setTrainState(TRAINING_RECORD);
        predicting = false;
    }
    else if (e.getName() == "save preset") {
        if (((ofxUIButton *) e.widget)->getValue()==1) return;
        // save preset
        // go to performance mode
    }    
}

//-------
void ofApp::update(){
    checkIfDeleteParameters();

    birl.update();
    
    switch (trainState) {
        
        // MODE OUTPUT SELECTION
        case TRAINING_SELECT_OUTPUTS:
            // nothing to update
            break;
            
        
        // MODE RECORDING EXAMPLES
        case TRAINING_RECORD:
            if (countingDown) {
                timer = countdown - (ofGetElapsedTimef() - timerLast);
                if (timer <= 0) {
                    recording = true;
                    countingDown = false;
                    timerLast = ofGetElapsedTimef();
                    guiRecord->setColorOutline(ofColor(255, 0, 0));
                }
            }            
            else if (recording) {
                vector<float> keys = birl.getKeys();
                vector<float> keysDiscrete = birl.getKeysDiscrete();
                vector<float> pressure = birl.getPressure();
                vector<float> embouchure = birl.getEmbouchure();
                
                // add instances
                for (int i=0; i<outputParameters.size(); i++) {
                    if (outputParameters[i]->isTraining()) {
                        
                        vector<double> instance;
                        if (outputParameters[i]->getInputKeys()) {
                            for (int i=0; i<keys.size(); i++) {
                                instance.push_back(keys[i]);
                            }
                        }
                        else if (outputParameters[i]->getInputKeysDiscrete()) {
                            for (int i=0; i<keysDiscrete.size(); i++) {
                                instance.push_back(keysDiscrete[i]);
                            }
                        }
                        if (outputParameters[i]->getInputPressure()) {
                            instance.push_back(pressure[0]);
                            instance.push_back(pressure[1]);
                        }
                        if (outputParameters[i]->getInputEmbouchure()) {
                            //instance.push_back(embouchure);
                        }
                        cout << "add instance " << instance.size() << endl;
                        cout << ofToString(instance) << endl;
                        outputParameters[i]->addInstance(instance);
                    }
                }
                
                // update message
                numCollectedCurrent++;
                numCollectedTotal++;
                trainingMessage = "Collected "+ofToString(numCollectedCurrent)+" samples, total "+ofToString(numCollectedTotal);
                
                timer = duration - (ofGetElapsedTimef() - timerLast);
                if (timer <= 0) {
                    timer = 0;
                    recording = false;
                    countingDown = false;
                    numCollectedCurrent = 0;
                    guiRecord->setColorOutline(ofColor(255));
                    ((ofxUIToggle *) guiRecord->getWidget("record"))->setValue(false);
                }
            }
            break;
            
        // MODE PLAYING BACK TRAINED CLASSIFIER
        case TRAINING_PLAY:            
            if (predicting) {
                vector<float> keys = birl.getKeys();
                vector<float> keysDiscrete = birl.getKeysDiscrete();
                vector<float> pressure = birl.getPressure();
                vector<float> embouchure = birl.getEmbouchure();

                // classify instance
                for (int i=0; i<outputParameters.size(); i++) {
                    vector<double> instance;
                    if (outputParameters[i]->getInputKeys()) {
                        for (int i=0; i<keys.size(); i++) {
                            instance.push_back(keys[i]);
                        }
                    }
                    else if (outputParameters[i]->getInputKeysDiscrete()) {
                        for (int i=0; i<keysDiscrete.size(); i++) {
                            instance.push_back(keysDiscrete[i]);
                        }
                    }
                    if (outputParameters[i]->getInputPressure()) {
                        instance.push_back(pressure[0]);
                        instance.push_back(pressure[1]);
                    }
                    if (outputParameters[i]->getInputEmbouchure()) {
                        //instance.push_back(embouchure);
                    }
                    
                    outputParameters[i]->classifyInstance(instance);
                }
            }
            break;
            
        default:
            break;
    }
}

//-------
void ofApp::setTrainState(TrainState trainState){
    this->trainState = trainState;
    for (int i=0; i<outputParameters.size(); i++) {
        outputParameters[i]->setTrainState(trainState);
    }
    switch (trainState) {
        case TRAINING_SELECT_OUTPUTS:
            guiSelect->setVisible(true);
            guiMonitor->setVisible(false);
            guiRecord->setVisible(false);
            guiPlay->setVisible(false);
            break;
            
        case TRAINING_RECORD:
            guiSelect->setVisible(false);
            guiMonitor->setVisible(true);
            guiRecord->setVisible(true);
            guiPlay->setVisible(false);
            break;
            
        case TRAINING_PLAY:
            guiSelect->setVisible(false);
            guiMonitor->setVisible(false);
            guiRecord->setVisible(false);
            guiPlay->setVisible(true);
            predicting = true;
            break;
            
        default:
            break;
    }   
}

//-------
void ofApp::draw(){
    // draw birl
    birl.draw(BIRL_DRAW_X, BIRL_DRAW_Y, BIRL_DRAW_W, BIRL_DRAW_H);
        
    // draw additional elements for training gui
    ofPushMatrix();
    ofTranslate(TRAIN_GUI_X, TRAIN_GUI_Y);
    ofSetColor(50);
    ofRect(0, 0, TRAIN_GUI_W, TRAIN_GUI_H);

    switch (trainState) {
        case TRAINING_SELECT_OUTPUTS:
            break;

        case TRAINING_RECORD:
            ofSetColor(recording ? ofColor(255,0,0) : ofColor(255));
            if (countingDown || recording) {
                geneva.drawString(ofToString(ceil(timer)),
                    105, TRAIN_GUI_H-100);
            }
            ofDrawBitmapString(trainingMessage,
                10, TRAIN_GUI_H-18);
            break;

        case TRAINING_PLAY:
            break;

        default:
            break;
    }
    ofPopMatrix();
}


//-------
void ofApp::keyPressed(int key){
}

//-------
void ofApp::checkIfDeleteParameters() {
    // super-hacky way to delete parameters on request from
    // user and re-index remaining parameters.
    // trained programmers, please look away. it works.
    bool hasDeleted = false;
    vector<OutputParameter*>::iterator it;
    for (it = outputParameters.begin(); it != outputParameters.end(); ) {
        if((*it)->getToDelete()) {
            (*it)->destroy();
            delete *it;
            it = outputParameters.erase(it);
            hasDeleted = true;
        }
        else {
            ++it;
        }
    }
    if (hasDeleted) {
        for (int i=0; i<outputParameters.size(); i++) {
            outputParameters[i]->reindex(i);
        }
    }
}
