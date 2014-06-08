#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	osc.setup(PORT);

    // initialize a 9 element instance vector
    for (int i=0; i<9; i++) {
        keys.push_back(0);
    }
    
    // birl data
//    guiData = new ofxUISuperCanvas("Birl data");
//    guiData->setWidth(500);
//    guiData->addSpacer();

    
    guiData = new ofxUISuperCanvas("Birl Data");
    guiData->addSpacer();
    ofxUICircleSlider *s1 = guiData->addCircleSlider("1", 0, 100, 50, 50);
    guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    ofxUICircleSlider *s2 = guiData->addCircleSlider("2", 0, 100, 50, 50);
    ofxUICircleSlider *s3 = guiData->addCircleSlider("3", 0, 100, 50, 50);
    ofxUICircleSlider *s4 = guiData->addCircleSlider("4", 0, 100, 50, 50);
    ofxUICircleSlider *s5 = guiData->addCircleSlider("5", 0, 100, 50, 50);
    ofxUICircleSlider *s6 = guiData->addCircleSlider("6", 0, 100, 50, 50);
    

    guiData->autoSizeToFitWidgets();
    
//    guiData->addRotarySlider("R2SLIDER", 0, 100, 50);
    //    guiData->addSlider("1", 0.0, 1.0, &keys[0], 40, 150);
//	guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
//    for (int i=1; i<9; i++) {
//        guiData->addSlider(ofToString(i+1), 0.0, 1.0, &keys[i], 40, 150);
//    }
//    guiData->addSlider("label", 0, 1, &label, 40, 150);
//	guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
//    guiData->addSpacer();
    
    // options
    guiOptions = new ofxUISuperCanvas("Options");
    ofAddListener(guiOptions->newGUIEvent,this,&ofApp::guiEvent);
    guiOptions->setPosition(ofGetWidth()-200, 0);
    guiOptions->setWidth(200);
    guiOptions->addLabel("Manual training");
    guiOptions->addToggle("record", &recording);
    guiOptions->addToggle("test", &testing);
    guiOptions->addSpacer();
    
    // STK
    guiData = new ofxUISuperCanvas("STK synth");
    guiData->setPosition(0, 300);
    guiData->setWidth(500);
    guiData->addSlider("jetDelay", 0.0, 1.0, &jetDelay, 40, 150);
    guiData->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    guiData->addSlider("jetReflection", 0.0, 1.0, &jetReflection, 40, 150);
    guiData->addSlider("endReflection", 0.0, 1.0, &endReflection, 40, 150);
    guiData->addSlider("noiseGain", 0.0, 1.0, &noiseGain, 40, 150);
    guiData->addSlider("breathPressure", 0.0, 1.0, &breathPressure, 40, 150);
    guiData->addSlider("frequency", 100, 1000, &frequency, 40, 150);
    guiData->addSlider("amplitude", 0.0, 1.0, &amplitude, 40, 150);
	
    // setup STK
    synth = new stk::Flute(50);
    stk::Stk::setSampleRate(44100.0);
    ofSoundStreamSetup(2, 0);
    
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
        learnJetDelay.addTrainingInstance(instance, jetDelay);
        learnJetReflection.addTrainingInstance(instance, jetReflection);
        learnEndReflection.addTrainingInstance(instance, endReflection);
        learnNoiseGain.addTrainingInstance(instance, noiseGain);
        learnBreathPressure.addTrainingInstance(instance, breathPressure);
        learnFrequency.addTrainingInstance(instance, frequency);
        learnAmplitude.addTrainingInstance(instance, amplitude);
        
    }
    
    // if we are in testing mode, we are continously classifying
    // the current state of the keys
    else if (testing) {
        vector<double> instance(keys.begin(), keys.end());
        jetDelay = learnJetDelay.predict(instance);
        jetReflection = learnJetReflection.predict(instance);
        endReflection = learnEndReflection.predict(instance);
        noiseGain = learnNoiseGain.predict(instance);
        breathPressure = learnBreathPressure.predict(instance);
        frequency = learnFrequency.predict(instance);
        amplitude = learnAmplitude.predict(instance);
    }
    
    // update STK
    synth->controlChange(ccVibratoGain, 0);
    synth->setJetReflection(jetReflection);
    synth->setEndReflection(endReflection);
    synth->controlChange(ccJetDelay, jetDelay);
    synth->controlChange(ccNoiseGain, noiseGain);
    synth->controlChange(ccBreathPressure, breathPressure);
    synth->setFrequency(frequency);
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
            learnJetDelay.trainRegression();
            learnJetReflection.trainRegression();
            learnEndReflection.trainRegression();
            learnNoiseGain.trainRegression();
            learnBreathPressure.trainRegression();
            learnFrequency.trainRegression();
            learnAmplitude.trainRegression();
        }
    }
    if (key=='3') {
        synth->noteOn(frequency, amplitude);
    }
    if (key=='4') {
        synth->noteOff(0);
    }
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "test") {
        testing = !testing;
        if (testing) {
            recording = false;
            learnJetDelay.trainRegression();
            learnJetReflection.trainRegression();
            learnEndReflection.trainRegression();
            learnNoiseGain.trainRegression();
            learnBreathPressure.trainRegression();
            learnFrequency.trainRegression();
            learnAmplitude.trainRegression();
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
void ofApp::audioOut(float *output, int bufferSize, int nChannnels){
    stk::StkFrames synth2Frames(bufferSize,1);
    synth->tick(synth2Frames);
    for (int i = 0; i < bufferSize; i++) {
        output[2*i] = synth2Frames(i,0);
        output[2*i+1] =  synth2Frames(i,0);
    }
}

//--------------------------------------------------------------
ofApp::~ofApp(){
    delete synth;
}
