#include "NeuralNet.h"


//---------
void NeuralNet::setup(ofxLearn *learn, int numFeatures, int numLayers) {
    this->learn = learn;
    this->numFeatures = numFeatures;
    this->numLayers = numLayers;
    getCoefficients();
}

//---------
void NeuralNet::getCoefficients() {
    dlib::matrix<double> w1m = learn->getRegressionMlp()->get_w1();
    dlib::matrix<double> w3m = learn->getRegressionMlp()->get_w3();
    for (int i=0; i<numLayers+1; i++) {
        for (int j=0; j<numFeatures+1; j++) {
            w1.push_back(w1m(i, j));
        }
    }

    for (int i=0; i<numLayers+1; i++) {
        w3.push_back(w3m(i));
    }
}

//---------
double NeuralNet::sigmoid(double x) {
    return 1.0/(1.0 + pow(EXP, -x));
}

//---------
double NeuralNet::predict(vector<double> example) {
    vector<double> z;
    for (int i=0; i<example.size(); i++) {
        z.push_back(example[i]);
    }
    z.push_back(-1);
    
    vector<double> tmp1;
    for (int i=0; i<numLayers+1; i++) {
        float tmp0 = 0.0;
        for (int j=0; j<numFeatures+1; j++) {
            tmp0 += ( w1[ i*(numFeatures+1) + j ] * z[j] );
        }
        tmp1.push_back( sigmoid(tmp0) );
    }
    tmp1[numLayers] = -1.0;    // bias term overrides last element
    
    float tmp2 = 0.0;
    for (int j=0; j<numLayers+1; j++) {
        tmp2 += (w3[j] * tmp1[j]);
    }
    return sigmoid(tmp2);
}