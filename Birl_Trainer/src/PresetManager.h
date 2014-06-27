#pragma once

#include "ofMain.h"
#include "OutputParameter.h"

class PresetManager
{
public:
    void setup(vector<OutputParameter *> *outputParameters);
    
    void loadPreset(string path);
    void savePreset(vector<OutputParameter *> &outputParameters);
    
    void setVisible(bool isVisible);
    
    bool isPredicting() { return predicting; }
    bool isSendingOsc() { return sendingOsc; }
    
    void setPredicting(bool predicting);
    void setSendingOsc(bool sendingOsc);

    void setTheme(int idxStyle);

private:
    void guiPerformEvent(ofxUIEventArgs &e);
    void guiPresetEvent(ofxUIEventArgs &e);
    
    bool predicting, sendingOsc;

    ofxUISuperCanvas *gui, *guiPresets;
    
    vector<OutputParameter *> *outputParameters;
};