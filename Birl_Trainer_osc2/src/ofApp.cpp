#include "ofApp.h"

//-------
void ofApp::setup(){
    birl.setup(BIRL_PORT);
    synthManager.setupOsc(SYNTH_HOST, SYNTH_PORT);
}

//-------
void ofApp::update(){
    synthManager.sendOsc();
}

//-------
void ofApp::draw(){
    birl.draw(50, 50, 200, 640);
}

//-------
void ofApp::keyPressed(int key){
}
