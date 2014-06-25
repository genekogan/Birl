#include "Birl.h"

//-------
void Birl::setup(int port) {
	osc.setup(port);
    
    keys.resize(9);
    keysDiscrete.resize(9);
    pressure.resize(2);
    embouchure.resize(40);
}

//-------
void Birl::update() {
    while(osc.hasWaitingMessages()){
        ofxOscMessage m;
        osc.getNextMessage(&m);
        if (m.getAddress() == "/birl/keys/") {
            for (int i=0; i<9; i++) {
                keys[i] = ofClamp(ofMap(m.getArgAsInt32(i), 0, MAX_KEY_VALUE, 0, 1), 0, 1);
                keysDiscrete[i] = keys[i] > 0.5 ? 1.0 : 0.0;
            }
        }
        else if (m.getAddress() == "/birl/breathpos/") {
            pressure[1] = m.getArgAsInt32(0) / maxbreathpos;
        }
        else if (m.getAddress() == "/birl/breathneg/") {
            pressure[0] = m.getArgAsInt32(0) / maxbreathneg;
        }
        else if (m.getAddress() == "/birl/maxbreathpos/") {
            maxbreathpos = m.getArgAsInt32(0);
        }
        else if (m.getAddress() == "/birl/maxbreathneg/") {
            maxbreathneg = m.getArgAsInt32(0);
        }
    }

//    for (int i=0; i<9; i++) {
//        keys[i] = ofNoise(i, ofGetFrameNum()*0.003*ofNoise(ofGetFrameNum()*0.003, i*5+5));
//    }
}

vector<float> Birl::getKeys() {
    return keys;
}

vector<float> Birl::getKeysDiscrete() {
    return keysDiscrete;
}

vector<float> Birl::getPressure() {
    return pressure;
}

vector<float> Birl::getEmbouchure() {
    return embouchure;
}

//-------
void Birl::draw(int x, int y, int w, int h) {
    ofPoint fingerPositions[9] = {
        ofPoint(0.6,  0.1 ),
        ofPoint(0.5,  0.19),
        ofPoint(0.6,  0.28),
        ofPoint(0.8,  0.37),
        ofPoint(0.5,  0.52),
        ofPoint(0.5,  0.61),
        ofPoint(0.4,  0.7 ),
        ofPoint(0.2,  0.79),
        ofPoint(0.15, 0.86)
    };
    
    ofPushMatrix();
    ofTranslate(x, y);
    
    // draw embouchure
    
    
    // draw breath pressure
    ofTranslate(0, 60);
    
//    float s1 = ofNoise(ofGetElapsedTimef()*0.1, 5);
//    float s2 = ofNoise(ofGetElapsedTimef()*0.1, 15);
    float s1 = pressure[0];
    float s2 = pressure[1];

    ofNoFill();
    ofSetColor(200, 60, 30);
    ofRect(0, 0, w/2, 38);
    ofRect(w/2, 0, w/2, 38);
    ofFill();
    ofSetColor(20, 20, 255);
    ofRect( 1+w/2, 0, s1 * 0.5 * w, 38);
    ofRect(-1+w/2, 0, s2 * -0.5 * w, 38);
    
    // draw keys
    ofTranslate(0, 40);
    
    ofFill();
    ofSetColor(200, 60, 30);
    ofRect(0, 0, w, h);
    ofNoFill();
    ofSetColor(20, 20, 255);
    ofRect(0, 0, w, h);
    
    for (int i=0; i<9; i++) {
        ofPushMatrix();
        ofTranslate(w * fingerPositions[i].x, (h-100) * fingerPositions[i].y);
        
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(255);
        ofEllipse(0, 0, 40, 40);
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(200);
        ofEllipse(0, 0, 40, 40);
        
        float s = 40 * keys[i];
        ofFill();
        ofSetLineWidth(0);
        ofSetColor(20, 20, 255);
        ofEllipse(0, 0, s, s);
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
}
