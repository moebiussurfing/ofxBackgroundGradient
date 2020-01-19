#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofxBackgroundGradient
{
public:

    ofxBackgroundGradient();
    ~ofxBackgroundGradient();

    void setup();

    void drawBackground();
    void drawGui();
    void draw();//both

    //void update();
    //void exit();

    ofParameterGroup params;//the required paramaters to store preset/states
    ofParameterGroup params_controls;//we need to show the controls outside too
    ofParameter<ofColor> color1;
    ofParameter<ofColor> color2;

    ofxPanel gui;

    void setAutoSaveLoad(bool b)
    {
        autoSaveLoad = b;
        ofLogNotice("ofxBackgroundGradient") << "setAutoSaveLoad: " << b;
    }

	void refreshGui();
	
	//TODO:
	//this method could handle the collapsing of the groups
	//but should use guiGroups...or to pass the gui pannel and the parent levels..
	void refreshGuiPtr(ofxPanel &_gui);
	//void refreshGuiGrp(ofParameterGroup &_group);

	void reset() 
	{
		resetAll();
	}
    
	//-

private:

    ofParameterGroup params_circleMode;

    ofParameter<glm::vec2> pos;
    ofParameter<float> scaleX;//for circle mode only
    ofParameter<float> scaleY;//for circle mode only
    ofParameter<int> gradientType;
    ofParameter<string> gradientType_str;
    // TODO: this requires a custom gradient/mesh drawing method..
    ofParameter<float> degrees;
    ofParameter<bool> bRandomize;
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
    string path_folder = "ofxBackgroundGradient/";
    string path_file = "myBackground.xml";
    void XML_load(ofParameterGroup &g, string path);
    void XML_save(ofParameterGroup &g, string path);
};


