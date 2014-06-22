#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	osc.setup(PORT);
    
    
    // BIRL
    
    // initialize a 9 element instance vector
    for (int i=0; i<9; i++) {
        keys.push_back(0);
    }
    
    guiData = new ofxUISuperCanvas("Birl data");
    guiData->setWidth(500);
    guiData->addSlider("1", 0.0, 1.0, &keys[0], 40, 150);
	guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    for (int i=1; i<9; i++) {
        guiData->addSlider(ofToString(i+1), 0.0, 1.0, &keys[i], 40, 150);
    }
//    guiData->addSlider("label", 0, 1, &label, 40, 150);
	guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    guiData->addSpacer();
    
    guiOptions = new ofxUISuperCanvas("Options");
    ofAddListener(guiOptions->newGUIEvent,this,&ofApp::guiEvent);
    guiOptions->setPosition(ofGetWidth()-200, 0);
    guiOptions->setWidth(200);
    guiOptions->addLabel("Manual training");
    guiOptions->addToggle("record", &recording);
    guiOptions->addToggle("test", &testing);
    guiOptions->addSpacer();
    guiOptions->addLabel("Train from file");
    guiOptions->addButton("trainFile", false);
    guiOptions->addSpacer();
    
    
    
    // Instruments
    rezonator.setup("rezonator");
    rezonator.setGuiPosition(0, 300);
    rezonator.addParameter("tone", 0, 12);
    rezonator.addParameter("octave", 1, 8);
    rezonator.addParameter("atk", 0.0, 1.0);
    rezonator.addParameter("rq", 0.0001, 0.1);
    rezonator.addParameter("deltime", 0, 2.5);
    rezonator.create();
    
    klanky.setup("klanky");
    klanky.setGuiPosition(250, 300);
    klanky.addParameter("tone", 0, 12);
    klanky.addParameter("octave", 1, 10);
    klanky.addParameter("atk", 0.0, 1.0);
    klanky.addParameter("cutoff", 0.0, 2000.0);
    klanky.create();
    
    organ.setup("organ");
    organ.setGuiPosition(500, 300);
    organ.addParameter("tone", 0, 12);
    organ.addParameter("octave", 1, 8);
    organ.addParameter("atk", 0.0, 1.0);
    organ.create();
    
    pmosc.setup("pmosc");
    pmosc.setGuiPosition(750, 300);
    pmosc.addParameter("tone", 0, 12);
    pmosc.addParameter("octave", 1, 8);
    pmosc.addParameter("atk", 0.0, 1.0);
    pmosc.addParameter("indexLow", 0.0, 20.0);
    pmosc.addParameter("indexHigh", 0.0, 20.0);
    pmosc.create();
    
    voices.setup("voices");
    voices.setGuiPosition(1000, 300);
    voices.addParameter("tone", 0, 12);
    voices.addParameter("octave", 1, 8);
    voices.addParameter("atk", 0.0, 1.0);
    voices.addParameter("cnoiseMul", 0.0, 1.0);
    voices.addParameter("lpfreq", 0.0, 10000.0);
    voices.create();
}


//--------------------------------------------------------------
void ofApp::update(){
    
    // listening for OSC messages from Birl
    while(osc.hasWaitingMessages()){
		ofxOscMessage m;
		osc.getNextMessage(&m);
        
        // for each of the 9 keys on the birl, clamp values between 0 and 1
        if       (m.getAddress() == "/JT0"){
			keys[0] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT1"){
			keys[1] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT2"){
			keys[2] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
        } else if(m.getAddress() == "/JT3"){
			keys[3] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT4"){
			keys[4] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT5"){
			keys[5] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT6"){
			keys[6] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT7"){
			keys[7] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
		} else if(m.getAddress() == "/JT8"){
			keys[8] = ofClamp(ofMap(m.getArgAsFloat(0), 0, 1000, 0, 1), 0, 1);
        }
    }
    
    
    // if we are in recording mode, we are actively adding
    // samples to the training set. the feature vector to add is
    // the current state of the keys (a 9-element vector of floats
    // between 0 and 1.
    // the label is a continous value between 0 and 1
    
    if (recording) {
        vector<double> instance(keys.begin(), keys.end());
        vector<Parameter *> params = voices.getParameters();

        if (isMLP) {
            learn1.addTrainingInstance(instance, ofMap(params[0]->getValue(), params[0]->getMinValue(), params[0]->getMaxValue(), 0, 1));
            learn2.addTrainingInstance(instance, ofMap(params[1]->getValue(), params[1]->getMinValue(), params[1]->getMaxValue(), 0, 1));
            learn3.addTrainingInstance(instance, ofMap(params[2]->getValue(), params[2]->getMinValue(), params[2]->getMaxValue(), 0, 1));
            learn4.addTrainingInstance(instance, ofMap(params[3]->getValue(), params[3]->getMinValue(), params[3]->getMaxValue(), 0, 1));
            learn5.addTrainingInstance(instance, ofMap(params[4]->getValue(), params[4]->getMinValue(), params[4]->getMaxValue(), 0, 1));
        } else {
            learn1.addTrainingInstance(instance, params[0]->getValue());
            learn2.addTrainingInstance(instance, params[1]->getValue());
            learn3.addTrainingInstance(instance, params[2]->getValue());
            learn4.addTrainingInstance(instance, params[3]->getValue());
            learn5.addTrainingInstance(instance, params[4]->getValue());            
        }
    }
    
    // if we are in testing mode, we are continously classifying
    // the current state of the keys
    else if (testing) {

        vector<double> instance(keys.begin(), keys.end());
        
        
        vector<Parameter *> params = voices.getParameters();
        
        if (isMLP) {
            params[0]->setValue(ofMap(learn1.predict2(instance), 0, 1, params[0]->getMinValue(), params[0]->getMaxValue()));
            params[1]->setValue(ofMap(learn2.predict2(instance), 0, 1, params[1]->getMinValue(), params[1]->getMaxValue()));
            params[2]->setValue(ofMap(learn3.predict2(instance), 0, 1, params[2]->getMinValue(), params[2]->getMaxValue()));
            params[3]->setValue(ofMap(learn4.predict2(instance), 0, 1, params[3]->getMinValue(), params[3]->getMaxValue()));
            params[4]->setValue(ofMap(learn5.predict2(instance), 0, 1, params[4]->getMinValue(), params[4]->getMaxValue()));
        } else {
            params[0]->setValue(learn1.predict(instance));
            params[1]->setValue(learn2.predict(instance));
            params[2]->setValue(learn3.predict(instance));
            params[3]->setValue(learn4.predict(instance));
            params[4]->setValue(learn5.predict(instance));
        }
    }
    
    
    // update instruments
    rezonator.update();
    klanky.update();
    organ.update();
    pmosc.update();
    voices.update();    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(235);
    
    // if recording instances, draw a red dot on top right corner
    if (recording) {
        ofSetColor(255, 0, 0);
        ofEllipse(ofGetWidth()-50, 50, 50, 50);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key=='1') {
        recording = !recording;
    }
    if (key=='2') {
        testing = !testing;
        if (testing) {
            recording = false;
            if (isMLP) {
                learn1.trainRegression2();
                learn2.trainRegression2();
                learn3.trainRegression2();
                learn4.trainRegression2();
                learn5.trainRegression2();
            } else {
                learn1.trainRegression();
                learn2.trainRegression();
                learn3.trainRegression();
                learn4.trainRegression();
                learn5.trainRegression();

            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "test") {
        if (testing) {
            recording = false;
            if (isMLP) {
                learn1.trainRegression2();
                learn2.trainRegression2();
                learn3.trainRegression2();
                learn4.trainRegression2();
                learn5.trainRegression2();
            } else {
                learn1.trainRegression();
                learn2.trainRegression();
                learn3.trainRegression();
                learn4.trainRegression();
                learn5.trainRegression();                
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
ofApp::~ofApp(){
    rezonator.free();
    klanky.free();
    organ.free();
    pmosc.free();
    voices.free();
}
