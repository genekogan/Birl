#include "ofMain.h"
#include "ofxLearn.h"


#define EXP 2.71828182845904523536028747135266249775724709369995


class NeuralNet
{
public:
    void setup(ofxLearn *learn, int numFeatures, int numLayers);
    double predict(vector<double> example);

private:
    void getCoefficients();
    double sigmoid(double x);

    ofxLearn *learn;
    int numFeatures, numLayers;
    vector<double> w1, w3;
};

