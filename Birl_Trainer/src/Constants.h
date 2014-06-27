#pragma once

//------- OSC CONSTANTS

#define SYNTH_HOST "localhost"
#define SYNTH_PORT 5678
#define BIRL_PORT 1234


//------- BIRL INPUT CONSTANTS

#define USE_FAKE_DATA false

#define KEYS_NUMBER 9
#define KEYS_MAX 1000
#define KEYS_DISCRETE_THRESHOLD 0.5

 // if true, we use only continuous key values for prediction (never discrete)
#define PREDICTING_DISALLOW_KEYS_DISCRETE true

#define PRESSURE_NUMBER 2

#define EMBOUCHURE_NUMBER 20
#define EMBOUCHURE_MAX 4096


//------- OUTPUT CONSTANTS

#define MAX_OUTPUT_PARAMETERS 8
 // forces predictions on output parameters to be clamped to the specified range
#define FORCE_CLAMP_OUTPUT_PARAMETERS true

//------- POSITIONING CONSTANTS

#define GUI_TRAIN_X 200
#define GUI_TRAIN_Y 30
#define GUI_TRAIN_W 820
#define GUI_TRAIN_H 165

#define GUI_PARAMETERS_X 200
#define GUI_PARAMETERS_Y 200
#define GUI_PARAMETERS_W 820
#define GUI_PARAMETERS_H 594

#define PARAMETER_SELECT_H 146
#define PARAMETER_VIEW_H 132

#define BIRL_DRAW_X 12
#define BIRL_DRAW_Y 60
#define BIRL_DRAW_W 170
#define BIRL_DRAW_H 700

#define BIRL_DRAW_EMBOUCHURE_H 80
#define BIRL_DRAW_PRESSURE_H 40


//------- COLOR CONSTANTS


enum Mode {
    PERFORMANCE,
    TRAINING_SELECT_OUTPUTS,
    TRAINING_RECORD,
    TRAINING_PLAY };
