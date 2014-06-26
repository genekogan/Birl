#include "SynthManager.h"

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
    gui->addDropDownList("presets", presetList, 360)->setAutoClose(true);
    
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addButton("save", false);
    gui->addButton("add parameter", false);
    
    ofAddListener(gui->newGUIEvent, this, &SynthManager::guiEvent);
}

//-------
void SynthManager::setupOsc(string host, int port) {
    osc.setup(host, port);
}

//-------
void SynthManager::guiEvent(ofxUIEventArgs &e) {
	if (e.getParent()->getName() == "presets" && e.getKind() == OFX_UI_WIDGET_LABELTOGGLE) {
        loadPreset(e.getName());
    }
    else if (e.getName() == "save") {
        if (((ofxUIButton *) e.widget)->getValue()==1) {
            savePreset();
        }
    }
    else if (e.getName() == "add parameter") {
        if (((ofxUIButton *) e.widget)->getValue()==1) {
            string paramName = ofSystemTextBoxDialog("name of new parameter");
            SynthParameter *newParameter = new SynthParameter(paramName, 0, 1);
            newParameter->setGuiPosition(400, 150+(SYNTH_PARAMETER_HEIGHT+12)*synthParameters.size());
            synthParameters.push_back(newParameter);
        }
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
    ofXml xml;
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
                    param->setGuiPosition(400, 150+(SYNTH_PARAMETER_HEIGHT+12)*synthParameters.size());
                    synthParameters.push_back(param);
                }
                while (xml.setToSibling());
                xml.setToParent();
            }
        }
        while (xml.setToSibling());
    }
}

//-------
void SynthManager::savePreset() {
    string newPresetName = ofSystemTextBoxDialog("name of preset");
    
    ofXml xml;
    xml.addChild("Preset");
    xml.setTo("Preset");
    
    xml.addChild("Metadata");
    xml.setTo("Metadata");
    xml.addValue("Name", newPresetName);
    xml.setToParent();
    
    xml.addChild("Parameters");
    xml.setTo("Parameters");
    
    for (int i=0; i<synthParameters.size(); i++) {
        ofXml parameter;
        parameter.addChild("Parameter");
        parameter.setTo("Parameter");
        parameter.addValue("Name", synthParameters[i]->getName());
        parameter.addValue("MinValue", synthParameters[i]->getMinValue());
        parameter.addValue("MaxValue", synthParameters[i]->getMaxValue());
        xml.addXml(parameter);
    }
    xml.setToParent();
    xml.save(ofToString(newPresetName+".xml"));
    
    // add new preset to dropdown list
    //ofxUIDropDownList *dd = (ofxUIDropDownList *) gui->getWidget("presets");
    //dd->addToggle(newPresetName+".xml");
}

