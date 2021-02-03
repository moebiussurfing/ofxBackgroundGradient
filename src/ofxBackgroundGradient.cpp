#include "ofxBackgroundGradient.h"

//--------------------------------------------------------------
ofxBackgroundGradient::ofxBackgroundGradient()
{
#ifdef USE_JSON
	_extension = ".json";
#else
#ifdef USE_XML
	_extension = ".xml";
#endif
#endif

	path_Global = "ofxBackgroundGradient/";
	path_Images = path_Global + "images/";
	path_file = "ofxBackgroundGradient_";
	path_AppSettings = "AppSettings";
	path_AppSettings += _extension;

	path_SettingsXml = path_Global + "layout.xml";

#ifdef USE_PRESETS
	path_Presets = path_Global + "presets/";
#endif

#ifndef USE_PRESETS
	path_PresetSettings = path_Global + "ofxBackgroundGradient_Preset" + _extension;
#endif

	//-

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Images);
	ofxSurfingHelpers::CheckFolder(path_Presets);

	ofAddListener(ofEvents().windowResized, this, &ofxBackgroundGradient::windowResized);
	ofAddListener(ofEvents().update, this, &ofxBackgroundGradient::update);
	ofAddListener(ofEvents().keyPressed, this, &ofxBackgroundGradient::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxBackgroundGradient::keyReleased);

	//-

	helpInfo = "HELP\n";
	helpInfo += "H              HELP\n";
	helpInfo += "G              GUI\n";
	helpInfo += "+|-            TYPE\n";
	helpInfo += "TAB            SWAP COLORS\n";
	helpInfo += "MOUSE WHEEL    ZOOM TRANSFORM\n";
	helpInfo += "+Ctrl          ZOOM OFFSET\n";
	
	//-

	greenFuxia.setHsb(255.0f / 3.0f, 200.0f, 255.0f, 255.0f);

	setup();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::setup()
{
	img.load(path_Images + "img4.png");

	font.load("assets/fonts/telegrama_render.otf", 10, true, true, true);

	//--

	// fbo

	w = ofGetWidth();
	h = ofGetHeight();

	ofFboSettings s;
	s.width = w;
	s.height = h;
	s.internalformat = GL_RGB;
	//s.numColorbuffers = 1;
	//s.useDepth = false;
	//s.useStencil = false;
	//s.depthStencilAsTexture = false;
	//s.textureTarget = GL_TEXTURE_2D;
	//s.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
	//s.wrapModeVertical = GL_CLAMP_TO_EDGE;
	//s.minFilter = GL_LINEAR;
	//s.maxFilter = GL_LINEAR;
	//s.numSamples = 0;
	fbo.allocate(s);

	fbo.begin();
	ofClear(0, 0);
	fbo.end();

	//--

	// params
	SHOW_Gui.set("Gui BACKGROUND", false);//we use this toggle to easy add to external (ofApp) gui panel
	bEditByMouse.set("Mouse Edit", false);
	SHOW_Gui_Advanced.set("Gui Advanced", false);
	SHOW_Help.set("HELP", false);

	bTransform.set("Transform", false);
	bRotateAuto.set("Auto Rotate", false);
	angle.set("Angle", 0.0f, -180.1f, 180.0f);
	speed.set("Speed", 1.f, 0.01f, 5.0f);
	zoom.set("Zoom", 1.0f, 0.5f, 3.0f);

	posOffset.set("Offset", glm::vec2(0, 0),
		glm::vec2(-ofGetWidth(), -ofGetHeight()),
		glm::vec2(ofGetWidth(), ofGetHeight()));
	color1.set("Color 1", ofColor(200), ofColor(0), ofColor(255));
	color2.set("Color 2", ofColor(32), ofColor(0), ofColor(255));
	gradientType.set("Type", 0, 0, NUM_TYPES - 1);
	gradientType_str.set(" ", "");
	scaleX.set("Scale x", 2.0f, 0.1f, 5.0f);
	scaleY.set("Scale y", 2.0f, 0.1f, 5.0f);

	bScaleLink.set("Link Scales", true);
	//degrees.set("Degrees", 0.0f, 0.0f, 360.0f);
	bRandomize.set("Randomize All", false);
	bSwapColors.set("Swap Colors", false);
	bRandomizeColors.set("Randomize Colors", false);
	bResetAll.set("Reset All", false);
	bResetTransform.set("Reset Transform", false);
	bResetOffset.set("Reset Offset", false);
	//bEditorMode.set("Editor Mode", false);
	bDrawFloorGrid.set("Floor Grid", false);
	bThemeGreenFloor.set("Green Floor", false);

#ifdef USE_PRESETS
	bNewPreset.set("New Preset", false);
	bNextPreset.set("Next Preset", false);
	bSavePreset.set("Save Preset", false);
	indexFilePreset.set("Preset", -1, 0, 1);
#endif

	//-

	params_Gradient.setName("Gradient");
	params_Gradient.add(color1);
	params_Gradient.add(color2);
	params_Gradient.add(bSwapColors);

	params_circleMode.setName("Circular Type");
	params_circleMode.add(posOffset);
	params_circleMode.add(scaleX);
	params_circleMode.add(scaleY);
	params_circleMode.add(bScaleLink);
	//params_circleMode.add(bEditByMouse);
	params_Gradient.add(params_circleMode);

	params_Editor.setName("3D Editor Mode");
	//params_Editor.add(bEditorMode);
	params_Editor.add(bDrawFloorGrid);
	params_Editor.add(bThemeGreenFloor);
	//params_Gradient.add(params_Editor);

	params_Transform.setName("Transform");
	params_Transform.add(bTransform);
	params_Transform.add(zoom);
	params_Transform.add(angle);
	params_Transform.add(bRotateAuto);
	params_Transform.add(speed);

	params_Preset.setName("Preset");
	params_Preset.add(gradientType);
	params_Preset.add(gradientType_str);
	//params_Preset.add(bScaleLink);
	//params_Preset.add(bEditByMouse);//TODO: broken
	params_Preset.add(SHOW_Gui_Advanced);
	params_Preset.add(SHOW_Help);
	params_Preset.add(bEditByMouse);

	params_Preset.add(params_Gradient);
	params_Preset.add(params_Editor);
	params_Preset.add(params_Transform);

	//--

	params_AppSettings.setName("AppSettings");
#ifdef USE_PRESETS
	params_AppSettings.add(bNewPreset);
	params_AppSettings.add(bSavePreset);
	params_AppSettings.add(bNextPreset);
	params_AppSettings.add(indexFilePreset);
#endif

	//--

	params_Advanced.setName("Advanced");
	params_Advanced.add(bRandomize);
	params_Advanced.add(bRandomizeColors);
	params_Advanced.add(bResetTransform);
	params_Advanced.add(bResetOffset);
	params_Advanced.add(bResetAll);
	params_Advanced.add(bEditByMouse);
	////params_Advanced.add(positionGui);
	//params_AppSettings.add(params_Advanced);

	//params_AppSettings.add(bScaleLink);
	//params_AppSettings.add(bEditByMouse);

	//-

	//exclude
	gradientType_str.setSerializable(false);
	bSwapColors.setSerializable(false);
	bEditByMouse.setSerializable(false);
	//bScaleLink.setSerializable(false);
	bRandomize.setSerializable(false);
	bRandomizeColors.setSerializable(false);
	bRandomizeColors.setSerializable(false);
	bResetTransform.setSerializable(false);
	bResetOffset.setSerializable(false);
	bResetAll.setSerializable(false);
#ifdef USE_PRESETS
	bSavePreset.setSerializable(false);
	bNextPreset.setSerializable(false);
	bNewPreset.setSerializable(false);
#endif

	//--

	//gui

	ofxSurfingHelpers::setThemeDark_ofxGui();

	//1. control
	gui_AppControl.setup("ofxBackgroundGradient");
	gui_AppControl.add(params_Advanced);
	//gui_AppControl.add(params_AppSettings);

	//2. preset
	gui_PresetSettings.setup("PresetSettings");
	gui_PresetSettings.add(params_Preset);

	//--

	//collapse

	//gui1
	//auto &g1 = gui_AppControl.getGroup(params_AppSettings.getName());
	//auto &g2 = g1.getGroup(params_Advanced.getName());
	//g1.minimize();

	auto &g1 = gui_AppControl.getGroup(params_Advanced.getName());
	g1.minimize();

	//g2.getGroup(positionGui.getName()).minimize();
	//g2.minimize();

	//gui2
	auto &g0 = gui_PresetSettings.getGroup(params_Preset.getName());
	g0.minimize();
	g0.getGroup(params_Gradient.getName()).minimize();
	g0.getGroup(params_Transform.getName()).minimize();

	//--

	ofAddListener(params_Advanced.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params_AppSettings);
	//ofAddListener(params_AppSettings.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params_AppSettings);
	ofAddListener(params_Preset.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params_Preset);

	//--

	//mouse callbacks
	ofAddListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);

	//----

	//startup

	setAutoSaveLoad(true);
	setVisibleGui(true);

	//--

	//settings

	bDisableCallbacks = true;

	//app
	//ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_AppSettings);

	params_SettingsXml.setName("SettingsXml");
	params_SettingsXml.add(positionGui);
	params_SettingsXml.add(positionGui2);
	params_SettingsXml.add(SHOW_Gui_Advanced);
	params_SettingsXml.add(SHOW_Gui);

	ofxSurfingHelpers::loadGroup(params_SettingsXml, path_SettingsXml);
	gui_AppControl.setPosition(positionGui.get().x, positionGui.get().y);
	gui_PresetSettings.setPosition(positionGui2.get().x, positionGui2.get().y);

	//preset
#ifndef USE_PRESETS
	ofxSurfingHelpers::loadGroup(params_Preset, path_PresetSettings);
#endif

	//kit
#ifdef USE_PRESETS
	filesRefresh();
	if (autoSaveLoad)
	{
		loadPreset(indexFilePreset);
	}
#endif
}

//--------------------------------------------------------------
void ofxBackgroundGradient::mouseDragged(ofMouseEventArgs &eventArgs)
{
	const int &x = eventArgs.x;
	const int &y = eventArgs.y;
	const int &button = eventArgs.button;
	//ofLogNotice(__FUNCTION__) << "mouseDragged " << x << ", " << y << ", " << button;

	if (bEditByMouse)
	{
		posOffset = glm::vec2(x - w / 2.0f, y - h / 20.f);
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

	if (bEditByMouse)
	{
		if (bKeyMod)//transform
		{
			if (scrollY == 1) zoom += 0.1f;
			else if (scrollY == -1) zoom -= 0.1f;
		}
		else//offset
		{
			if (scrollY == 1) scaleY += 0.1f;
			else if (scrollY == -1) scaleY -= 0.1f;
		}
	}
}

#ifdef USE_PRESETS
//--------------------------------------------------------------
void ofxBackgroundGradient::filesRefresh()
{
	ofDirectory loadDir;
	loadDir.open(path_Presets);
	loadDir.allowExt("xml");

	ofLogNotice(__FUNCTION__) << "Presets files:";
	imgNamesForListBox.clear();
	for (auto im : loadDir) {
		imgNamesForListBox.emplace_back(im.getFileName());
		ofLogNotice(__FUNCTION__) << im.getFileName();
	}

	indexFilePreset.setMax(imgNamesForListBox.size() - 1);
}
#endif

//--------------------------------------------------------------
void ofxBackgroundGradient::drawFloorGrid() {
	//draw a grid on the floor

#define MAX_MAGNITUDE 1000

	ofPushStyle();
	ofPushMatrix();

	ofEnableDepthTest();
	ofSetLineWidth(2.f);
	//ofTranslate(0, -250, 0);

	//dark colors grid
	if (!bThemeGreenFloor)
	{
		ofRotate(90, 0, 0, -1);
		ofSetColor(ofColor(90));
		ofDrawGridPlane(0.4*MAX_MAGNITUDE, 1, false);
		ofSetColor(ofColor(50));
		ofDrawGridPlane(0.1*MAX_MAGNITUDE, 4, false);
	}

	//--

	//green fuxia theme
	else
	{
		//green is default color
		int gridSize = 0.4*MAX_MAGNITUDE;
		ofDrawGrid(gridSize, 1, false, false, true, false);//only ground (xz plane)
		//ofDrawGrid(gridSize, 1, true, true, true, false);//ground and yx plane
		//y axis line
		ofSetColor(greenFuxia);
		ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0.1*gridSize, 0));
		//ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, gridSize, 0));
		//ofDrawLine(glm::vec3(0,MAX_MAGNITUDE,0), glm::vec3(0,-MAX_MAGNITUDE,0));
		//ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, MAX_MAGNITUDE, 0));
	}

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::refresh_Draw()
{
	fbo.begin();
	ofClear(ofColor::black);
	{
		// 0. one color

		if (gradientType == 0)
		{
			ofBackground(color1);
		}

		//-

		// 1. gradient linear

		else if (gradientType == 1)
		{
			ofBackgroundGradient(color1, color2, ofGradientMode(0));
		}

		//-

		// 2.1 gradient circular
		// 2.2 gradient bar 

		else if (gradientType == 2 || gradientType == 3)
		{
			////clamp
			//scaleX = ofClamp(scaleX, 1.f, scaleX.getMax());
			//scaleY = ofClamp(scaleY, 1.f, scaleY.getMax());

			ofPushMatrix();
			ofTranslate(posOffset.get().x, posOffset.get().y);

			ofTranslate(w / 2.f, h / 2.f);
			ofScale(scaleX, scaleY);
			ofTranslate(-w / 2.f, -h / 2.f);

			//ofBackground(color2);
			ofClear(color2);

			if (gradientType == 2) ofBackgroundGradient(color1, color2, ofGradientMode(1));
			else if (gradientType == 3) ofBackgroundGradient(color1, color2, ofGradientMode(2));

		}

		//-

		// 3. image

		else if (gradientType == 4)
		{
			img.draw(0, 0);
		}

		//-

		// 4. editor mode

		else if (gradientType == 5)// && bEditorMode)
		{
			int _g1 = 40;
			int _g2 = 5;
			ofClear(0);
			ofBackgroundGradient(ofColor(_g1), ofColor(_g2), OF_GRADIENT_CIRCULAR);
		}

		//--

		//draw grid plane and testing camera too
		////bool bPlanes = true;
		////if (bPlanes) 
		//{
		//	cam.begin();//TODO: ?
		//	if (bDrawFloorGrid) drawFloorGrid();
		//	cam.end();
		//}

		//----

		if (gradientType == 2 || gradientType == 3)
		{
			ofPopMatrix();
		}

		//---
	}
	fbo.end();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawBackground()
{
	if (bTransform)
	{
		if (bRotateAuto)
		{
			angle += speed;
			//angle = ofWrap(speed, 0, 360);
		}
		ofPushMatrix();
		ofTranslate(w / 2., h / 2.);
		ofRotateZDeg(angle);
		ofScale(zoom, zoom);
		{
			fbo.draw(-w / 2.f, -h / 2.f, w, h);
		}
		ofPopMatrix();

		if (bRotateAuto) refresh_Draw();//TODO: improve calling only when params changed
	}
	else
	{
		fbo.draw(0, 0, w, h);
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawGui()
{
	if (SHOW_Gui)
	{
		if (SHOW_Help) ofxSurfingHelpers::drawTextBoxed(font, helpInfo, 10, 10);

		if (SHOW_Gui_Advanced) gui_AppControl.draw();

		//glm::vec2 posg = gui_AppControl.getShape().getBottomLeft() + glm::vec2(0, 5);
		//glm::vec2 posg = gui_AppControl.getShape().getTopRight() + glm::vec2(5, 0);
		//gui_PresetSettings.setPosition(posg.x, posg.y);
		gui_PresetSettings.draw();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::update(ofEventArgs & args)
{

}

//--------------------------------------------------------------
void ofxBackgroundGradient::draw()
{
	//workaround to avoid callback crashes
	if (ofGetFrameNum() == 0)
	{
		bDisableCallbacks = false;
		setType(gradientType);
		refresh_Gui();
	}

	drawBackground();

	if (SHOW_Gui)
	{
		drawGui();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::randomize()
{
	color1.set(randomColor());
	color2.set(randomColor());
	gradientType = (int)ofRandom(0.0f, 5.0f);
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
	angle = 0;
	speed = 1;
	zoom = 1;
	bTransform = false;
	bRotateAuto = false;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::resetOffset()
{
	posOffset = glm::vec2(0, 0);
	scaleX = scaleY = 1.0;
	//degrees = 0;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_Params_AppSettings(ofAbstractParameter &e)
{
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (0) {}

	else if (name == bResetTransform.getName())
	{
		if (bResetTransform)
		{
			bResetTransform = false;
			resetTransform();
		}
	}
	else if (name == bResetOffset.getName())
	{
		if (bResetOffset)
		{
			bResetOffset = false;
			resetOffset();
		}
	}

	else if (name == bRandomize.getName())
	{
		if (bRandomize)
		{
			bRandomize = false;
			randomize();
		}
	}
	else if (name == bRandomizeColors.getName())
	{
		if (bRandomizeColors)
		{
			bRandomizeColors = false;
			color1.set(randomColor());
			color2.set(randomColor());
		}
	}
	else if (name == bResetAll.getName())
	{
		if (bResetAll)
		{
			bResetAll = false;
			resetAll();
		}
	}
	else if (name == bRotateAuto.getName())
	{
		if (bRotateAuto) bTransform = true;
	}

	//--

#ifdef USE_PRESETS
	//browse
	else if (name == bSavePreset.getName())
	{
		if (bSavePreset)
		{
			bSavePreset = false;

			//saveSettings(params_AppSettings, path_Global + path_file + ofToString(imgNamesForListBox.size() - 1));
			filesRefresh();
			if (indexFilePreset == -1) indexFilePreset = 0;
			ofxSurfingHelpers::saveGroup(params_AppSettings, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
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
					ofxSurfingHelpers::saveGroup(params_AppSettings, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
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
#endif
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_Params_Preset(ofAbstractParameter &e)
{
	if (!bDisableCallbacks)
	{
		std::string name = e.getName();

		if (name != angle.getName())
		{
			ofLogNotice(__FUNCTION__) << name << " : " << e;

			//TODO:
			if (!bRotateAuto) refresh_Draw();
		}
		else
		{
			//nothing to do if angle changed
			return;//make faster
		}

		if (false) {}

		//else if (name == bEditorMode.getName())
		//{
		//	if (bEditorMode) gradientType = 5;
		//}

		else if (name == gradientType.getName())
		{
			const int t = gradientType.get();

			bDisableCallbacks = true;
			setType(t);
			bDisableCallbacks = false;

			refresh_Gui();
		}

		else if (name == bSwapColors.getName())
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

		//--

		else if (name == bScaleLink.getName())
		{
			if (bScaleLink)
			{
				//scaleY.setWithoutEventNotifications(scaleX);
				scaleY.set(scaleX);
			}
		}
		else if (name == scaleX.getName())
		{
			if (bScaleLink)
			{
				//scaleY.setWithoutEventNotifications(scaleX.get());
				scaleY.set(scaleX.get());
			}
		}
		else if (name == scaleY.getName())
		{
			if (bScaleLink)
			{
				//scaleX.setWithoutEventNotifications(scaleY.get());
				scaleX.set(scaleY.get());
			}
		}
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::refresh_Gui()
{
	//collapse

	//gui1
	auto &g1 = gui_AppControl.getGroup(params_Advanced.getName());//app
	//auto &g1 = gui_AppControl.getGroup(params_AppSettings.getName());//app
	//auto &g2 = g1.getGroup(params_Advanced.getName());
	//g2.getGroup(positionGui.getName()).minimize();
	//g1.minimize();
	//g2.minimize();

	//gui2
	auto &g0 = gui_PresetSettings.getGroup(params_Preset.getName());//preset
	//g0.getGroup(params_Gradient.getName()).minimize();
	//g0.getGroup(params_Transform.getName()).minimize();
	//g0.minimize();

	//circular and bar
	auto &ggr = g0.getGroup(params_Gradient.getName());
	auto &gc = ggr.getGroup(params_circleMode.getName());
	gc.minimize();
	if (gradientType == 2 || gradientType == 3)
	{
		gc.maximize();
	}

	//3d editor mode
	auto &ge = g0.getGroup(params_Editor.getName());//editor
	ge.minimize();
	if (gradientType == 5)
	{
		ge.maximize();
		gc.minimize();
		ggr.minimize();
	}

	//image
	if (gradientType == 4)
	{
		ge.minimize();
		gc.minimize();
		ggr.minimize();
	}

}

#ifdef USE_PRESETS
//--------------------------------------------------------------
void ofxBackgroundGradient::loadPreset(int index)
{
	if (index < imgNamesForListBox.size())
	{
		ofxSurfingHelpers::loadGroup(params_AppSettings, path_Presets + imgNamesForListBox[index]);
	}
	else
	{
		ofLogError(__FUNCTION__) << "Presets index file out of range!";
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::loadNext()
{
	bNextPreset = true;
}
#endif

////--------------------------------------------------------------
//void ofxBackgroundGradient::refreshGuiPtr(ofxPanel &_gui)
//{
//}

////--------------------------------------------------------------
//void ofxBackgroundGradient::refreshGuiGrp(ofParameterGroup &_group)
//{
//}

//--------------------------------------------------------------
ofxBackgroundGradient::~ofxBackgroundGradient()
{
	ofRemoveListener(params_Advanced.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params_AppSettings);
	//ofRemoveListener(params_AppSettings.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params_AppSettings);
	ofRemoveListener(params_Preset.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params_Preset);

	ofRemoveListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);

	ofRemoveListener(ofEvents().update, this, &ofxBackgroundGradient::update);

	ofRemoveListener(ofEvents().keyPressed, this, &ofxBackgroundGradient::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxBackgroundGradient::keyReleased);
	ofRemoveListener(ofEvents().windowResized, this, &ofxBackgroundGradient::windowResized);

	exit();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::exit()
{
	if (autoSaveLoad)
	{
		//kit presets
#ifdef USE_PRESETS
		ofxSurfingHelpers::CheckFolder(path_Presets);
		if (indexFilePreset != -1) ofxSurfingHelpers::saveGroup(params_AppSettings, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
#endif

		//preset
#ifndef USE_PRESETS
		ofxSurfingHelpers::saveGroup(params_Preset, path_PresetSettings);
#endif
	}

	//app
	positionGui = gui_AppControl.getPosition();
	positionGui2 = gui_PresetSettings.getPosition();
	//ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_AppSettings);
	ofxSurfingHelpers::saveGroup(params_SettingsXml, path_SettingsXml);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::windowResized(ofResizeEventArgs & args)
{
	w = args.width;
	h = args.height;
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::windowResized(int _w, int _h)
//{
//	w = _w;
//	h = _h;
//}

//--------------------------------------------------------------
void ofxBackgroundGradient::keyReleased(ofKeyEventArgs &eventArgs)
{
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (mod_CONTROL || key == OF_KEY_CONTROL)
	{
		bKeyMod = false;
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::keyPressed(ofKeyEventArgs &eventArgs)
{
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (mod_CONTROL || key == OF_KEY_CONTROL)
	{
		bKeyMod = true;
	}

	if (key == 'G')
	{
		toggleVisibleGui();
	}

	if (key == 'h')
	{
		SHOW_Help = !SHOW_Help;
	}

	if (bEditByMouse)
	{
		//TODO: must enable first..
		if (key == OF_KEY_TAB)
		{
			bSwapColors = !bSwapColors;
		}
		else if (key == '+')
		{
			gradientType++;
			gradientType = (int)ofWrap(gradientType, 0, NUM_TYPES - 1);
		}
		else if (key == '-')
		{
			gradientType--;
			gradientType = (int)ofWrap(gradientType, 0, NUM_TYPES - 1);
		}

		////TODO: broken
		//// monitor moiuse pointer
		//ofPushMatrix();
		//ofTranslate(w / 2.f, h / 2.f);
		//{
		//	ofPushStyle();
		//	ofFill();
		//	ofSetColor(0, 255);
		//	ofDrawCircle(posOffset, 6);
		//	ofSetColor(255, 255);
		//	ofDrawCircle(posOffset, 4);
		//	ofPopStyle();
		//}
		//ofPopMatrix();
	}
}