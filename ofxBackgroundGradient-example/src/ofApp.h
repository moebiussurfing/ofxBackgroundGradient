#pragma once

#include "ofMain.h"

#include "ofxBackgroundGradient.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void draw();

    ofxBackgroundGradient myBackground;
};
