machine learning framework for the Birl, using ofxLearn and ofxUI.

## Birl_Trainer

This application receives Birl input data and trains a neural net (multilayer perceptron) mapping to synthesis parameters.

To run it, you need openFrameworks installed, as well as the addons [ofxLearn](https://github.com/genekogan/ofxLearn) and [ofxUI](https://github.com/rezaali/ofxUI).

For the ML functionality, we are using ofxLearn, which wraps the [dlib library](http://dlib.net/). 

todo:
 - interface for performer to choose synthesis parameters during recording
 - implement grid parameter search to optimize regression accuracy
 - saving/loading models, presets
