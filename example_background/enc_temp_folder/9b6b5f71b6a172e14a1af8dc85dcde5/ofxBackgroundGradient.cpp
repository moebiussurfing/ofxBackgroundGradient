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
    color1.set("Color Gradient 1", ofColor(127), ofColor(0, 0), ofColor(255));
    color2.set("Color Gradient 2", ofColor(127), ofColor(0, 0), ofColor(255));
    gradientType.set("type", 1, 0, 2);
    gradientType_str.set("TYPE:", "");
    scaleX.set("scaleX", 2.0f, 0.1f, 5.0f);
    scaleY.set("scaleY", 2.0f, 0.1f, 5.0f);

    bScaleLink.set("Link Scales", true);
    degrees.set("degrees", 0.0f, 0.0f, 360.0f);
    bRandomize.set("Randomize All", false);
    bRandomizeColors.set("Randomize Colors", false);
    bResetAll.set("Reset All", false);
    bResetTransform.set("Reset Transform", false);

    params.setName("GRADIENT BACKGROUND");
    params.add(color1);
    params.add(color2);
    params.add(gradientType);
    params.add(gradientType_str);
    gradientType_str.setSerializable(false);

    params_circleMode.setName("CIRCULAR TYPE");
    params_circleMode.add(pos);
    params_circleMode.add(scaleX);
    params_circleMode.add(scaleY);
    //params_circleMode.add(degrees);
    params.add(params_circleMode);

    params_controls.setName("CONTROLS");

    bScaleLink.setSerializable(false);
    bRandomize.setSerializable(false);
    bRandomizeColors.setSerializable(false);
    bRandomizeColors.setSerializable(false);
    bResetTransform.setSerializable(false);
    bResetAll.setSerializable(false);

    params_controls.add(bScaleLink);
    params_controls.add(bRandomize);
    params_controls.add(bRandomizeColors);
    params_controls.add(bResetTransform);
    params_controls.add(bResetAll);
    params.add(params_controls);

    gui.setup();
    gui.add(params);
    //gui.add(params_controls);

    ofAddListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
    ofAddListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

    if (autoSaveLoad)
        XML_load(params, path_folder + path_file);

	auto g0 = gui.getGroup("CONTROLS");
	g0.minimize();
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::update()
//{
//}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawBackground()
{
    //only in circular gradient mode
    if (gradientType == 1)
    {
        ofPushMatrix();
        ofTranslate(pos.get().x, pos.get().y);
        ofScale(scaleX, scaleY);
        ofBackground(color2);
    }

    ofBackgroundGradient(color1, color2, ofGradientMode(gradientType.get()));

    //only in circular gradient mode
    if (gradientType == 1)
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
void ofxBackgroundGradient::resetAll()
{
    color1.set(ofColor(127));
    color2.set(ofColor(48));
    gradientType = 0;
    resetTransform();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::resetTransform()
{
    pos = glm::vec2(0, 0);
    scaleX = scaleY = 1.0;
    degrees = 0;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_Params(ofAbstractParameter &e)
{
    string name = e.getName();

    if (name == "Randomize All")
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
    else if (name == "Reset All")
    {
        if (bResetAll)
        {
            bResetAll = false;
            resetAll();
        }
    }
    else if (name == "Reset Transform")
    {
        if (bResetTransform)
        {
            bResetTransform = false;
            resetTransform();
        }
    }
    else if (name == "Link Scales")
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
		refreshGui();
    }
}

void ofxBackgroundGradient::refreshGui() 
{
	auto g0 = gui.getGroup("GRADIANT BACKGROUND");
	auto g1 = g0.getGroup("CIRCULAR TYPE");
	if (gradientType == 1)
	g1.maximize();
	else
		g1.minimize();

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