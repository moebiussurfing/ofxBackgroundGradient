#pragma once

#include "ofMain.h"

// some useful links:
//https://github.com/rystylee/ofxCosineGradient
//https://www.shadertoy.com/view/ltSyWz
//https://www.shadertoy.com/view/ttBBWt
//https://forum.openframeworks.cc/t/background-gradient/1304/2

//TODO:
// + Refill when smaller than 1 scale that do not fill the screen
// + Fix bar transformation margins to fit window

//-

#include "ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

//#define USE_PRESETS

#define NUM_TYPES 6

class ofxBackgroundGradient
//class ofxBackgroundGradient : public ofBaseApp
{

private:
	//public:

	float w;
	float h;
	ofFbo fbo;

	ofParameterGroup params_Transform;
	ofParameter<bool> bTransform;
	ofParameter<bool> bRotateAuto;
	ofParameter<float> angle;
	ofParameter<float> zoom;
	ofParameter<float> speed;

	ofColor greenFuxia;

	ofImage img;

public:
	ofParameter<bool>bDrawBg{ "Draw Bg", true };

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

public:
	void drawFloor();

private:
	void refresh_Draw();

private:
	void update(ofEventArgs & args);
	void windowResized(ofResizeEventArgs & args);
	//void windowResized(int w, int h);
	void keyPressed(ofKeyEventArgs &eventArgs);
	void keyReleased(ofKeyEventArgs &eventArgs);
	void mouseDragged(ofMouseEventArgs &eventArgs);
	void mouseScrolled(ofMouseEventArgs &eventArgs);

private:
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

	std::string helpInfo;
	ofTrueTypeFont font;

	bool bKeyMod = false;

	//--

public:
	ofParameterGroup params_Preset;//preset itself
	ofParameterGroup params_Gradient;

	ofParameterGroup params_Editor;
	ofParameterGroup params_AppSettings;
	ofParameterGroup params_Advanced;

	ofParameterGroup params_Internal;

//private:
public:
	ofParameter<ofColor> color1;
	ofParameter<ofColor> color2;

	//--

private:
	ofxPanel gui_Advanced;
	ofxPanel gui_Preset;

//public:
private:
	ofParameter<bool> bEditByMouse;

	//-

private:
	ofParameter<glm::vec2> positionGuiPreset{ "GUI POSITION 1", glm::vec2(600,10) , glm::vec2(0,0) , glm::vec2(1920,1080) };
	ofParameter<glm::vec2> positionGuiAdvanced{ "GUI POSITION 2", glm::vec2(400,10) , glm::vec2(0,0) , glm::vec2(1920,1080) };

private:
	void refresh_Gui();

	////TODO:
	////this method could handle the collapsing of the groups
	////but should use guiGroups...or to pass the gui pannel and the parent levels..
	//void refreshGuiPtr(ofxPanel &_gui);

	//----

	// API

public:
	void setGuiPosition(glm::vec2 position)
	{
		positionGuiPreset = position;
		gui_Preset.setPosition(positionGuiPreset.get().x, positionGuiPreset.get().y);
	}
	auto getGuiShape() { 
		if (!bGui_Advanced && bGui) return gui_Preset.getShape();
	}
	void toggleVisibleGui()
	{
		bGui = !bGui;
	}
	void setVisibleGui(bool b)
	{
		bGui = b;
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
		bFlipColors = true;
	}
	bool getSwapColors()
	{
		return bFlipColors;
	}

	//-

private:

	//to update gui outside addon class
	bool typeChanged = false;

	ofParameterGroup params_circleMode;

	ofParameter<glm::vec2> posOffset;
	ofParameter<float> scaleX;//for circle mode only
	ofParameter<float> scaleY;//for circle mode only
	ofParameter<int> gradientType;
	ofParameter<std::string> gradientType_str;
	ofParameter<bool> bRandomizeAll;
	ofParameter<bool> bFlipColors;
	ofParameter<bool> bRandomizeColors;
	ofParameter<bool> bResetAll;
	ofParameter<bool> bResetTransform;
	ofParameter<bool> bResetOffset;
	ofParameter<bool> bScaleLink;

public:
	ofParameter<bool> bGui_Advanced;
	//we use this toggle to easy add to external (ofApp) gui panel
	ofParameter<bool> bGui;
	ofParameter<bool> bGui_Help;
	ofParameter<bool> bKeys;

private:
	void Changed_Params_Preset(ofAbstractParameter &e);
	void Changed_Params_AppSettings(ofAbstractParameter &e);
	bool bDisableCallbacks = false;

	ofColor randomColor();
	void randomizeAll();
	void resetAll();
	void resetTransform();
	void resetOffset();

	bool autoSaveLoad = true;

	std::string path_Global;
	std::string path_Presets;
	std::string path_file;
	std::string path_AppSettings;
	std::string path_Internal;
	std::string path_PresetSettings;
	std::string path_Images;

public:
	//editor cam
	//a personalized neurral gradient color and camera too to use on a 3d editor environment
	//ofParameter<bool> bEditorMode;
	ofParameter<bool> bDrawFloorGrid;
	ofParameter<bool> bThemeGreenFloor;
	
public:
	ofParameter<bool> bDrawFloor{ "Floor", false };

	private:
	ofEasyCam cam;//testing purpose camera

	//TODO: 
	//this will require a custom gradient/mesh drawing method..
	//also to use translations on bar/linear modes besides circular
	//ofParameter<float> degrees;

	void setType(int t)
	{
		switch (t)
		{

		case 0:
		{
			//gradientType_str.setWithoutEventNotifications("ONE COLOR");
			gradientType_str = ofToString("ONE COLOR");
		}
		break;

		case 1:
		{
			//gradientType_str.setWithoutEventNotifications("LINEAR");
			gradientType_str = ofToString("LINEAR");
		}
		break;

		case 2:
		{
			//gradientType_str.setWithoutEventNotifications("CIRCULAR");
			gradientType_str = ofToString("CIRCULAR");
		}
		break;

		case 3:
		{
			//gradientType_str.setWithoutEventNotifications("BAR");
			gradientType_str = ofToString("BAR");
		}
		break;

		case 4:
		{
			//gradientType_str.setWithoutEventNotifications("IMAGE");
			gradientType_str = ofToString("IMAGE");
		}
		break;

		case 5:
		{
			//gradientType_str.setWithoutEventNotifications("IMAGE");
			gradientType_str = ofToString("3D EDITOR");
		}
		break;

		}

		//if (t == 5) bEditorMode = true;
		//else bEditorMode = false;
	}
};


