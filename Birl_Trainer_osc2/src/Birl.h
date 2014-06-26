#pragma once


#define MAX_KEY_VALUE 1000


#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"


class Birl
{
public:
    void setup(int port);
    void update();
    void draw(int x, int y, int w, int h);
    vector<double> getFeatureVector();
protected:
    ofxOscReceiver osc;
    //map<string, float> keys;
    float keys[9];
    
    ofxUISuperCanvas *featureSelector;
    bool featureKeys, featurePressure, featureEmbouchure;
};
