#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::draw()
{
	//bg.draw(); // -> draws bg and gui

	bg.drawBackground(); // -> draws bg

	cam.begin();
	{
		bg.drawFloor(); // -> draws a 3d floor environment
	}
	cam.end();

	bg.drawGui(); // -> draws gui
}