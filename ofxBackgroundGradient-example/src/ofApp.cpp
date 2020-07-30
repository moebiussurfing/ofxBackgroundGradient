#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    myBackground.setAutoSaveLoad(true);
    myBackground.setup();
    myBackground.setVisibleGui(true);
}

//--------------------------------------------------------------
void ofApp::draw(){
    myBackground.draw();
}
