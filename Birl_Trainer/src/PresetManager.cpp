#include "PresetManager.h"



//-------
void PresetManager::setTheme(int idxStyle) {
    gui->setTheme(idxStyle);
    guiPresets->setTheme(idxStyle);
}

//-------
void PresetManager::setup(vector<OutputParameter *> *outputParameters) {
    this->outputParameters = outputParameters;
    
    gui = new ofxUISuperCanvas("Performance mode");
    gui->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    gui->setWidgetSpacing(14);
    gui->setGlobalButtonDimension(72);
    gui->setPosition(GUI_TRAIN_X+4, GUI_TRAIN_Y+4);
    gui->setWidth(0.5*GUI_TRAIN_W);
    gui->setHeight(GUI_TRAIN_H-8);
    gui->addToggle("send osc", &sendingOsc)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
    gui->addToggle("predicting", &predicting)->setLabelPosition(OFX_UI_WIDGET_POSITION_DOWN);
    
    // load presets
    ofDirectory dir;
	dir.allowExt("xml");
	dir.open("presets/");
	dir.listDir();
    vector<string> presetList;
	for(int i = 0; i < dir.size(); i++) {
		presetList.push_back(dir.getName(i));
	}
    
    // preset menu
    guiPresets = new ofxUISuperCanvas("Presets");
    guiPresets->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
    guiPresets->setWidgetSpacing(14);
    guiPresets->setGlobalButtonDimension(72);
    guiPresets->setPosition(GUI_TRAIN_X+0.5*GUI_TRAIN_W+4, GUI_TRAIN_Y+4);
    guiPresets->setWidth(0.5*GUI_TRAIN_W-8);
    guiPresets->setHeight(GUI_TRAIN_H-8);
    guiPresets->addDropDownList("presets", presetList, 220)->setAutoClose(true);

    ofAddListener(gui->newGUIEvent, this, &PresetManager::guiPerformEvent);
    ofAddListener(guiPresets->newGUIEvent, this, &PresetManager::guiPresetEvent);
}

//-------
void PresetManager::setVisible(bool isVisible) {
    gui->setVisible(isVisible);
    guiPresets->setVisible(isVisible);
}

//-------
void PresetManager::guiPerformEvent(ofxUIEventArgs &e) {
}

//-------
void PresetManager::setPredicting(bool predicting) {
    this->predicting = predicting;
}

//-------
void PresetManager::setSendingOsc(bool sendingOsc) {
    this->sendingOsc = sendingOsc;
}

//-------
void PresetManager::guiPresetEvent(ofxUIEventArgs &e) {
	if (e.getParent()->getName() == "presets" && e.getKind() == OFX_UI_WIDGET_LABELTOGGLE) {
        loadPreset("presets/"+e.getName()); //, *outputParameters);
        for (int i=0; i<outputParameters->size(); i++) {
            outputParameters->at(i)->setMode(PERFORMANCE);
        }
    }
}

//-------
void PresetManager::loadPreset(string path) {
    ofXml xml;
    
    for (int i=0; i<outputParameters->size(); i++) {
        outputParameters->at(i)->destroy();
    }
    outputParameters->clear();
    
    xml.load(path);
    if(xml.exists("Metadata")) {
        xml.setTo("Metadata[0]");
        //name = xml.getValue<string>("Name");  // <-- no name needed at the moment
    }
    xml.setToParent();
    if(xml.exists("Parameters")) {
        xml.setTo("Parameters[0]");
        do {
            if (xml.getName() == "Parameters" && xml.setTo("Parameter[0]")) {
                do {
                    
                    int idx = xml.getValue<int>("Index");
                    string paramName = xml.getValue<string>("Name");
                    
                    float minValue = xml.getValue<float>("MinValue");
                    float maxValue = xml.getValue<float>("MaxValue");

                    string oscAddress = xml.getValue<string>("OscAddress");
                    
                    bool inputKeys = xml.getValue<string>("InputKeys") == "True" ? true : false;
                    bool inputKeysDiscrete = xml.getValue<string>("InputKeysDiscrete") == "True" ? true : false;
                    bool inputPressure = xml.getValue<string>("InputPressure") == "True" ? true : false;
                    bool inputEmbouchure = xml.getValue<string>("InputEmbouchure") == "True" ? true : false;
                    
                    OutputParameter *param = new OutputParameter(idx, paramName, minValue, maxValue);
                    
                    param->setInputKeys(inputKeys);
                    param->setInputKeysDiscrete(inputKeysDiscrete);
                    param->setInputPressure(inputPressure);
                    param->setInputEmbouchure(inputEmbouchure);
                    param->reindex(idx);
                    
                    outputParameters->push_back(param);
                    
                    string classifierPath = xml.getValue<string>("Classifier");
                    if (classifierPath != "") {
                        param->loadClassifier(classifierPath);
                    }
                }
                while (xml.setToSibling());
                xml.setToParent();
            }
        }
        while (xml.setToSibling());
    }
    
    //  by default, after loading a preset, set sending osc and predicting to true
    predicting = true;
    sendingOsc = true;
}

//-------
void PresetManager::savePreset(vector<OutputParameter *> &outputParameters) {
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
    
    for (int i=0; i<outputParameters.size(); i++) {
        ofXml parameter;
        parameter.addChild("Parameter");
        parameter.setTo("Parameter");
        parameter.addValue("Index", outputParameters[i]->getIndex());
        parameter.addValue("Name", outputParameters[i]->getName());
        parameter.addValue("MinValue", outputParameters[i]->getMinValue());
        parameter.addValue("MaxValue", outputParameters[i]->getMaxValue());
        parameter.addValue("OscAddress", outputParameters[i]->getOscAddress());

        parameter.addValue("InputKeys", outputParameters[i]->getInputKeys() ? "True" : "False");
        parameter.addValue("InputKeysDiscrete", outputParameters[i]->getInputKeysDiscrete() ? "True" : "False");
        parameter.addValue("InputPressure", outputParameters[i]->getInputPressure() ? "True" : "False");
        parameter.addValue("InputEmbouchure", outputParameters[i]->getInputEmbouchure() ? "True" : "False");

        if (outputParameters[i]->isTrained()) {
            string classifierPath = outputParameters[i]->saveClassifier(newPresetName);
            parameter.addValue("Classifier", classifierPath);
        }
        
        xml.addXml(parameter);
    }
    xml.setToParent();
    xml.save(ofToString("presets/"+newPresetName+".xml"));
    
    // add new preset to dropdown list
    ofxUIDropDownList *dd = (ofxUIDropDownList *) guiPresets->getWidget("presets");
    dd->addToggle("presets/"+newPresetName+".xml");
    
    cout << "Saved preset "<<newPresetName<< endl;
}

