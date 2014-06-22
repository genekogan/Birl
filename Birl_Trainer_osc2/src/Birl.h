#pragma once

#include "ofMain.h"
#include "ofxOsc.h"


class Birl
{
public:
    void setup(int port);
    void update();
    void draw(int x, int y, int w, int h);
protected:
    ofxOscReceiver osc;
    map<string, float> keys;
};
