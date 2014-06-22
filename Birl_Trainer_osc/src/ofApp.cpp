#include "ofApp.h"

void SynthParameter::setup(string name, float minVal, float maxVal) {
    this->name = name;
    this->minVal = minVal;
    this->maxVal = maxVal;
    value = 0.5 * (minVal + maxVal);
    gui = new ofxUICanvas(name);
    gui->setWidth(400);
    slider = gui->addSlider(name, minVal, maxVal, &value);
    gui->addTextInput("valuestr", ofToString(value))->setAutoClear(false);
    gui->addTextInput("minstr", ofToString(minVal))->setAutoClear(false);
    gui->addTextInput("maxstr", ofToString(maxVal))->setAutoClear(false);
    ofAddListener(gui->newGUIEvent,this,&SynthParameter::guiEvent);
}


void SynthParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "valuestr") {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        value = ofToFloat(ti->getTextString());
    }
    else if (e.getName() == "minstr") {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        minVal = ofToFloat(ti->getTextString());
        //slider->setMin(minVal);
        //ofxUISlider *sl = (ofxUISlider *) gui->getWidget(name);
        //sl->setMin(minVal);
    }
    else if (e.getName() == "maxstr") {
        ofxUITextInput *ti = (ofxUITextInput *) e.widget;
        maxVal = ofToFloat(ti->getTextString());
//        ofxUISlider *sl = (ofxUISlider *) gui->getWidget(name);
//        sl->setMax(maxVal);
    }
    else if (e.getName() == name) {
        ofxUITextInput *ti = (ofxUITextInput *) gui->getWidget("valuestr");
        ti->setTextString(ofToString(value));
    }
}



//--------------------------------------------------------------
void ofApp::setup(){
	sender.setup(HOST, PORT);
    osc.setup(BIRL_PORT);
    
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
    guiData->addSlider("label", 0, 1, &label, 40, 150);
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
    
 
    synthParameter.setup("pitch", 0, 127);
    synthParameter.setGuiPosition(100, 300);
    
    synthPressure.setup("pressure", 0, 1);
    synthPressure.setGuiPosition(350, 400);
    
    synthDistortion.setup("distortion", 0, 5);
    synthDistortion.setGuiPosition(600, 500);
    
    synthNoiseGain.setup("noisegain", 0, 1);
    synthNoiseGain.setGuiPosition(850, 600);

}

//--------------------------------------------------------------
void ofApp::update(){

    //synthParameter.update(&sender);
    
    
    ofxOscMessage m;
    m.setAddress("/birl/pitch");
    m.addFloatArg(synthParameter.getValue());
    sender.sendMessage(m);

    m.clear();
    m.setAddress("/birl/pressure");
    m.addFloatArg(synthPressure.getValue());
    sender.sendMessage(m);
    
    m.clear();
    m.setAddress("/birl/distortion");
    m.addFloatArg(synthDistortion.getValue());
    sender.sendMessage(m);

    m.clear();
    m.setAddress("/birl/noisegain");
    m.addFloatArg(synthNoiseGain.getValue());
    sender.sendMessage(m);

    
    
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
        label = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 1);
        vector<double> instance(keys.begin(), keys.end());
        mlp.addTrainingInstance(instance, label);
    }
    
    // if we are in testing mode, we are continously classifying
    // the current state of the keys
    else if (testing) {
        vector<double> instance(keys.begin(), keys.end());
        label = mlp.predict(instance);
        int label2 = ofMap(label, 0, 1, 0, 12);
        cout << "label is " << label2 << endl;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(235);
    
    // if recording instances, draw a red dot on top right corner
    if (recording) {
        ofSetColor(255, 0, 0);
        ofEllipse(ofGetWidth()-50, 50, 50, 50);
    }
    
    // draw the label
    ofSetColor(0, 255, 0);
    ofEllipse(ofMap(label, 0, 1, 0, ofGetWidth()), 500, 100, 100);
    ofSetColor(255);
    ofDrawBitmapString(ofToString(label), ofMap(label, 0, 1, 0, ofGetWidth()), 500);
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
            mlp.trainRegression();
        }
    }
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "test") {
        if (testing) {
            recording = false;
            mlp.trainRegression();
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
