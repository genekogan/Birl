#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxLearn.h"
#include "ofxOsc.h"


#define SYNTH_PARAMETER_HEIGHT 180


class SynthParameter
{
public:
    SynthParameter(string name, float minVal, float maxVal);
    void setGuiPosition(int x, int y) { gui->setPosition(x, y); }
    void clear();
    
    string getName() { return name; }
    string getOscAddress() { return oscAddress->getTextString(); }
    float getValue() { return value; }
    float getMinValue() { return minVal; }
    float getMaxValue() { return maxVal; }
    bool isTraining() { return training; }
    
    void addInstance(vector<double> instance);
    void trainClassifier();
    void classifyInstance(vector<double> instance);
    
protected:
    void guiEvent(ofxUIEventArgs &e);
    ofxUISuperCanvas *gui;
    
    string name;
    float value, minVal, maxVal;
    
    ofxLearn classifier;
    bool training=false;
    
    ofxUISlider *sliderValue;
    ofxUITextInput *oscAddress;
    ofxUITextInput *textMinVal, *textMaxVal, *textValue;
};