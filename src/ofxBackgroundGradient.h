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

    ofParameterGroup params;
    ofxPanel gui;

    void setAutoSaveLoad(bool b)
    {
        autoSaveLoad = b;
        ofLogNotice("ofxBackgroundGradient") << "setAutoSaveLoad: " << b;
    }

    //-

private:

    ofParameterGroup params_controls;

    ofParameter<ofColor> color1;
    ofParameter<ofColor> color2;
    ofParameter<glm::vec2> pos;
    ofParameter<float> scaleX;//for circle mode only
    ofParameter<float> scaleY;//for circle mode only
    ofParameter<int> gradientType;
    ofParameter<string> gradientType_str;
    // TODO: this requires a custom gradient/mesh drawing method..
    ofParameter<float> degrees;
    ofParameter<bool> bRandomize;
    ofParameter<bool> bRandomizeColors;
    ofParameter<bool> bReset;
    ofParameter<bool> bScaleLink;
    void Changed_Params(ofAbstractParameter &e);

    ofColor randomColor();
    void randomize();
    void reset();

    bool autoSaveLoad = false;
    string path_folder = "ofxBackgroundGradient/";
    string path_file = "myBackground.xml";
    void XML_load(ofParameterGroup &g, string path);
    void XML_save(ofParameterGroup &g, string path);
};


