#include "ofApp.h"

//--------
void ofApp::setup()
{
    if (DEBUG_MODE)
        ofSetLogLevel(OF_LOG_VERBOSE);

    ofSetEscapeQuitsApp(false);

    /* setup birl */
    birl.setup(BIRL_OSC_IN);

    /* setup learning */
    learn.setupOscSender(OSC_OUT_HOST, OSC_OUT_PORT);
    learn.setup(birl);
}

//--------
void ofApp::update()
{
    if (USE_FAKE_DATA)
        birl.updateFakeData();
    
    birl.update();
    learn.update();
}

//--------
void ofApp::draw()
{
    ofBackgroundGradient(ofColor(240), ofColor(120));
    birl.draw(10, 50, 180, 690);
    learn.draw();
}

//--------
void ofApp::keyPressed(int key){
    if (key=='T') {
        learn.saveOutputsToTouchOsc();
    }
}

void ofApp::windowResized(int w, int h) {
    ofHideCursor();
    ofShowCursor();
}
