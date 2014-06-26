#pragma once

#define PORT 1234

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxStk.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void trainFromFile(string path);
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
    ofxLearn mlp;
    ofxOscReceiver osc;
    vector<float> keys;
    float label;
    bool recording;
    bool testing;
};
