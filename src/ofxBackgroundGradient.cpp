#include "ofxBackgroundGradient.h"

//--------------------------------------------------------------
ofxBackgroundGradient::ofxBackgroundGradient()
{
}

//--------------------------------------------------------------
ofxBackgroundGradient::~ofxBackgroundGradient()
{
    ofRemoveListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
    ofRemoveListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

    if (autoSaveLoad)
        XML_save(params, path_folder + path_file);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::setup()
{
    pos.set("offset", glm::vec2(0, 0),
        glm::vec2(-ofGetWidth(), -ofGetHeight()),
        glm::vec2(ofGetWidth(), ofGetHeight()));
    color1.set("color1", ofColor(127), ofColor(0, 0), ofColor(255));
    color2.set("color2", ofColor(127), ofColor(0, 0), ofColor(255));
    gradientType.set("type", 1, 0, 2);
    gradientType_str.set("", "");
    scaleX.set("scaleX", 2.0f, 0.1f, 5.0f);
    scaleY.set("scaleY", 2.0f, 0.1f, 5.0f);

    bScaleLink.set("Link", true);
    degrees.set("degrees", 0.0f, 0.0f, 360.0f);
    bRandomize.set("Randomize", false);
    bRandomizeColors.set("Randomize Colors", false);
    bReset.set("Reset", false);

    params.setName("ofxBackgroundGradient");
    params.add(pos);
    params.add(color1);
    params.add(color2);
    params.add(gradientType);
    params.add(scaleX);
    params.add(scaleY);
    //params.add(degrees);

    params_controls.setName("CONTROLS");
    params_controls.add(gradientType_str);
    params_controls.add(bScaleLink);
    params_controls.add(bRandomize);
    params_controls.add(bRandomizeColors);
    params_controls.add(bReset);

    gui.setup();
    gui.add(params);
    gui.add(params_controls);

    ofAddListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
    ofAddListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

    if (autoSaveLoad)
        XML_load(params, path_folder + path_file);
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::update()
//{
//}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawBackground()
{
    //only in circular gradient mode
    //if (gradientType == 1)
    {
        ofPushMatrix();
        ofTranslate(pos.get().x, pos.get().y);
        ofScale(scaleX, scaleY);
        ofBackground(color2);
    }

    ofBackgroundGradient(color1, color2, ofGradientMode(gradientType.get()));

    //only in circular gradient mode
    //if (gradientType == 1)
    {
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawGui()
{
    gui.draw();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::draw()
{
    drawBackground();
    drawGui();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::randomize()
{
    color1.set(randomColor());
    color2.set(randomColor());
    gradientType = (int) ofRandom(0.0f, 3.0f);
    scaleX = (int) ofRandom(0.5f, 6.0f);
    scaleY = (int) ofRandom(0.5f, 6.0f);
}

//------------------------------------------------------------
ofColor ofxBackgroundGradient::randomColor()
{
    ofColor myColor;
    myColor.set(ofRandom(255), ofRandom(255), ofRandom(255));
    return myColor;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::reset()
{
    pos = glm::vec2(0, 0);
    color1.set(ofColor(127));
    color2.set(ofColor(48));
    gradientType = 0;
    scaleX = scaleY = 1.0;
    degrees = 0;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_Params(ofAbstractParameter &e)
{
    string name = e.getName();

    if (name == "Randomize")
    {
        if (bRandomize)
        {
            bRandomize = false;
            randomize();
        }
    }
    else if (name == "Randomize Colors")
    {
        if (bRandomizeColors)
        {
            bRandomizeColors = false;
            color1.set(randomColor());
            color2.set(randomColor());
        }
    }
    else if (name == "Reset")
    {
        if (bReset)
        {
            bReset = false;
            reset();
        }
    }
    else if (name == "Link")
    {
        if (bScaleLink)
        {
            scaleY = scaleX;
        }
    }
    else if (name == "scaleX")
    {
        if (bScaleLink)
        {
            scaleY = scaleX;
        }
    }
    else if (name == "scaleY")
    {
        if (bScaleLink)
        {
            scaleX = scaleY;
        }
    }
    else if (name == "type")
    {
        switch (gradientType)
        {
            case 0:
                gradientType_str = "LINEAR";
                break;
            case 1:
                gradientType_str = "CIRCULAR";
                break;
            case 2:
                gradientType_str = "BAR";
                break;
        }
    }
}

//--------------------------------------------------------------
void ofxBackgroundGradient::XML_load(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxBackgroundGradient") << "XML_load " << path;
    ofXml settings;

    bool b = settings.load(path);
    if (b)
        ofDeserialize(settings, g);
    else
        ofLogError("ofxBackgroundGradient") << "FILE NOT FOUND!";
}

//--------------------------------------------------------------
void ofxBackgroundGradient::XML_save(ofParameterGroup &g, string path)
{
    ofLogNotice("ofxBackgroundGradient") << "XML_save " << path;

    ofXml settings;
    ofSerialize(settings, g);
    settings.save(path);
}