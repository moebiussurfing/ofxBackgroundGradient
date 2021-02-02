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

	path_file = "ofxBackgroundGradient_";
	path_AppSettings = "AppSettings";
	path_AppSettings += _extension;
	
	path_Presets = path_Global + "presets/";
	path_Images = path_Global + "images/";
	
	//-

	greenFuxia.setHsb(255.0f / 3.0f, 200.0f, 255.0f, 255.0f);

	setup();
}

//--------------------------------------------------------------
ofxBackgroundGradient::~ofxBackgroundGradient()
{
	ofRemoveListener(params_Settings.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
	ofRemoveListener(params_AppSettings.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

	ofRemoveListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);

	exit();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::exit()
{
	if (autoSaveLoad)
	{
		ofxSurfingHelpers::CheckFolder(path_Presets);

#ifdef USE_PRESETS
		if (indexFilePreset != -1) ofxSurfingHelpers::saveGroup(params_Settings, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
#endif
	}

	positionGui = gui_AppControl.getPosition();
	ofxSurfingHelpers::saveGroup(params_AppSettings, path_Global + path_AppSettings);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::setup()
{
	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Images);
	ofxSurfingHelpers::CheckFolder(path_Presets);

	img.load(path_Images + "img5.png");

	//--

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

	bTransform.set("Transform", false);
	bRotate.set("Rotator", false);
	angle.set("Angle", 0.0f, -180.1f, 180.0f);
	speed.set("Speed", 1.f, 0.01f, 5.0f);
	zoom.set("Zoom", 1.0f, 0.5f, 3.0f);

	pos.set("Offset", glm::vec2(0, 0),
		glm::vec2(-ofGetWidth(), -ofGetHeight()),
		glm::vec2(ofGetWidth(), ofGetHeight()));
	color1.set("C1", ofColor(200), ofColor(0), ofColor(255));
	color2.set("C2", ofColor(32), ofColor(0), ofColor(255));
	gradientType.set("Type", 0, 0, 4);
	gradientType_str.set("", "");
	scaleX.set("Scale x", 2.0f, 0.1f, 5.0f);
	scaleY.set("Scale y", 2.0f, 0.1f, 5.0f);

	bScaleLink.set("Link Scales", true);
	degrees.set("Degrees", 0.0f, 0.0f, 360.0f);
	bRandomize.set("Randomize All", false);
	bSwapColors.set("Swap Colors", false);
	bRandomizeColors.set("Randomize Colors", false);
	bResetAll.set("Reset All", false);
	bResetTransform.set("Reset Transform", false);
	bEditorMode.set("Editor Mode", false);
	bDrawFloorGrid.set("Floor Grid", false);
	bThemeGreenFloor.set("Green Floor", false);

#ifdef USE_PRESETS
	bNewPreset.set("New Preset", false);
	bNextPreset.set("Next Preset", false);
	bSavePreset.set("Save Preset", false);
	indexFilePreset.set("Preset", -1, 0, 1);
#endif

	//-

	params_Editor.setName("Settings");
	params_Editor.add(bEditorMode);
	params_Editor.add(bDrawFloorGrid);
	params_Editor.add(bThemeGreenFloor);

	params_Settings.setName("Colors");
	params_Settings.add(gradientType);
	params_Settings.add(gradientType_str);
	params_Settings.add(color1);
	params_Settings.add(color2);
	params_Settings.add(bSwapColors);

	params_circleMode.setName("Circular Type");
	params_circleMode.add(pos);
	params_circleMode.add(scaleX);
	params_circleMode.add(scaleY);
	//params_circleMode.add(degrees);
	params_Settings.add(params_circleMode);

	params_Editor.setName("Editor Mode");
	params_Editor.add(bEditorMode);
	params_Editor.add(bDrawFloorGrid);
	params_Editor.add(bThemeGreenFloor);
	params_Settings.add(params_Editor);

	//browse
	params_AppSettings.setName("AppSettings");
#ifdef USE_PRESETS
	params_AppSettings.add(bNewPreset);
	params_AppSettings.add(bSavePreset);
	params_AppSettings.add(bNextPreset);
	params_AppSettings.add(indexFilePreset);
#endif
	params_AppSettings.add(bScaleLink);
	params_AppSettings.add(bEditByMouse);
	//params_AppSettings.add(bEditorMode);

	params_Advanced.setName("Avanced");
	params_Advanced.add(bRandomize);
	params_Advanced.add(bRandomizeColors);
	params_Advanced.add(bResetTransform);
	params_Advanced.add(bResetAll);
	params_Advanced.add(positionGui);
	params_AppSettings.add(params_Advanced);

	params_Transform.setName("Transform");
	params_Transform.add(bTransform);
	params_Transform.add(bRotate);
	params_Transform.add(angle);
	params_Transform.add(speed);
	params_Transform.add(zoom);

	//exclude
	gradientType_str.setSerializable(false);
	bSwapColors.setSerializable(false);
	bEditByMouse.setSerializable(false);
	bScaleLink.setSerializable(false);
	bRandomize.setSerializable(false);
	bRandomizeColors.setSerializable(false);
	bRandomizeColors.setSerializable(false);
	bResetTransform.setSerializable(false);
	bResetAll.setSerializable(false);
	
#ifdef USE_PRESETS
	bSavePreset.setSerializable(false);
	bNextPreset.setSerializable(false);
	bNewPreset.setSerializable(false);
#endif

	//--

	//gui

	ofxSurfingHelpers::setThemeDark_ofxGui();

	gui_Settings.setup("Settings");
	gui_Settings.add(params_Settings);
	gui_Settings.add(params_Transform);

	gui_AppControl.setup("ofxBackgroundGradient");
	gui_AppControl.add(params_AppSettings);

	gui_AppControl.setPosition(positionGui.get().x, positionGui.get().y);

	//--

	//mouse callbacks
	ofAddListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);

	//--

	//collapse

	//gui2
	auto &g0 = gui_Settings.getGroup(params_Settings.getName());
	g0.minimize();
	g0.getGroup(params_Editor.getName()).minimize();
	auto &gt = gui_Settings.getGroup(params_Transform.getName());
	gt.minimize();

	//gui1
	auto &g1 = gui_AppControl.getGroup(params_AppSettings.getName());
	auto &g2 = g1.getGroup(params_Advanced.getName());
	g1.minimize();
	g2.getGroup(positionGui.getName()).minimize();
	g2.minimize();

	//--

#ifdef USE_PRESETS
	filesRefresh();
	if (autoSaveLoad)
	{
		loadPreset(indexFilePreset);
	}
#endif

	//--

	//settings

	ofAddListener(params_AppSettings.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);
	ofAddListener(params_Settings.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Params);

	ofxSurfingHelpers::loadGroup(params_AppSettings, path_Global + path_AppSettings);

	//--

	//startup
	setAutoSaveLoad(true);
	setVisibleGui(true);
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
void ofxBackgroundGradient::refresh()
{
	fbo.begin();
	ofClear(ofColor::black);
	{
		//editor mode

		if (bEditorMode)
		{
			int _g1 = 40;
			int _g2 = 5;
			ofClear(0);
			ofBackgroundGradient(ofColor(_g1), ofColor(_g2), OF_GRADIENT_CIRCULAR);
		}

		//-

		else
		{
			//0. one color

			if (gradientType == 0)
			{
				ofBackground(color1);
			}

			//-

			//1. gradient linear

			else if (gradientType == 1)
			{
				ofBackgroundGradient(color1, color2, ofGradientMode(0));
			}

			//-

			//2.1 gradient circular
			//2.2 gradient bar 

			else if (gradientType == 2 || gradientType == 3)
			{
				////clamp
				//scaleX = ofClamp(scaleX, 1.f, scaleX.getMax());
				//scaleY = ofClamp(scaleY, 1.f, scaleY.getMax());

				ofPushMatrix();
				ofTranslate(pos.get().x, pos.get().y);

				ofTranslate(w / 2.f, h / 2.f);
				ofScale(scaleX, scaleY);
				ofTranslate(-w / 2.f, -h / 2.f);

				//ofBackground(color2);
				ofClear(color2);

				if (gradientType == 2) ofBackgroundGradient(color1, color2, ofGradientMode(1));
				else if (gradientType == 3) ofBackgroundGradient(color1, color2, ofGradientMode(2));

			}

			//-

			//3. image
			else if (gradientType == 4)
			{
				img.draw(0, 0);
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

			//mouse picker
			if (bEditByMouse)
			{
				//TODO: must enable first..
				if (ofGetKeyPressed() == OF_KEY_TAB) bSwapColors = !bSwapColors;
				else if (ofGetKeyPressed() == '+')
				{
					gradientType++;
					gradientType = (int)ofWrap(gradientType, 0, 3);
				}

				ofPushMatrix();
				ofTranslate(w / 2.f, h / 2.f);
				{
				ofPushStyle();
					ofFill();
					ofSetColor(0, 255);
					ofDrawCircle(pos, 6);
					ofSetColor(255, 255);
					ofDrawCircle(pos, 4);
				ofPopStyle();
				}
				ofPopMatrix();
			}

			//-

			if (gradientType == 2 || gradientType == 3)
			{
				ofPopMatrix();
			}
		}

		//---
	}
	fbo.end();
}
//--------------------------------------------------------------
void ofxBackgroundGradient::drawBackground()
{
	refresh();//TODO: improve calling only when params changed

	//--

	if (bTransform)
	{
		if (bRotate) angle += speed;
		ofPushMatrix();
		ofTranslate(w / 2., h / 2.);
		ofRotateZDeg(angle);
		ofScale(zoom, zoom);
		{
			fbo.draw(-w / 2.f, -h / 2.f, w, h);
		}
		ofPopMatrix();
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
		gui_AppControl.draw();

		//glm::vec2 posg = gui_AppControl.getShape().getBottomLeft() + glm::vec2(0, 5);
		glm::vec2 posg = gui_AppControl.getShape().getTopRight() + glm::vec2(5, 0);
		gui_Settings.setPosition(posg.x, posg.y);
		gui_Settings.draw();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::draw()
{
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
	pos = glm::vec2(0, 0);
	scaleX = scaleY = 1.0;
	degrees = 0;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_Params(ofAbstractParameter &e)
{
	std::string name = e.getName();

	if (0) {}

	else if (name == gradientType.getName())
	{
		switch (gradientType.get())
		{
		case 0:
			gradientType_str = "ONE COLOR";
			break;
		case 1:
			gradientType_str = "LINEAR";
			break;
		case 2:
			gradientType_str = "CIRCULAR";
			break;
		case 3:
			gradientType_str = "BAR";
			break;
		case 4:
			gradientType_str = "IMAGE";
			break;
		}

		refreshGui();
	}

	else if (name == bRandomize.getName())
	{
		if (bRandomize)
		{
			bRandomize = false;
			randomize();
		}
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


#ifdef USE_PRESETS
	//browse
	else if (name == bSavePreset.getName())
	{
		if (bSavePreset)
		{
			bSavePreset = false;

			//saveSettings(params_Settings, path_Global + path_file + ofToString(imgNamesForListBox.size() - 1));
			filesRefresh();
			if (indexFilePreset == -1) indexFilePreset = 0;
			ofxSurfingHelpers::saveGroup(params_Settings, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
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
					ofxSurfingHelpers::saveGroup(params_Settings, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
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

	//--

	else if (name == bResetTransform.getName())
	{
		if (bResetTransform)
		{
			bResetTransform = false;
			resetTransform();
		}
	}
	else if (name == bScaleLink.getName())
	{
		if (bScaleLink)
		{
			scaleY = scaleX;
		}
	}
	else if (name == scaleX.getName())
	{
		if (bScaleLink)
		{
			scaleY = scaleX.get();
		}
	}
	else if (name == scaleY.getName())
	{
		if (bScaleLink)
		{
			scaleX = scaleY.get();
		}
	}
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::refreshGuiPtr(ofxPanel &_gui)
//{
//	//collapse
//	auto &g0 = _gui_AppControl.getGroup("GRADIENT BACKGROUND");
//	auto &g1 = g0.getGroup("CIRCULAR TYPE");
//	g1.minimize();
//	if (gradientType == 2 || gradientType == 3)
//		g1.maximize();
//	else
//		g1.minimize();
//}

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
	auto &g0 = gui_Settings.getGroup(params_Settings.getName());
	auto &gc = g0.getGroup(params_circleMode.getName());
	gc.minimize();

	if (gradientType == 2) gc.maximize();
}

#ifdef USE_PRESETS

//--------------------------------------------------------------
	void ofxBackgroundGradient::loadPreset(int index)
	{
		if (index < imgNamesForListBox.size())
		{
			ofxSurfingHelpers::loadGroup(params_Settings, path_Presets + imgNamesForListBox[index]);
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