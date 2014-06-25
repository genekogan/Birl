#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxLearn.h"
#include "Constants.h"


class OutputParameter
{
public:
    OutputParameter(int idx, string name, float minValue, float maxValue);
    void reindex(int idx);
    void destroy();

    string getName()            { return name; }
    string getOscAddress()      { return oscAddress; }
    float getValue()            { return value; }
    float getMinValue()         { return minValue; }
    float getMaxValue()         { return maxValue; }
    bool isTraining()           { return training; }
    bool getToDelete()          { return toDelete; }
    
    bool getInputKeys()         { return inputKeys; }
    bool getInputKeysDiscrete() { return inputKeysDiscrete; }
    bool getInputPressure()     { return inputPressure; }
    bool getInputEmbouchure()   { return inputEmbouchure; }
    bool hasInput()             { return inputKeys | inputKeysDiscrete | inputPressure | inputEmbouchure; };
    
    void setTrainState(TrainState trainState);
    
    void addInstance(vector<double> instance);
    void trainClassifierFast();
    void trainClassifierAccurate();
    void classifyInstance(vector<double> instance);
    
protected:
    void guiEvent(ofxUIEventArgs &e);
    
    int idx;
    string name;
    float value, minValue, maxValue;
    string oscAddress;
    bool toDelete = false;
    
    bool inputKeys=false, inputKeysDiscrete=false, inputPressure=false, inputEmbouchure=false;
    bool training = false;
    bool isTrained = false;
    
    ofxUISuperCanvas *guiTrain1, *guiTrain2;
    
    ofxLearn classifier;
};