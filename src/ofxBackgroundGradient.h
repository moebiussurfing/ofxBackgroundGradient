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

//--

// Optional
//#define USE_PRESETS

//--

#include "ofxGui.h"
#include "ofxSurfingHelpers.h"
#include "ofxSurfing_ofxGui.h"

#define NUM_TYPES 6

class ofxBackgroundGradient {

public:
	ofxBackgroundGradient();
	~ofxBackgroundGradient();

private:
	void setup();
	void setupParams();
	void startup();
	void exit();

public:
	//void draw(); //draw background and gui
	void drawBackground(); //draw background
	void drawFloor();
	
	void drawGui(); //draw gui

	//--

private:
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
	ofParameter<bool> bDrawBg { "Draw Bg", true };

public:
	enum typeGradienMode {
		ONE_COLOR = 0,
		LINEAR,
		CIRCULAR,
		BAR,
		IMAGE
	};

private:
	void refresh_Draw();

private:
	void update(ofEventArgs & args);
	void windowResized(ofResizeEventArgs & args);
	void keyPressed(ofKeyEventArgs & eventArgs);
	void keyReleased(ofKeyEventArgs & eventArgs);
	void mouseDragged(ofMouseEventArgs & eventArgs);
	void mouseScrolled(ofMouseEventArgs & eventArgs);

	// presets browser
#ifdef USE_PRESETS
private:
	void filesRefresh();
	vector<std::string> imgNamesForListBox;
	ofParameter<int> indexFilePreset;
	ofParameter<bool> bSavePreset;
	ofParameter<bool> bNewPreset;
	ofParameter<bool> bNextPreset;

	void loadPreset(int index);
	void loadNext();
	ofParameterGroup params_PresetManager;
#endif

	std::string _extension;

	std::string helpInfo;
	ofTrueTypeFont font;

	bool bKeyMod = false;

	//--

public:
	ofParameterGroup params_Preset; // settings for preset itself
	ofParameterGroup params_Helpers;

	ofParameterGroup params_Gradient;
	ofParameterGroup params_Editor;
	ofParameterGroup params_Internal;

public:
	ofParameter<ofColor> color1;
	ofParameter<ofColor> color2;

	//--

private:
	ofxPanel gui_Helpers;
	ofxPanel gui_Preset;

private:
	ofParameter<bool> bEditByMouse;

	//-

private:
	void doRefreshGui();

	//----

	// API

public:
	void setGuiPosition(glm::vec2 position) {
		gui_Preset.setPosition(position.x, position.y);
	}
	auto getGuiShape() {
		if (!bGui_Helpers && bGui) return gui_Preset.getShape();
	}
	void toggleVisibleGui() {
		bGui = !bGui;
	}
	void setVisibleGui(bool b) {
		bGui = b;
	}
	void setAutoSaveLoad(bool b) {
		bAutoSaveLoad = b;
		ofLogNotice(__FUNCTION__) << "setAutoSaveLoad: " << b;
	}

public:
	void doReset() {
		doResetAll();
	}

	//to update gui outside addon class
	bool isTypeChanged() {
		if (bTypeChanged) {
			bTypeChanged = false;
			return true;
		} else {
			return false;
		}
	}
	int getType() {
		return gradientType.get();
	}

	//-

private:
	// to update gui outside addon class
	bool bTypeChanged = false;

	ofParameterGroup params_circleMode;

	ofParameter<glm::vec2> posOffset;
	ofParameter<float> scaleX; //for circle mode only
	ofParameter<float> scaleY; //for circle mode only
	ofParameter<int> gradientType;
	ofParameter<std::string> gradientType_str;

	ofParameter<void> bRandomizeAll;
	ofParameter<void> bRandomizeAllExceptType;
	ofParameter<void> bFlipColors;
	ofParameter<void> bRandomizeColors;
	ofParameter<void> bResetAll;
	ofParameter<void> bResetAllExceptColors;
	ofParameter<void> bResetTransform;
	ofParameter<void> bResetOffset;
	ofParameter<bool> bScaleLink;

public:
	ofParameter<bool> bGui_Helpers;
	ofParameter<bool> bGui;
	ofParameter<bool> bGui_Help;
	ofParameter<bool> bKeys;

private:
	void Changed_Preset(ofAbstractParameter & e);
	void Changed_AppSettings(ofAbstractParameter & e);
	bool bDisableCallbacks = false;

	ofColor doRandomColor();
	void doRandomizeAll();
	void doRandomizeAllExceptType();
	void doResetAll();
	void doResetAllExceptColors();
	void doResetTransform();
	void doResetOffset();

	bool bAutoSaveLoad = true;

	std::string path_Global;

#ifdef USE_PRESETS
	std::string path_file;
	std::string path_Presets;
	std::string path_PresetsManager;
#endif

	std::string path_Internal;
	std::string path_PresetSettings;
	std::string path_Images;

public:
	ofParameter<bool> bDrawFloorGrid;
	ofParameter<bool> bThemeGreenFloor;

public:
	ofParameter<bool> bDrawFloor { "Floor", false };

private:
	//ofEasyCam cam; //testing purpose camera

	void setType(int t) {
		switch (t) {

		case 0: {
			gradientType_str = ofToString("ONE COLOR");
		} break;

		case 1: {
			gradientType_str = ofToString("LINEAR");
		} break;

		case 2: {
			gradientType_str = ofToString("CIRCULAR");
		} break;

		case 3: {
			gradientType_str = ofToString("BAR");
		} break;

		case 4: {
			gradientType_str = ofToString("IMAGE");
		} break;

		case 5: {
			gradientType_str = ofToString("3D EDITOR");
		} break;
		}
	}
};
