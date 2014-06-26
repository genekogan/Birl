#pragma once


#include "ofMain.h"
#include "ofxUI.h"
#include "Constants.h"
#include "OutputParameter.h"
#include "Birl.h"
#include "PresetManager.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void setMode(Mode mode);
    void draw();
    void keyPressed(int key);
    
protected:
    void checkIfDeleteParameters();
    void guiMainEvent(ofxUIEventArgs &e);
    void guiSelectEvent(ofxUIEventArgs &e);
    void guiMonitorEvent(ofxUIEventArgs &e);
    void guiRecordEvent(ofxUIEventArgs &e);
    void guiPlayEvent(ofxUIEventArgs &e);
    
    void toggleRecording(bool toRecord);
    void trainClassifiers(TrainMode trainMode);
    
    ofxUISuperCanvas *guiMain, *guiSelect, *guiMonitor, *guiRecord, *guiPlay;
    Mode mode;
    
    string trainingMessage;
    int numCollectedCurrent=0, numCollectedTotal=0;
    
    Birl birl;
    PresetManager presetManager;
    vector<OutputParameter *> outputParameters;
    
    bool recording, predicting, countingDown;
    float timerLast, timer;
    float countdown = 2;
    float duration = 3;
    
    ofTrueTypeFont geneva;
};
