/*
    Birl Trainer GUI
*/


#pragma once

#define BIRL_OSC_IN 1234
#define OSC_OUT_HOST "localhost"
#define OSC_OUT_PORT 1236

#define DEBUG_MODE true
#define USE_FAKE_DATA true


#include "ofMain.h"
#include "Constants.h"
#include "Birl.h"
#include "BirlLearn.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void windowResized(int w, int h);

private:
    
    Birl birl;
    BirlLearner learn;
};
