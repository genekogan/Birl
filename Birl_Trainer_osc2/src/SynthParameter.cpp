#include "SynthParameter.h"

//-------
SynthParameter::SynthParameter(string name, float minVal, float maxVal) {
    this->name = name;
    this->minVal = minVal;
    this->maxVal = maxVal;
    value = 0.5 * (minVal + maxVal);
    
    gui = new ofxUISuperCanvas(name);
    gui->setWidth(540);
    gui->setHeight(140);
    gui->setDrawPadding(true);
    gui->addSpacer();
    sliderValue = gui->addSlider("value", minVal, maxVal, &value);
    
    gui->addLabel("min");
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    
    textMinVal = gui->addTextInput("textMin", ofToString(minVal), 80.0f);
    
    gui->addLabel("max");
    textMaxVal = gui->addTextInput("textMax", ofToString(maxVal), 80.0f);
    
    gui->addLabel("value");
    textValue  = gui->addTextInput("textValue", ofToString(value), 80.0f);
    
    textMinVal->setAutoClear(false);
    textMaxVal->setAutoClear(false);
    textValue->setAutoClear(false);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    gui->addLabel("OSC");
    oscAddress = gui->addTextInput("osc address", ofToString("/birl/"+name));
    oscAddress->setAutoClear(false);
    ofAddListener(gui->newGUIEvent, this, &SynthParameter::guiEvent);
}

//-------
void SynthParameter::clear() {
    delete gui;
}

//-------
void SynthParameter::guiEvent(ofxUIEventArgs &e) {
    if (e.getName() == "textMin") {
        minVal = ofToFloat(textMinVal->getTextString());
        sliderValue->setMin(minVal);
        sliderValue->setValue(ofToFloat(textValue->getTextString()));
    }
    else if (e.getName() == "textMax") {
        maxVal = ofToFloat(textMaxVal->getTextString());
        sliderValue->setMax(maxVal);
        sliderValue->setValue(ofToFloat(textValue->getTextString()));
    }
    else if (e.getName() == "textValue") {
        value = ofToFloat(textValue->getTextString());
    }
    else if (e.getName() == "value") {
        textValue->setTextString(ofToString(value));
    }
}

//-------
void SynthParameter::addInstance(vector<double> instance) {
    classifier.addTrainingInstance(instance, value);
}

//-------
void SynthParameter::trainClassifier() {
    classifier.trainRegression();
}

//-------
void SynthParameter::classifyInstance(vector<double> instance) {
    value = classifier.predict(instance);
}



//-------
SynthManager::SynthManager() {
    gui = new ofxUICanvas();
    gui->addLabel("presets");
    gui->setPosition(400, 5);
    gui->setWidth(560);
    gui->setHeight(100);
    
    // load presets
    ofDirectory dir;
	dir.allowExt("xml");
	dir.open(".");
	dir.listDir();
    vector<string> presetList;
	for(int i = 0; i < dir.size(); i++) {
		presetList.push_back(dir.getName(i));
	}
    gui->addDropDownList("presets", presetList)->setAutoClose(true);
    ofAddListener(gui->newGUIEvent, this, &SynthManager::guiEvent);
}

//-------
void SynthManager::setupOsc(string host, int port) {
    osc.setup(host, port);
}

//-------
void SynthManager::guiEvent(ofxUIEventArgs &e) {
	if (e.getParent()->getName() == "presets" && e.getKind() == OFX_UI_WIDGET_LABELTOGGLE) {
        ofxUIDropDownList *dd = (ofxUIDropDownList *) e.widget;
        loadPreset(e.getName());
    }
}

//-------
void SynthManager::sendOsc() {
    for (int i=0; i<synthParameters.size(); i++) {
        ofxOscMessage m;
        m.setAddress(synthParameters[i]->getOscAddress());
        m.addFloatArg(synthParameters[i]->getValue());
        osc.sendMessage(m);
    }
}

//-------
void SynthManager::loadPreset(string path) {
    for (int i=0; i<synthParameters.size(); i++) {
        synthParameters[i]->clear();
    }
    synthParameters.clear();
    xml.load(path);
    if(xml.exists("Metadata")) {
        xml.setTo("Metadata[0]");
        name = xml.getValue<string>("Name");
    }
    xml.setToParent();
    if(xml.exists("Parameters")) {
        xml.setTo("Parameters[0]");
        do {
            if (xml.getName() == "Parameters" && xml.setTo("Parameter[0]")) {
                do {
                    string paramName = xml.getValue<string>("Name");
                    float minValue = xml.getValue<float>("MinValue");
                    float maxValue = xml.getValue<float>("MaxValue");
                    SynthParameter *param = new SynthParameter(paramName, minValue, maxValue);
                    param->setGuiPosition(400, 200+160*synthParameters.size());
                    synthParameters.push_back(param);
                }
                while (xml.setToSibling());
                xml.setToParent();
            }
        }
        while (xml.setToSibling());
    }
}
