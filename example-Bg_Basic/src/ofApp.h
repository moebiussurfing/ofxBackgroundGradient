#pragma once
#include "ofMain.h"

#include "ofxBackgroundGradient.h"

class ofApp : public ofBaseApp {
public:
	void draw();
	ofEasyCam cam;

	ofxBackgroundGradient bg;
};
