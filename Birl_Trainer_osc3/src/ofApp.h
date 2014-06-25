#pragma once

#define BIRL_PORT 1234
#define SYNTH_HOST "localhost"
#define SYNTH_PORT 5678


#include "ofMain.h"
#include "Birl.h"
#include "SynthParameter.h"
#include "SynthManager.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();    
    void keyPressed(int key);
    
protected:
    SynthManager synthManager;
    Birl birl;
    bool recording;    
};
