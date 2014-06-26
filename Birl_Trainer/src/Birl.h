#pragma once


#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxUI.h"
#include "Constants.h"


class Birl
{
public:
    void setup(int port);
    void update();
    void updateFake();
    void draw(int x, int y, int w, int h);

    vector<float> getKeys();
    vector<float> getKeysDiscrete();
    vector<float> getPressure();
    vector<float> getEmbouchure();

protected:
    ofxOscReceiver osc;
    vector<float> keys;
    vector<float> keysDiscrete;
    vector<float> pressure;
    vector<float> embouchure;
    
    float maxbreathpos = 45000.0f;
    float maxbreathneg = 15000.0f;
};
