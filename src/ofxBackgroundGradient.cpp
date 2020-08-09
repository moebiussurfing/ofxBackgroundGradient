#include "ofxBackgroundGradient.h"

//--------------------------------------------------------------
ofxBackgroundGradient::ofxBackgroundGradient()
{
	path_folder = "ofxBackgroundGradient/";
	path_file = "backgroundApp_";
	path_ControlSettings = "ControlSettings.xml";
}

//--------------------------------------------------------------
ofxBackgroundGradient::~ofxBackgroundGradient()
{
	ofRemoveListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
	ofRemoveListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

	ofRemoveListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);

	exit();//TODO:
}

//--------------------------------------------------------------
void ofxBackgroundGradient::exit()
{
	if (autoSaveLoad) {
		ofxSurfingHelpers::CheckFolder(path_folder + "presets/");
		//if (indexFilePreset == -1) saveSettings(params, path_folder + "presets/" + path_file + ofToString(0));
		saveSettings(params, path_folder + "presets/" + path_file + ofToString(indexFilePreset) + ".xml");
	}
	positionGui = gui.getPosition();
	ofxSurfingHelpers::CheckFolder(path_folder);
	saveSettings(params_controls, path_folder + path_ControlSettings);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::setup()
{
	pos.set("OFFSET", glm::vec2(0, 0),
		glm::vec2(-ofGetWidth(), -ofGetHeight()),
		glm::vec2(ofGetWidth(), ofGetHeight()));
	color1.set("COLOR GRADIENT 1", ofColor(127), ofColor(0, 0), ofColor(255));
	color2.set("COLOR GRADIENT 2", ofColor(32), ofColor(0, 0), ofColor(255));
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

	bNewPreset.set("NEW PRESET", false);
	bNextPreset.set("NEXT PRESET", false);
	bSavePreset.set("SAVE PRESET", false);
	indexFilePreset.set("PRESET", -1, 0, 1);

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
	params.add(bEditorMode);

	params_controls.setName("CONTROLS");

	bSwapColors.setSerializable(false);
	bEditByMouse.setSerializable(false);
	bScaleLink.setSerializable(false);
	bRandomize.setSerializable(false);
	bRandomizeColors.setSerializable(false);
	bRandomizeColors.setSerializable(false);
	bResetTransform.setSerializable(false);
	bResetAll.setSerializable(false);
	bSavePreset.setSerializable(false);
	bNextPreset.setSerializable(false);
	bNewPreset.setSerializable(false);

	//browse
	params_controls.add(bNewPreset);
	params_controls.add(bSavePreset);
	params_controls.add(bNextPreset);
	params_controls.add(indexFilePreset);

	params_controls.add(bScaleLink);
	params_controls.add(bEditByMouse);
	params_controls.add(bRandomize);
	params_controls.add(bRandomizeColors);
	params_controls.add(bResetTransform);
	params_controls.add(bResetAll);
	//params_controls.add(bEditorMode);
	params_controls.add(positionGui);
	//params.add(params_controls);

	gui.setup("ofxBackgroundGradient");
	gui.add(params_controls);
	gui.add(params);

	ofAddListener(params.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
	ofAddListener(params_controls.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

	filesRefresh();
	if (autoSaveLoad) {
		loadPreset(indexFilePreset);
	}

	ofxSurfingHelpers::CheckFolder(path_folder);
	loadSettings(params_controls, path_folder + path_ControlSettings);

	//collapse
	auto &g0 = gui.getGroup("GRADIENT BACKGROUND");
	auto &g1 = gui.getGroup("CONTROLS");
	g1.minimize();
	g0.minimize();
	g1.getGroup("GUI POSITION").minimize();

	gui.setPosition(positionGui.get().x, positionGui.get().y);

	ofAddListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mouseDragged " << x << ", " << y << ", " << button;

	if (bEditByMouse) {
		pos = glm::vec2(x - 0.5 * ofGetWidth(), y - 0.5 * ofGetHeight());
	}
}
//--------------------------------------------------------------
void ofxBackgroundGradient::mouseScrolled(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const float &scrollX = eventArgs.scrollX;
	const float &scrollY = eventArgs.scrollY;
	ofLogNotice(__FUNCTION__) << "scrollX: " << scrollX << "  scrollY: " << scrollY;

	if (bEditByMouse) {
		if (scrollY == 1) scaleY += 0.1;
		else if (scrollY == -1) scaleY -= 0.1;
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::filesRefresh()
{
	ofxSurfingHelpers::CheckFolder(path_folder + "presets/");

	ofDirectory loadDir;
	loadDir.open(path_folder + "presets/");
	loadDir.allowExt("xml");

	ofLogNotice(__FUNCTION__) << "Presets files:";
	imgNamesForListBox.clear();
	for (auto im : loadDir) {
		imgNamesForListBox.emplace_back(im.getFileName());
		ofLogNotice(__FUNCTION__) << im.getFileName();
	}

	indexFilePreset.setMax(imgNamesForListBox.size() - 1);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawGrid() {
	//draw a grid on the floor

#define MAX_MAGNITUDE 1000

	//ofPushMatrix();
	//ofPushStyle();
	//ofTranslate(0, -250, 0);
	//ofRotate(90, 0, 0, -1);
	//ofSetColor(96, 128);
	//ofDrawGridPlane(500, 1, false);
	//ofSetColor(48, 128);
	//ofDrawGridPlane(250, 2, false);
	//ofPopStyle();
	//ofPopMatrix();


	ofPushMatrix();
	ofPushStyle();
	ofTranslate(0, -250, 0);
	ofRotate(90, 0, 0, -1);
	ofSetColor(ofColor(90));
	ofDrawGridPlane(0.4*MAX_MAGNITUDE, 1, false);
	ofSetColor(ofColor(50));
	ofDrawGridPlane(0.1*MAX_MAGNITUDE, 4, false);
	ofPopStyle();
	ofPopMatrix();
}

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
		int _g1 = 40;
		int _g2 = 5;
		ofClear(0);
		ofBackgroundGradient(ofColor(_g1), ofColor(_g2), OF_GRADIENT_CIRCULAR);

		//--

		bool bPlanes = true;//draw grid plane and testing camera too
		if (bPlanes) {
			cam.begin();

			drawGrid();

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
	else if (name == "RESET ALL")
	{
		if (bResetAll)
		{
			bResetAll = false;
			resetAll();
		}
	}

	//browse
	else if (name == bSavePreset.getName())
	{
		if (bSavePreset)
		{
			bSavePreset = false;

			//saveSettings(params, path_folder + path_file + ofToString(imgNamesForListBox.size() - 1));
			filesRefresh();
			if (indexFilePreset == -1) indexFilePreset = 0;
			saveSettings(params, path_folder + "presets/" + path_file + ofToString(indexFilePreset) + ".xml");
		}
	}
	else if (name == bNextPreset.getName())
	{
		if (bNextPreset)
		{
			bNextPreset = false;

			if (imgNamesForListBox.size() == 0) {//no files in folder
				indexFilePreset = -1;
			}
			else {
				if (autoSaveLoad) {
					saveSettings(params, path_folder + "presets/" + path_file + ofToString(indexFilePreset) + ".xml");
				}

				if (indexFilePreset < imgNamesForListBox.size() - 1) indexFilePreset++;
				else indexFilePreset = 0;
			}
		}
	}
	else if (name == bNewPreset.getName())
	{
		if (bNewPreset)
		{
			bNewPreset = false;

			if (indexFilePreset < imgNamesForListBox.size())//go to last
				indexFilePreset = imgNamesForListBox.size();
		}
	}
	else if (name == indexFilePreset.getName())
	{
		filesRefresh();

		if (indexFilePreset < imgNamesForListBox.size()) {
			loadPreset(indexFilePreset);
		}
	}

	//--

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
void ofxBackgroundGradient::loadSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << path;
	ofXml settings;

	bool b = settings.load(path);
	if (b)
		ofDeserialize(settings, g);
	else
		ofLogError(__FUNCTION__) << "FILE NOT FOUND!";
}

//--------------------------------------------------------------
void ofxBackgroundGradient::saveSettings(ofParameterGroup &g, string path)
{
	ofLogNotice(__FUNCTION__) << path;

	ofXml settings;
	ofSerialize(settings, g);
	settings.save(path);
}