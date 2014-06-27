#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxLearn.h"
#include "Constants.h"


class OutputParameter
{
public:
    OutputParameter(int idx, string name, float minValue, float maxValue);
    void reindex(int idx);
    void destroy();
    
    int getIndex()              { return idx; }
    string getName()            { return name; }
    string getOscAddress()      { return oscAddress; }
    int getNumExamples()        { return classifier.getNumberTrainingInstances(); }
    float getValue()            { return value; }
    float getMinValue()         { return minValue; }
    float getMaxValue()         { return maxValue; }
    bool isTraining()           { return training; }
    bool isTrained()            { return trained; }
    bool getToDelete()          { return toDelete; }
    
    bool getInputKeys()         { return inputKeys; }
    bool getInputKeysDiscrete() { return inputKeysDiscrete; }
    bool getInputPressure()     { return inputPressure; }
    bool getInputEmbouchure()   { return inputEmbouchure; }
    
    bool hasInput()             { return inputKeys | inputKeysDiscrete | inputPressure | inputEmbouchure; };
    
    bool setInputKeys(bool b)         { inputKeys = b; }
    bool setInputKeysDiscrete(bool b) { inputKeysDiscrete = b; }
    bool setInputPressure(bool b)     { inputPressure = b; }
    bool setInputEmbouchure(bool b)   { inputEmbouchure = b; }
    
    void setMode(Mode mode);
    
    void addExample(vector<double> example);
    void trainClassifier(TrainMode trainMode);
    void classifyExample(vector<double> example);
    
    string saveClassifier(string presetName);
    void loadClassifier(string path);
    
    void sendOsc(ofxOscSender &osc);
    
protected:
    void guiEvent(ofxUIEventArgs &e);
    
    int idx;
    string name;
    float value, minValue, maxValue;
    string oscAddress;
    bool toDelete = false;
    
    bool inputKeys=false, inputKeysDiscrete=false, inputPressure=false, inputEmbouchure=false;
    bool training = false;
    bool trained = false;
    
    ofxUISuperCanvas *guiTrain1, *guiTrain2;
    
    ofxLearn classifier;
};