#pragma once

#define PORT 1234

#include "ofMain.h"
#include "ofxLearn.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxStk.h"


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
    
    void audioOut(float *output,int bufferSize,int nChannnels);
    
private:
    // birl
    vector<float> keys;
    float label;

    // learn
    ofxUISuperCanvas *guiData, *guiOptions, *guiSynth;
    bool recording, testing;
    ofxOscReceiver osc;
    ofxLearn learnJetDelay, learnJetReflection, learnEndReflection,
            learnNoiseGain, learnBreathPressure, learnFrequency, learnAmplitude;

    // stk parameters
    stk::Flute *synth;
    float jetDelay, jetReflection, endReflection, noiseGain, breathPressure, frequency, amplitude;
    int ccVibratoGain=0, ccJetDelay=2, ccNoiseGain=4, ccBreathPressure=128;
    
};
