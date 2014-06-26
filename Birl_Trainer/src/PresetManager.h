#pragma once

#include "ofMain.h"
#include "OutputParameter.h"

class PresetManager
{
public:
    void loadPreset(string path, vector<OutputParameter *> &outputParameters);
    void savePreset(vector<OutputParameter *> &outputParameters);
};