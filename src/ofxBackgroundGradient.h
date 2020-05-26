#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofxBackgroundGradient
{

public:
	ofxBackgroundGradient();
	~ofxBackgroundGradient();

	void setup();
	//void update();

	void drawBackground();
	void drawGui();
	void draw();//both

	//--

public:
	ofParameterGroup params;//the required paramaters to store preset/states
	ofParameterGroup params_controls;//we need to show the controls outside too
	ofParameter<ofColor> color1;
	ofParameter<ofColor> color2;

	//TODO:
	//this method could handle the collapsing of the groups
	//but should use guiGroups...or to pass the gui pannel and the parent levels..
	void refreshGuiPtr(ofxPanel &_gui);
	//void refreshGuiGrp(ofParameterGroup &_group);

	//--

private:
	ofxPanel gui;
	bool bShowGui = false;

private:
	void refreshGui();

	//--

	//api

public:
	void toggleVisibleGui() {
		bShowGui = !bShowGui;
	}
	void setVisibleGui(bool b) {
		bShowGui = b;
	}
	void setAutoSaveLoad(bool b)
	{
		autoSaveLoad = b;
		ofLogNotice("ofxBackgroundGradient") << "setAutoSaveLoad: " << b;
	}

public:
	void reset()
	{
		resetAll();
	}

	//to update gui outside addon class
	bool isTypeChanged()
	{
		if (typeChanged)
		{
			typeChanged = false;
			return true;
		}
		else
		{
			return false;
		}
	}
	int getType()
	{
		return gradientType.get();
	}

	//-

private:

	//to update gui outside addon class
	bool typeChanged = false;

	ofParameterGroup params_circleMode;

	ofParameter<glm::vec2> pos;
	ofParameter<float> scaleX;//for circle mode only
	ofParameter<float> scaleY;//for circle mode only
	ofParameter<int> gradientType;
	ofParameter<string> gradientType_str;
	ofParameter<bool> bRandomize;
	ofParameter<bool> bSwapColors;
	ofParameter<bool> bRandomizeColors;
	ofParameter<bool> bResetAll;
	ofParameter<bool> bResetTransform;
	ofParameter<bool> bScaleLink;
	void Changed_Params(ofAbstractParameter &e);

	ofColor randomColor();
	void randomize();
	void resetAll();
	void resetTransform();

	bool autoSaveLoad = false;
	string path_folder;
	string path_file;
	void XML_load(ofParameterGroup &g, string path);
	void XML_save(ofParameterGroup &g, string path);

	//editor cam
	//a personalized neurral gradient color and camera too to use on a 3d editor environment
	ofParameter<bool> bEditorMode;
	ofEasyCam cam;//testing purpose camera
	
	//TODO: 
	//this will require a custom gradient/mesh drawing method..
	//also to use translations on bar/linear modes besides circular
	ofParameter<float> degrees;
};


