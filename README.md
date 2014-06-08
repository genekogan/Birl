machine learning framework for the Birl, using ofxLearn and ofxUI.

## Birl_Trainer

This application receives Birl input data and trains a regression (support vector machine) mapping to synthesis parameters.

To run it, you need openFrameworks installed, as well as the addons [ofxLearn](https://github.com/genekogan/ofxLearn) and [ofxUI](https://github.com/rezaali/ofxUI).

To run the Stk example, you also need [ofxStk](https://github.com/Ahbee/ofxStk).

For the ML functionality, we are using ofxLearn, which wraps the [dlib library](http://dlib.net/). 

todo:
 - saving/loading models, presets
 - better interface for performer
 - more STK models
