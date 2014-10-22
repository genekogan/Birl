#include "ofApp.h"

//--------
void ofApp::setup()
{
    if (DEBUG_MODE)
        ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofSetFrameRate(1000);
    ofSetEscapeQuitsApp(false);

    /* setup birl */
    birl.setup(BIRL_OSC_IN);

    /* setup learning */
    learn.setupOscSender(OSC_OUT_HOST, OSC_OUT_PORT);
    learn.setup(&birl);
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
    ofBackgroundGradient(ofColor(BIRL_GRADIENT_COLOR_LIGHT), ofColor(BIRL_GRADIENT_COLOR_DARK));
    birl.draw(10, 50, 180, 684);
    learn.draw();
}

//--------
void ofApp::keyPressed(int key)
{

}

//--------
void ofApp::windowResized(int w, int h)
{
    ofHideCursor();
    ofShowCursor();
}
