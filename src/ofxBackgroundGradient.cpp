#include "ofxBackgroundGradient.h"

//--------------------------------------------------------------
ofxBackgroundGradient::ofxBackgroundGradient()
{
	path_folder = "ofxBackgroundGradient/";
	path_file = "backgroundApp.xml";
}

//--------------------------------------------------------------
ofxBackgroundGradient::~ofxBackgroundGradient()
{
	ofRemoveListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
	ofRemoveListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

	exit();//TODO:
}

//--------------------------------------------------------------
void ofxBackgroundGradient::exit()
{
	if (autoSaveLoad) {
		positionGui = gui.getPosition();

		XML_save(params, path_folder + path_file);
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::setup()
{
	pos.set("OFFSET", glm::vec2(0, 0),
		glm::vec2(-ofGetWidth(), -ofGetHeight()),
		glm::vec2(ofGetWidth(), ofGetHeight()));
	color1.set("COLOR GRADIENT 1", ofColor(127), ofColor(0, 0), ofColor(255));
	color2.set("COLOR GRADIENT 2", ofColor(127), ofColor(0, 0), ofColor(255));
	gradientType.set("TYPE", 1, 0, 2);
	gradientType_str.set("", "");
	scaleX.set("SCALEX", 2.0f, 0.1f, 5.0f);
	scaleY.set("SCALEY", 2.0f, 0.1f, 5.0f);

	bScaleLink.set("LINK SCALES", true);
	degrees.set("DEGREES", 0.0f, 0.0f, 360.0f);
	bRandomize.set("RANDOMIZE ALL", false);
	bSwapColors.set("SWAP COLORS", false);
	bRandomizeColors.set("RANDOMIZE COLORS", false);
	bResetAll.set("RESET ALL", false);
	bResetTransform.set("RESET TRANSFORM", false);
	bEditorMode.set("EDITOR MODE", false);

	params.setName("GRADIENT BACKGROUND");
	params.add(color1);
	params.add(color2);
	params.add(bSwapColors);
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

	bSwapColors.setSerializable(false);
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
	params_controls.add(bEditorMode);
	params_controls.add(positionGui);
	params.add(params_controls);

	gui.setup();
	gui.add(params);
	//gui.add(params_controls);

	ofAddListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
	ofAddListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

	if (autoSaveLoad)
		XML_load(params, path_folder + path_file);

	//collapse
	auto &g0 = gui.getGroup("GRADIENT BACKGROUND");
	auto &g1 = g0.getGroup("CONTROLS");
	g1.minimize();
	g0.minimize();

	//gui.setPosition(210,10);
	gui.setPosition(positionGui.get().x, positionGui.get().y);
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::update()
//{
//}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawBackground()
{
	//only in circular gradient mode
	if (gradientType == 1 && !bEditorMode)
	{
		ofPushMatrix();
		ofTranslate(pos.get().x, pos.get().y);
		
		ofTranslate(0.5*ofGetWidth(), 0.5*ofGetHeight());
		ofScale(scaleX, scaleY);
		ofTranslate(-0.5*ofGetWidth(), -0.5*ofGetHeight());
		
		ofBackground(color2);
		//ofClear(color2);
	}

	//editor mode
	if (bEditorMode) {
		int _g = 40;
		ofClear(0);
		ofBackgroundGradient(ofColor(_g), ofColor(0, 0, 0), OF_GRADIENT_CIRCULAR);	
		
		//--

		bool bPlanes = true;//draw grid plane and testing camera too
		if (bPlanes) {
			cam.begin();

			//draw a grid on the floor
			ofPushStyle();
			ofPushMatrix();
			ofSetColor(ofColor(60));
			//ofTranslate(0, -400, 0);//change height
			ofRotateDeg(90, 0, 0, -1);
			ofDrawGridPlane(100, 10, false);//size of a side is 100x10 = 1000
			ofPopMatrix();
			ofPopStyle();

			cam.end();
		}
	}
	else {
		ofBackgroundGradient(color1, color2, ofGradientMode(gradientType.get()));
	}

	//only in circular gradient mode
	if (gradientType == 1 && !bEditorMode)
	{
		ofPopMatrix();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawGui()
{
	if (bShowGui)
	{
		gui.draw();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::draw()
{
	drawBackground();

	if (bShowGui)
	{
	drawGui();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::randomize()
{
	color1.set(randomColor());
	color2.set(randomColor());
	gradientType = (int)ofRandom(0.0f, 3.0f);
	scaleX = (int)ofRandom(0.5f, 6.0f);
	scaleY = (int)ofRandom(0.5f, 6.0f);
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
	color2.set(ofColor(24));
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

	if (name == "RANDOMIZE ALL")
	{
		if (bRandomize)
		{
			bRandomize = false;
			randomize();
		}
	}
	else if (name == "SWAP COLORS")
	{
		if (bSwapColors)
		{
			bSwapColors = false;
			ofColor _c;
			_c.set(color1.get());
			color1.set(color2.get());
			color2 = _c;
		}
	}
	else if (name == "RANDOMIZE COLORS")
	{
		if (bRandomizeColors)
		{
			bRandomizeColors = false;
			color1.set(randomColor());
			color2.set(randomColor());
		}
	}
	else if (name == "RESET ALL")
	{
		if (bResetAll)
		{
			bResetAll = false;
			resetAll();
		}
	}
	else if (name == "RESET TRANSFORM")
	{
		if (bResetTransform)
		{
			bResetTransform = false;
			resetTransform();
		}
	}
	else if (name == "LINK SCALES")
	{
		if (bScaleLink)
		{
			scaleY = scaleX;
		}
	}
	else if (name == "SCALEX")
	{
		if (bScaleLink)
		{
			scaleY = scaleX;
		}
	}
	else if (name == "SCALEY")
	{
		if (bScaleLink)
		{
			scaleX = scaleY;
		}
	}
	else if (name == "TYPE")
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

		typeChanged = true;
		refreshGui();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::refreshGuiPtr(ofxPanel &_gui)
{
	//collapse
	auto &g0 = _gui.getGroup("GRADIENT BACKGROUND");
	auto &g1 = g0.getGroup("CIRCULAR TYPE");
	g1.minimize();
	if (gradientType == 1)
		g1.maximize();
	else
		g1.minimize();
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::refreshGuiGrp(ofParameterGroup &_group)
//{
//	//collapse
//	//auto &g0 = _group.getGroup("GRADIENT BACKGROUND");
//	//auto &g1 = g0.getGroup("CIRCULAR TYPE");
//	auto &g1 = _group.getGroup("CIRCULAR TYPE");
//	g1.minimize();
//	if (gradientType == 1)
//		g1.maximize();
//	else
//		g1.minimize();
//}

//--------------------------------------------------------------
void ofxBackgroundGradient::refreshGui()
{
	//collapse
	auto &g0 = gui.getGroup("GRADIENT BACKGROUND");
	auto &g1 = g0.getGroup("CIRCULAR TYPE");
	g1.minimize();
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