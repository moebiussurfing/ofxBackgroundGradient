#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::draw()
{
	bg.drawBackground(); // draws bg

	cam.begin();
	{
		bg.drawFloor(); // draws a 3d floor environment
	}
	cam.end();

	//--

	bg.drawGui(); // draws gui
}