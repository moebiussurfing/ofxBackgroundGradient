#include "ofxBackgroundGradient.h"

//--------------------------------------------------------------
ofxBackgroundGradient::ofxBackgroundGradient() {
	setup();
}

//--------------------------------------------------------------
ofxBackgroundGradient::~ofxBackgroundGradient() {
	ofRemoveListener(params_Helpers.parameterChangedE(), this, &ofxBackgroundGradient::Changed_AppSettings);

	ofRemoveListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofRemoveListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);
	ofRemoveListener(ofEvents().update, this, &ofxBackgroundGradient::update);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxBackgroundGradient::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &ofxBackgroundGradient::keyReleased);
	ofRemoveListener(ofEvents().windowResized, this, &ofxBackgroundGradient::windowResized);

	exit();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::setup() {
#ifdef USE_JSON
	_extension = ".json";
#else
	#ifdef USE_XML
	_extension = ".xml";
	#else //default when not defined any directive!
	_extension = ".json";
	#endif
#endif

	path_Global = "ofxBgGradient/";
	//path_Global = "ofxBackgroundGradient/";

	path_Images = path_Global + "images/";

	path_Internal = path_Global + "BgGradient_Internal" + _extension;

#ifdef USE_PRESETS
	path_file = "ofxBackgroundGradient_";
	path_Presets = path_Global + "presets/";
	path_PresetsManager = "AppSettings";
	path_PresetsManager += _extension;

#endif

#ifndef USE_PRESETS
	path_PresetSettings = path_Global + "BgGradient_Preset" + _extension;
#endif

	//--

	ofxSurfingHelpers::CheckFolder(path_Global);
	ofxSurfingHelpers::CheckFolder(path_Images);

#ifdef USE_PRESETS
	ofxSurfingHelpers::CheckFolder(path_Presets);
#endif

	ofAddListener(ofEvents().windowResized, this, &ofxBackgroundGradient::windowResized);
	ofAddListener(ofEvents().update, this, &ofxBackgroundGradient::update);
	ofAddListener(ofEvents().keyPressed, this, &ofxBackgroundGradient::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxBackgroundGradient::keyReleased);

	//--

	helpInfo = "HELP\n\n";
	helpInfo += "H              HELP\n";
	helpInfo += "G              GUI\n";
	helpInfo += "+|-            TYPE\n";
	helpInfo += "TAB            FLIP COLORS\n";
	helpInfo += "M              MOUSE EDIT\n";
	helpInfo += "MOUSE WHEEL    ZOOM TRANSFORM\n";
	helpInfo += "+Ctrl          ZOOM OFFSET\n";

	//--

	greenFuxia.setHsb(255.0f / 3.0f, 200.0f, 255.0f, 255.0f);

	//--

	img.load(path_Images + "img4.png");

	bool b = font.load("assets/fonts/telegrama_render.otf", 10, true, true, true);
	if (!b) ofLogError(__FUNCTION__) << "Font file not found!";

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

	setupParams();
}
//--------------------------------------------------------------
void ofxBackgroundGradient::setupParams() {

	bGui.set("BgGradient", true);

	bEditByMouse.set("Mouse Edit", false);
	bGui_Helpers.set("Gui Helpers", false);
	bGui_Help.set("Help", false);
	bKeys.set("Keys", true);

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

	gradientType.set("Type", 2, 0, NUM_TYPES - 1);
	gradientType_str.set(" ", "");

	scaleX.set("Scale x", 2.0f, 0.1f, 5.0f);
	scaleY.set("Scale y", 2.0f, 0.1f, 5.0f);

	bScaleLink.set("Link Scales", true);

	bRandomizeAll.set("Randomize All");
	bRandomizeAllExceptType.set("Randomize All No Type");
	bFlipColors.set("Flip Colors");
	bRandomizeColors.set("Randomize Colors");
	bResetAll.set("Reset All");
	bResetAllExceptColors.set("Reset All No Colors");
	bResetTransform.set("Reset Transform");
	bResetOffset.set("Reset Offset");

	bDrawFloorGrid.set("Grey Floor", false);
	bThemeGreenFloor.set("Green Floor", true);

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
	params_Gradient.add(bFlipColors);

	params_circleMode.setName("Circular Type");
	params_circleMode.add(posOffset);
	params_circleMode.add(scaleX);
	params_circleMode.add(scaleY);
	params_circleMode.add(bScaleLink);
	params_Gradient.add(params_circleMode);

	params_Editor.setName("3D Editor Mode");
	params_Editor.add(bDrawFloor);
	params_Editor.add(bDrawFloorGrid);
	params_Editor.add(bThemeGreenFloor);

	params_Transform.setName("Transform");
	params_Transform.add(bTransform);
	params_Transform.add(zoom);
	params_Transform.add(angle);
	params_Transform.add(bRotateAuto);
	params_Transform.add(speed);

	params_Preset.setName("Preset");
	params_Preset.add(bDrawBg);
	params_Preset.add(gradientType);
	params_Preset.add(gradientType_str);

	params_Preset.add(params_Gradient);
	params_Preset.add(params_Transform);

	params_Preset.add(params_Editor);

	params_Preset.add(bGui_Help);
	params_Preset.add(bKeys);
	params_Preset.add(bEditByMouse);
	params_Preset.add(bGui_Helpers);

	//--

#ifdef USE_PRESETS
	params_PresetManager.setName("AppSettings");
	params_PresetManager.add(bNewPreset);
	params_PresetManager.add(bSavePreset);
	params_PresetManager.add(bNextPreset);
	params_PresetManager.add(indexFilePreset);
#endif

	//--

	params_Helpers.setName("Helpers");
	params_Helpers.add(bRandomizeAll);
	params_Helpers.add(bRandomizeAllExceptType);
	params_Helpers.add(bRandomizeColors);
	params_Helpers.add(bResetTransform);
	params_Helpers.add(bResetOffset);
	params_Helpers.add(bResetAllExceptColors);
	params_Helpers.add(bResetAll);

	//-

	// exclude
	gradientType_str.setSerializable(false);
	bEditByMouse.setSerializable(false);

#ifdef USE_PRESETS
	bSavePreset.setSerializable(false);
	bNextPreset.setSerializable(false);
	bNewPreset.setSerializable(false);
#endif

	//--

	// gui

	if (1) ofxSurfingHelpers::setThemeDark_ofxGui();

	// 1. control
	gui_Helpers.setup("BgGradient Helpers");
	gui_Helpers.add(params_Helpers);

	// 2. preset
	gui_Preset.setup("BgGradient Preset");
	gui_Preset.add(params_Preset);

	//--

	auto & g1 = gui_Helpers.getGroup(params_Helpers.getName());

	auto & g0 = gui_Preset.getGroup(params_Preset.getName());
	g0.getGroup(params_Gradient.getName()).minimize();
	g0.getGroup(params_Transform.getName()).minimize();

	//--

	ofAddListener(params_Helpers.parameterChangedE(), this, &ofxBackgroundGradient::Changed_AppSettings);
	ofAddListener(params_Preset.parameterChangedE(), this, &ofxBackgroundGradient::Changed_Preset);

	//--

	// mouse callbacks
	ofAddListener(ofEvents().mouseDragged, this, &ofxBackgroundGradient::mouseDragged);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxBackgroundGradient::mouseScrolled);

	//----

	startup();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::startup() {

	bDisableCallbacks = true;

	// Fix
	doResetAll();
	doResetOffset();

#ifdef USE_PRESETS
//ofxSurfingHelpers::loadGroup(params_PresetManager, path_Global + path_PresetsManager);
#endif

	// internal
	params_Internal.setName("Internal");
	params_Internal.add(bGui_Helpers);
	params_Internal.add(bGui);

	// Load settings

	ofxSurfingHelpers::loadGroup(params_Internal, path_Internal);

	// Preset
#ifndef USE_PRESETS
	ofxSurfingHelpers::loadGroup(params_Preset, path_PresetSettings);
#endif

	// Kit
#ifdef USE_PRESETS
	filesRefresh();
	if (bAutoSaveLoad) {
		loadPreset(indexFilePreset);
	}
#endif
}

//--

//--------------------------------------------------------------
void ofxBackgroundGradient::mouseDragged(ofMouseEventArgs & eventArgs) {
	if (!bDrawBg) return;

	const int & x = eventArgs.x;
	const int & y = eventArgs.y;
	const int & button = eventArgs.button;

	ofLogVerbose(__FUNCTION__) << "mouseDragged " << x << ", " << y << ", " << button;

	if (bEditByMouse) {
		posOffset = glm::vec2(x - w / 2.0f, y - h / 20.f);
	}
}
//--------------------------------------------------------------
void ofxBackgroundGradient::mouseScrolled(ofMouseEventArgs & eventArgs) {
	if (!bDrawBg) return;

	const int & x = eventArgs.x;
	const int & y = eventArgs.y;
	const float & scrollX = eventArgs.scrollX;
	const float & scrollY = eventArgs.scrollY;

	ofLogNotice(__FUNCTION__) << "scrollX: " << scrollX << "  scrollY: " << scrollY;

	if (bEditByMouse) {
		if (bKeyMod) //transform
		{
			if (scrollY == 1)
				zoom += 0.1f;
			else if (scrollY == -1)
				zoom -= 0.1f;
		} else //offset
		{
			if (scrollY == 1)
				scaleY += 0.1f;
			else if (scrollY == -1)
				scaleY -= 0.1f;
		}
	}
}

#ifdef USE_PRESETS
//--------------------------------------------------------------
void ofxBackgroundGradient::filesRefresh() {
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
void ofxBackgroundGradient::drawFloor() {
	if (!bDrawBg) return;
	if (!bDrawFloor) return;
	if (!bDrawFloorGrid && !bThemeGreenFloor) return;

		//-

		// draw a grid on the floor
#define MAX_MAGNITUDE 1000

	ofPushStyle();
	ofPushMatrix();
	{
		ofEnableDepthTest();
		ofSetLineWidth(2.f);
		//ofTranslate(0, -250, 0);

		//dark colors grid
		if (bDrawFloorGrid) {
			ofRotate(90, 0, 0, -1);
			ofSetColor(ofColor(90));
			ofDrawGridPlane(0.4 * MAX_MAGNITUDE, 1, false);
			ofSetColor(ofColor(50));
			ofDrawGridPlane(0.1 * MAX_MAGNITUDE, 4, false);
		}

		//--

		// green fuxia theme
		else if (bThemeGreenFloor) {
			//green is default color
			int gridSize = 0.4 * MAX_MAGNITUDE;
			ofDrawGrid(gridSize, 1, false, false, true, false); //only ground (xz plane)
			//ofDrawGrid(gridSize, 1, true, true, true, false);//ground and yx plane
			//y axis line
			ofSetColor(greenFuxia);
			ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0.1 * gridSize, 0));
			//ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, gridSize, 0));
			//ofDrawLine(glm::vec3(0,MAX_MAGNITUDE,0), glm::vec3(0,-MAX_MAGNITUDE,0));
			//ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, MAX_MAGNITUDE, 0));
		}
	}
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::refresh_Draw() {
	if (!bDrawBg) return;

	fbo.begin();
	ofClear(ofColor::black);
	{
		// 0. one color

		if (gradientType == 0) {
			ofBackground(color1);
		}

		//-

		// 1. gradient linear

		else if (gradientType == 1) {
			ofBackgroundGradient(color1.get(), color2.get(), ofGradientMode(0));
		}

		//-

		// 2.1 gradient circular
		// 2.2 gradient bar

		else if (gradientType == 2 || gradientType == 3) {
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

			if (gradientType == 2)
				ofBackgroundGradient(color1.get(), color2.get(), ofGradientMode(1));
			else if (gradientType == 3)
				ofBackgroundGradient(color1.get(), color2.get(), ofGradientMode(2));
		}

		//-

		// 3. image

		else if (gradientType == 4) {
			if (img.isAllocated()) img.draw(0, 0);
		}

		//-

		// 4. editor mode

		else if (gradientType == 5) // && bEditorMode)
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
		//	if (bDrawFloorGrid) drawFloor();
		//	cam.end();
		//}

		//----

		if (gradientType == 2 || gradientType == 3) {
			ofPopMatrix();
		}

		//---
	}
	fbo.end();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawBackground() {
	if (!bDrawBg) return;

	if (bTransform) {
		if (bRotateAuto) {
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

		if (bRotateAuto) refresh_Draw(); //TODO: improve calling only when params changed
	} else {
		fbo.draw(0, 0, w, h);
	}

	if (bEditByMouse && bGui) {
		float r = 5;
		ofPushStyle();
		ofPushMatrix();
		//ofTranslate(posOffset.get());
		ofTranslate(glm::vec2(posOffset.get().x, posOffset.get().y) + glm::vec2(ofGetWidth() / 2, ofGetHeight() / 2));
		ofFill();
		ofSetColor(255, 225);
		ofDrawCircle(0, 0, r);
		ofNoFill();
		ofSetColor(0, 255);
		ofDrawCircle(0, 0, r);
		ofPopMatrix();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::drawGui() {
	if (bGui) {
		ofDisableDepthTest();

		//help
		if (bGui_Help) ofxSurfingHelpers::drawTextBoxed(font, helpInfo, 10, 10);

		if (bGui_Helpers) {
			auto p = gui_Preset.getShape().getTopRight() + glm::vec2(2, 0);
			gui_Helpers.setPosition(p);
			gui_Helpers.draw();
		}

		gui_Preset.draw();
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::update(ofEventArgs & args) {
	if (!bDrawBg) return;

	//workaround to avoid callback crashes
	if (ofGetFrameNum() == 0) {
		bDisableCallbacks = false;
		setType(gradientType);
		doRefreshGui();
	}
}

////--------------------------------------------------------------
//void ofxBackgroundGradient::draw() {
//	drawBackground();
//
//	//-
//
//	if (bGui) {
//		drawGui();
//	}
//}

//--------------------------------------------------------------
void ofxBackgroundGradient::doRandomizeAll() {
	doRandomizeAllExceptType();
	gradientType = (int)ofRandom(0.0f, 4.0f);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::doRandomizeAllExceptType() {
	color1.set(doRandomColor());
	color2.set(doRandomColor());
	scaleX = (int)ofRandom(0.5f, 6.0f);
	scaleY = (int)ofRandom(0.5f, 6.0f);
}

//------------------------------------------------------------
ofColor ofxBackgroundGradient::doRandomColor() {
	ofColor myColor;
	myColor.set(ofRandom(255), ofRandom(255), ofRandom(255));
	return myColor;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::doResetAll() {
	color1.set(ofColor(24));
	color2.set(ofColor(127));

	//gradientType = 0;
	gradientType = 2;

	doResetTransform();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::doResetAllExceptColors() {
	gradientType = 2;

	doResetTransform();
}

//--------------------------------------------------------------
void ofxBackgroundGradient::doResetTransform() {
	angle = 0;
	speed = 1;
	zoom = 1;
	bTransform = false;
	bRotateAuto = false;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::doResetOffset() {
	posOffset = glm::vec2(0, 0);
	scaleX = scaleY = 1.0;
	//degrees = 0;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_AppSettings(ofAbstractParameter & e) {
	std::string name = e.getName();

	ofLogNotice(__FUNCTION__) << name << " : " << e;

	if (0) {
	}

	else if (name == bResetTransform.getName()) {
		doResetTransform();
	} else if (name == bResetOffset.getName()) {
		doResetOffset();
	}

	else if (name == bRandomizeAll.getName()) {
		doRandomizeAll();
	} else if (name == bRandomizeAllExceptType.getName()) {
		doRandomizeAllExceptType();
	} else if (name == bRandomizeColors.getName()) {
		color1.set(doRandomColor());
		color2.set(doRandomColor());
	} else if (name == bResetAll.getName()) {
		doResetAll();
	}  else if (name == bResetAllExceptColors.getName()) {
		doResetAllExceptColors();
	} else if (name == bRotateAuto.getName()) {
		if (bRotateAuto) bTransform = true;
	}

	//--

#ifdef USE_PRESETS
	//browse
	else if (name == bSavePreset.getName()) {
		if (bSavePreset) {
			bSavePreset = false;

			//saveSettings(params_PresetManager, path_Global + path_file + ofToString(imgNamesForListBox.size() - 1));
			filesRefresh();
			if (indexFilePreset == -1) indexFilePreset = 0;
			ofxSurfingHelpers::saveGroup(params_PresetManager, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
		}
	} else if (name == bNextPreset.getName()) {
		if (bNextPreset) {
			bNextPreset = false;

			if (imgNamesForListBox.size() == 0) { //no files in folder
				indexFilePreset = -1;
			} else {
				if (bAutoSaveLoad) {
					ofxSurfingHelpers::saveGroup(params_PresetManager, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
				}

				if (indexFilePreset < imgNamesForListBox.size() - 1)
					indexFilePreset++;
				else
					indexFilePreset = 0;
			}
		}
	} else if (name == bNewPreset.getName()) {
		if (bNewPreset) {
			bNewPreset = false;

			if (indexFilePreset < imgNamesForListBox.size()) //go to last
				indexFilePreset = imgNamesForListBox.size();
		}
	} else if (name == indexFilePreset.getName()) {
		filesRefresh();

		if (indexFilePreset < imgNamesForListBox.size()) {
			loadPreset(indexFilePreset);
		}
	}
#endif
}

//--------------------------------------------------------------
void ofxBackgroundGradient::Changed_Preset(ofAbstractParameter & e) {
	if (!bDisableCallbacks) {
		std::string name = e.getName();

		if (name != angle.getName()) {
			ofLogNotice(__FUNCTION__) << name << " : " << e;

			//TODO:
			if (!bRotateAuto) refresh_Draw();
		} else {
			//nothing to do if angle changed
			return; //make faster
		}

		if (false) {
		}

		//else if (name == bEditorMode.getName())
		//{
		//	if (bEditorMode) gradientType = 5;
		//}

		else if (name == gradientType.getName()) {
			const int t = gradientType.get();

			bDisableCallbacks = true;
			setType(t);
			bDisableCallbacks = false;

			doRefreshGui();
		}

		else if (name == bFlipColors.getName()) {
			ofColor _c;
			_c.set(color1.get());
			//color1.setWithoutEventNotifications(color2.get());
			//color2.setWithoutEventNotifications(_c);
			color1.set(color2.get());
			color2.set(_c);
		}

		//--

		else if (name == bScaleLink.getName()) {
			if (bScaleLink) {
				//scaleY.setWithoutEventNotifications(scaleX);
				scaleY.set(scaleX);
			}
		} else if (name == scaleX.getName()) {
			if (bScaleLink) {
				//scaleY.setWithoutEventNotifications(scaleX.get());
				scaleY.set(scaleX.get());
			}
		} else if (name == scaleY.getName()) {
			if (bScaleLink) {
				//scaleX.setWithoutEventNotifications(scaleY.get());
				scaleX.set(scaleY.get());
			}
		}

		//--

		// floor
		else if (name == bDrawFloorGrid.getName()) {
			if (bDrawFloorGrid)
				bThemeGreenFloor = false;
			else
				bThemeGreenFloor = true;
		} else if (name == bThemeGreenFloor.getName()) {
			if (bThemeGreenFloor)
				bDrawFloorGrid = false;
			else
				bDrawFloorGrid = true;
		}
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::doRefreshGui() {
	// minimize

	auto & g1 = gui_Helpers.getGroup(params_Helpers.getName()); // app
	auto & g0 = gui_Preset.getGroup(params_Preset.getName()); // preset

	// circular and bar
	auto & ggr = g0.getGroup(params_Gradient.getName());
	auto & gc = ggr.getGroup(params_circleMode.getName());
	gc.minimize();
	if (gradientType == 2 || gradientType == 3) {
		gc.maximize();
	}

	// 3d editor mode
	auto & ge = g0.getGroup(params_Editor.getName()); //editor
	ge.minimize();
	if (gradientType == 5) {
		ge.maximize();
		gc.minimize();
		ggr.minimize();
	}

	// image
	if (gradientType == 4) {
		ge.minimize();
		gc.minimize();
		ggr.minimize();
	}
}

#ifdef USE_PRESETS
//--------------------------------------------------------------
void ofxBackgroundGradient::loadPreset(int index) {
	if (index < imgNamesForListBox.size()) {
		ofxSurfingHelpers::loadGroup(params_PresetManager, path_Presets + imgNamesForListBox[index]);
	} else {
		ofLogError(__FUNCTION__) << "Presets index file out of range!";
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::loadNext() {
	bNextPreset = true;
}
#endif

//--------------------------------------------------------------
void ofxBackgroundGradient::exit() {
	if (bAutoSaveLoad) {
		// kit presets
#ifdef USE_PRESETS
		ofxSurfingHelpers::CheckFolder(path_Presets);
		if (indexFilePreset != -1) ofxSurfingHelpers::saveGroup(params_PresetManager, path_Presets + path_file + ofToString(indexFilePreset) + _extension);
#endif

		// preset
#ifndef USE_PRESETS
		ofxSurfingHelpers::saveGroup(params_Preset, path_PresetSettings);
#endif
	}

	//ofxSurfingHelpers::saveGroup(params_PresetManager, path_Global + path_PresetsManager);

	ofxSurfingHelpers::saveGroup(params_Internal, path_Internal);
}

//--------------------------------------------------------------
void ofxBackgroundGradient::windowResized(ofResizeEventArgs & args) {
	w = args.width;
	h = args.height;
}

//--------------------------------------------------------------
void ofxBackgroundGradient::keyReleased(ofKeyEventArgs & eventArgs) {
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (mod_CONTROL || key == OF_KEY_CONTROL) {
		bKeyMod = false;
	}
}

//--------------------------------------------------------------
void ofxBackgroundGradient::keyPressed(ofKeyEventArgs & eventArgs) {
	if (!bKeys) return;

	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	if (mod_CONTROL || key == OF_KEY_CONTROL) {
		bKeyMod = true;
	}

	if (key == 'G') {
		toggleVisibleGui();
	}

	if (key == 'h') {
		bGui_Help = !bGui_Help;
	}

	if (key == 'm') {
		bEditByMouse = !bEditByMouse;
	}

	if (bEditByMouse) {
		//TODO: must enable first..
		if (key == OF_KEY_TAB) {
			bFlipColors.trigger();
		} else if (key == '+') {
			gradientType++;
			gradientType = (int)ofWrap(gradientType, 0, NUM_TYPES - 1);
		} else if (key == '-') {
			gradientType--;
			gradientType = (int)ofWrap(gradientType, 0, NUM_TYPES - 1);
		}

		////TODO: broken
		//// monitor mouse pointer
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