#include "Birl.h"

//-------
void Birl::setup(int port) {
	osc.setup(port);
    for (int i=0; i<9; i++) {
        //keys["/birl/keys/"+ofToString(i)] = 0.0;
        keys[i] = 0.0;
    }
    
    featureSelector = new ofxUISuperCanvas("features");
    featureSelector->setPosition(16, ofGetHeight()-115);
    featureSelector->setWidth(185);
    featureSelector->setHeight(110);
    featureSelector->addLabel("features");
    featureSelector->addToggle("keys", &featureKeys);
    featureSelector->addToggle("pressure", &featurePressure);
    featureSelector->addToggle("embouchure", &featureEmbouchure);
}

//-------
void Birl::update() {
    while(osc.hasWaitingMessages()){
        ofxOscMessage m;
        osc.getNextMessage(&m);
        cout << "got osc message " << endl;
        cout << m.getAddress() << endl;
        if (m.getAddress() == "/birl/keys/") {
            for (int i=0; i<9; i++) {
                keys[i] = ofClamp(ofMap(m.getArgAsInt32(i), 0, MAX_KEY_VALUE, 0, 1), 0, 1);
            }
        }
//        keys[m.getAddress()] = ofClamp(ofMap(m.getArgAsFloat(0), 0, MAX_KEY_VALUE, 0, 1), 0, 1);
    }
}

vector<double> Birl::getFeatureVector() {
    vector<double> features;
    if (featureKeys) {
        for (int i=0; i<9; i++) {
            //features.push_back(keys["/JT"+ofToString(i)]);
            features.push_back(keys[i]); // keys begin, end
        }
    }
    else if (featurePressure) {
        
    }
    else if (featureEmbouchure) {
        
    }
    cout << "features: "<<ofToString(features) << endl;
    return features;
}

//-------
void Birl::draw(int x, int y, int w, int h) {
    ofPoint fingerPositions[9] = {
        ofPoint(0.6,  0.1),
        ofPoint(0.5,  0.19),
        ofPoint(0.6,  0.28),
        ofPoint(0.8,  0.37),
        
        ofPoint(0.5,  0.52),
        ofPoint(0.5,  0.61),
        ofPoint(0.4,  0.7),
        ofPoint(0.2,  0.79),
        ofPoint(0.15, 0.86)
    };
    
    ofPushMatrix();
    ofTranslate(x, y);
    
    ofFill();
    ofSetColor(200, 60, 30);
    ofRect(0, 0, w, h);
    
    for (int i=0; i<9; i++) {
        ofPushMatrix();
        ofTranslate(w * fingerPositions[i].x, h * fingerPositions[i].y);
        
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(200);
        ofEllipse(0, 0, 40, 40);
        
        //float s = 40 * ofNoise(ofGetFrameNum()*0.01, i);
        //float s = 40 * keys["/JT"+ofToString(i)];
        float s = 40 * keys[i];
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(255);
        ofEllipse(0, 0, s, s);
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}
