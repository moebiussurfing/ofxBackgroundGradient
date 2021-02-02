#pragma once
#include "ofMain.h"

//https://www.shadertoy.com/view/ltSyWz
//https://www.shadertoy.com/view/ttBBWt
//https://forum.openframeworks.cc/t/background-gradient/1304/2


//TODO:
//+ undo history
//+ refill when smaller than 1 scale that do not fill the screen

#include "ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

//#define USE_PRESETS

class ofxBackgroundGradient
{

private:
	//public:

	float w;
	float h;
	ofFbo fbo;

	ofParameterGroup params_Transform;
	ofParameter<bool> bTransform;
	ofParameter<bool> bRotate;
	ofParameter<float> angle;
	ofParameter<float> zoom;
	ofParameter<float> speed;

	ofColor greenFuxia;

	ofImage img;

public:
	enum typeGradienMode
	{
		ONE_COLOR = 0,
		LINEAR,
		CIRCULAR,
		BAR,
		IMAGE
	};

	//--


public:
	ofxBackgroundGradient();
	~ofxBackgroundGradient();

	void setup();
	void exit();

	void draw();//draw background and gui
	void drawBackground();//draw background
	void drawGui();//draw gui

private:
	void drawFloorGrid();
	void refresh();

private:
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mouseScrolled(ofMouseEventArgs &eventArgs);

	//presets browser
#ifdef USE_PRESETS
	void filesRefresh();
	vector <std::string> imgNamesForListBox;
	ofParameter<int> indexFilePreset;
	ofParameter<bool> bSavePreset;
	ofParameter<bool> bNewPreset;
	ofParameter<bool> bNextPreset;

	void loadPreset(int index);
	void loadNext();
#endif

	std::string _extension;

	//--

public:
	ofParameterGroup params_Settings;//preset itself

	ofParameterGroup params_Editor;
	ofParameterGroup params_AppSettings;
	ofParameterGroup params_Advanced;

private:
	ofParameter<ofColor> color1;
	ofParameter<ofColor> color2;

	//--

private:
	ofxPanel gui_AppControl;
	ofxPanel gui_Settings;

public:
	ofParameter<bool> SHOW_Gui{ "Gui BACKGROUND", false };//we use this toggle to easy add to external (ofApp) gui panel
	ofParameter<bool> bEditByMouse{ "MOUSE EDIT", false };

	//-

private:
	ofParameter<glm::vec2> positionGui{ "GUI POSITION", glm::vec2(400,10) , glm::vec2(0,0) , glm::vec2(1920,1080) };

private:
	void refreshGui();

	////TODO:
	////this method could handle the collapsing of the groups
	////but should use guiGroups...or to pass the gui pannel and the parent levels..
	//void refreshGuiPtr(ofxPanel &_gui);

	//----

	// API

public:
	void setPosition(glm::vec2 position)
	{
		positionGui = position;
		gui_AppControl.setPosition(positionGui.get().x, positionGui.get().y);
	}

	void toggleVisibleGui()
	{
		SHOW_Gui = !SHOW_Gui;
	}
	void setVisibleGui(bool b)
	{
		SHOW_Gui = b;
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
	void setSwapColors()
	{
		bSwapColors = true;
	}
	bool getSwapColors()
	{
		return bSwapColors;
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
	ofParameter<std::string> gradientType_str;
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

	std::string path_Global;
	std::string path_Presets;
	std::string path_file;
	std::string path_AppSettings;
	std::string path_Images;

	//void loadSettings(ofParameterGroup &g, std::string path);
	//void saveSettings(ofParameterGroup &g, std::string path);

	//editor cam
	//a personalized neurral gradient color and camera too to use on a 3d editor environment
	ofParameter<bool> bEditorMode;
	ofParameter<bool> bDrawFloorGrid;
	ofParameter<bool> bThemeGreenFloor;

	ofEasyCam cam;//testing purpose camera

	//TODO: 
	//this will require a custom gradient/mesh drawing method..
	//also to use translations on bar/linear modes besides circular
	ofParameter<float> degrees;
};


