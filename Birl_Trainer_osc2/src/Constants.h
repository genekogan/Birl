#pragma once



#define SYNTH_HOST "localhost"
#define SYNTH_PORT 5678
#define BIRL_PORT 1234

#define MAX_OUTPUT_PARAMETERS 9

#define TRAIN_GUI_X 200
#define TRAIN_GUI_Y 40
#define TRAIN_GUI_W 820
#define TRAIN_GUI_H 750

#define BIRL_DRAW_X 12
#define BIRL_DRAW_Y 12
#define BIRL_DRAW_W 170
#define BIRL_DRAW_H 700


enum TrainState {
    TRAINING_SELECT_OUTPUTS,
    TRAINING_RECORD,
    TRAINING_PLAY };

