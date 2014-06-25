#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "SynthParameter.h"


class SynthManager
{
public:
    SynthManager();
    void setupOsc(string host, int port);
    void loadPreset(string path);
    void sendOsc();
    vector<SynthParameter *> *getSynthParameters() { return &synthParameters; }
    
protected:
    void savePreset();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    
    string name;
    vector<SynthParameter *> synthParameters;
    
    ofxOscSender osc;
};