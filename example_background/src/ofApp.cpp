#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    myBackground.setAutoSaveLoad(true);
    myBackground.setup();
}

//--------------------------------------------------------------
void ofApp::draw(){
    myBackground.draw();
}
