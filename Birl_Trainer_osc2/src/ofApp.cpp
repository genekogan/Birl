#include "ofApp.h"

//-------
void ofApp::setup(){
    birl.setup(BIRL_PORT);
    synthManager.setupOsc(SYNTH_HOST, SYNTH_PORT);
}

//-------
void ofApp::update(){
    birl.update();
    
    if (recording) {
        vector<double> features = birl.getFeatureVector();
        vector<SynthParameter *> *synths = synthManager.getSynthParameters();
        for (int i=0; i<synths->size(); i++) {
            if (synths->at(i)->isTraining()) {
                synths->at(i)->addInstance(features);
            }
        }
    }
    
    synthManager.sendOsc();
}

//-------
void ofApp::draw(){
    birl.draw(50, 50, 160, 560);
}

//-------
void ofApp::keyPressed(int key){
}
