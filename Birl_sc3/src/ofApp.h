#pragma once

#define PORT 1234

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "Instrument.h"

class ofApp : public ofBaseApp{
    
public:
    ~ofApp();
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
	void guiEvent(ofxUIEventArgs &e);
    
private:
    ofxUISuperCanvas *guiData, *guiOptions;
    
    ofxLearn learn1, learn2, learn3, learn4, learn5;
    
    ofxOscReceiver osc;
    vector<float> keys;

    
//    float tone, octave;
    
    
    bool recording;
    bool testing;
    
    Instrument rezonator;
    Instrument klanky;
    Instrument organ;
    Instrument pmosc;
    Instrument voices;

    
    bool isMLP = false;
};
