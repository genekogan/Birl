#pragma once

#include "ofMain.h"
#include "OutputParameter.h"

class PresetManager
{
public:
    void setup(vector<OutputParameter *> *outputParameters);
    void loadPreset(string path); //, vector<OutputParameter *> &outputParameters);
    void savePreset(vector<OutputParameter *> &outputParameters);
    void setVisible(bool isVisible);
    bool isPredicting() { return predicting; }

private:
    void guiPerformEvent(ofxUIEventArgs &e);
    void guiPresetEvent(ofxUIEventArgs &e);
    
    bool predicting, sendingOsc;
    ofxUISuperCanvas *gui, *guiPresets;
    vector<OutputParameter *> *outputParameters;
};