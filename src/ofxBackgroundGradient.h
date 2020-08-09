#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSurfingHelpers.h"

class ofxBackgroundGradient
{

public:
	ofxBackgroundGradient();
	~ofxBackgroundGradient();

	void setup();
	//void update();

	void drawBackground();
	void drawGrid();
	void drawGui();
	void draw();////draw background and gui
	void exit();//TODO: should be used out of destructor if getting crashes..

private:
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mouseScrolled(ofMouseEventArgs &eventArgs);

	//presets browser
	void filesRefresh();
	vector <string> imgNamesForListBox;
	ofParameter<int> indexFilePreset;
	ofParameter<bool> bSavePreset;
	ofParameter<bool> bNewPreset;
	ofParameter<bool> bNextPreset;
	void loadPreset(int index) {
		if (index < imgNamesForListBox.size()) {
			loadSettings(params, path_folder + "presets/" + imgNamesForListBox[index]);
		}
		else
		{
			ofLogError(__FUNCTION__) << "Presets index file out of range!";
		}

	}
	void loadNext() {
		bNextPreset = true;
	}

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

public:
	ofParameter<bool> bShowGui{ "BACKGROUND", false };//we use this toggle to easy add to external (ofApp) gui panel
	ofParameter<bool> bEditByMouse{ "MOUSE EDIT", false };

	void setPosition(glm::vec2 position) {
		positionGui = position;
		gui.setPosition(positionGui.get().x, positionGui.get().y);
	}

private:
	void refreshGui();

	ofParameter<glm::vec2> positionGui{ "GUI POSITION", glm::vec2(400,10) , glm::vec2(0,0) , glm::vec2(1920,1080) };

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
		ofLogNotice(__FUNCTION__) << "setAutoSaveLoad: " << b;
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

	bool autoSaveLoad = true;
	string path_folder;
	string path_file;
	string path_ControlSettings;
	void loadSettings(ofParameterGroup &g, string path);
	void saveSettings(ofParameterGroup &g, string path);

	//editor cam
	//a personalized neurral gradient color and camera too to use on a 3d editor environment
	ofParameter<bool> bEditorMode;
	ofEasyCam cam;//testing purpose camera

	//TODO: 
	//this will require a custom gradient/mesh drawing method..
	//also to use translations on bar/linear modes besides circular
	ofParameter<float> degrees;
};


