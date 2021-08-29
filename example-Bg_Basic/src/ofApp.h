#pragma once

#include "ofMain.h"

#include "ofxBackgroundGradient.h"

class ofApp : public ofBaseApp {
	
	ofEasyCam cam;

public:
	void draw();

	ofxBackgroundGradient bg;
};
