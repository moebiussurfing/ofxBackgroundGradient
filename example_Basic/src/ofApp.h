#pragma once

#include "ofMain.h"

#include "ofxBackgroundGradient.h"

class ofApp : public ofBaseApp {

public:
	void draw();
	void windowResized(int w, int h);

	ofxBackgroundGradient bg;
};
