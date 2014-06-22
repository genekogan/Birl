#pragma once

#define HOST "localhost"
#define PORT 5678
#define BIRL_PORT 1234

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxUI.h"
#include "ofxOsc.h"

class SynthParameter
{
public:
    void setup(string name, float minVal, float maxVal);
    float getValue() {return value;}
    void setGuiPosition(int x, int y) { gui->setPosition(x, y); }
    void guiEvent(ofxUIEventArgs &e);
    string name;
    float value, minVal, maxVal;

    ofxUICanvas *gui;
    ofxUISlider *slider;
    

};


class ofApp : public ofBaseApp
{
public:
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
    ofxLearn mlp;
    vector<float> keys;
    float label;
    bool recording;
    bool testing;
    
    

    SynthParameter synthParameter;
    
    SynthParameter synthPressure, synthDistortion, synthNoiseGain;

    
    ofxOscReceiver osc;
    ofxOscSender sender;
};
