#pragma once


#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "Constants.h"
#include "OutputParameter.h"
#include "Birl.h"
#include "PresetManager.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
private:
    void setMode(Mode mode);
    void checkIfDeleteParameters();
    
    void guiMainEvent(ofxUIEventArgs &e);
    void guiSelectEvent(ofxUIEventArgs &e);
    void guiMonitorEvent(ofxUIEventArgs &e);
    void guiRecordEvent(ofxUIEventArgs &e);
    void guiPlayEvent(ofxUIEventArgs &e);
    
    void toggleRecording(bool toRecord);
    void trainClassifiers(TrainMode trainMode);
    void predictOutputParameters();
    void sendOutputParametersOsc();
    
    ofxUICanvas      *guiMain;
    ofxUISuperCanvas *guiSelect, *guiMonitor,
    *guiRecord, *guiPlay;
    Mode mode;
    
    string trainingMessage;
    int numCollectedCurrent=0, numCollectedTotal=0;
    
    Birl birl;
    PresetManager presetManager;
    vector<OutputParameter *> outputParameters;
    
    ofxOscSender osc;
    
    bool recording, predicting, countingDown, sendingOsc;
    float timerLast, timer;
    float countdown = 2;
    float duration = 2;
    
    ofTrueTypeFont geneva;
};
