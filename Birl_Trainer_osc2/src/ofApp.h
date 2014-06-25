#pragma once


#include "ofMain.h"
#include "ofxUI.h"
#include "Constants.h"
#include "OutputParameter.h"
#include "Birl.h"
#include "Presets.h"


class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void setTrainState(TrainState trainState);
    void draw();    
    void keyPressed(int key);
    
protected:
    void checkIfDeleteParameters();
    void guiSelectEvent(ofxUIEventArgs &e);
    void guiMonitorEvent(ofxUIEventArgs &e);
    void guiRecordEvent(ofxUIEventArgs &e);
    void guiPlayEvent(ofxUIEventArgs &e);
    
    ofxUISuperCanvas *guiSelect, *guiMonitor, *guiRecord, *guiPlay;
    TrainState trainState;
    
    string trainingMessage;
    int numCollectedCurrent=0, numCollectedTotal=0;
    
    vector<OutputParameter *> outputParameters;
    
    Birl birl;
    
    bool recording, predicting, countingDown;
    float timerLast, timer;
    float countdown = 2;
    float duration = 3;
    
    ofTrueTypeFont geneva;
};
