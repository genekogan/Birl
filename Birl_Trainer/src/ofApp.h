#pragma once


#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "Constants.h"
#include "OutputParameter.h"
#include "Birl.h"
#include "PresetManager.h"




class ClassifierThread : public ofThread
{
public:
    bool isTraining;
    
    vector<OutputParameter *> *outputParameters;
    vector<bool> toTrain;
    TrainMode trainMode;
    string progressString;
    
    ClassifierThread(vector<OutputParameter *> *outputParameters){
        this->outputParameters = outputParameters;
        isTraining = false;
    }
    
    void start(vector<bool> toTrain, TrainMode trainMode){
        this->toTrain = toTrain;
        this->trainMode = trainMode;
        isTraining = true;
        startThread(true, false);   // blocking, verbose
    }

    void threadedFunction(){
        while( isThreadRunning() != 0 ){
            if( lock() ){
                cout << "start training classifiers." << endl;
                progressString = "Training! 0/"+ofToString(outputParameters->size())+" classifiers done...";
                for (int i=0; i<outputParameters->size(); i++) {
                    progressString = "Training! "+ofToString(i)+"/"+ofToString(outputParameters->size())+" classifiers done...";
                    if (toTrain[i]) {
                        outputParameters->at(i)->trainClassifier(trainMode);
                    }
                }
                ofSleepMillis(2000);
                isTraining = false;
                unlock();
                stopThread();
            }
        }
    }
    
    string getProgressString() {
        return progressString;
    }
    
    bool getIsTraining() {
        return isTraining;
    }
};


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
    bool trainClassifiers(TrainMode trainMode);
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
    
    bool recording, predicting, countingDown;
    bool sendingOsc = true;
    float timerLast, timer;
    float countdown = 2;
    float duration = 2;
    
    ofTrueTypeFont geneva;
    ofColor trainingBgColor;
    int idxStyle=-1;
    
    ClassifierThread *classifierThread;
};
