#include "PresetManager.h"

//-------
void PresetManager::loadPreset(string path, vector<OutputParameter *> &outputParameters) {
    ofXml xml;
    
    for (int i=0; i<outputParameters.size(); i++) {
        outputParameters[i]->destroy();
    }
    outputParameters.clear();
    
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
                    
                    outputParameters.push_back(param);
                    
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
    //ofxUIDropDownList *dd = (ofxUIDropDownList *) gui->getWidget("presets");
    //dd->addToggle(newPresetName+".xml");
    
    cout << "Saved preset "<<newPresetName<< endl;
}

