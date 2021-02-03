# ofxBackgroundGradient 

## Overview
*ofxBackgroundGradient* is an addon for *openFrameworks* to easy generate different gradiented background styles.  

## Screenshots
![image](readme_images/Capture1.PNG?raw=true "Capture1.PNG")  

## Features
- 5 different types: Linear, Circular, Bar, Image and 3D editor dark-style with 3D-Grid-Floor.  
- Two colors with offset transforms: scale and x,y position for cicrcular type.
- Transform animator to auto rotator with variable speed.  
- Mouse control with wheel.  
- Helpers: randomize colors, transforms, reset, swap colors...  
- Control GUI.  
- Store and handle different presets.  

## Usage

ofApp.h
```.cpp
#include "ofxBackgroundGradient.h"

class ofApp : public ofBaseApp {

public:
	void draw();

	ofxBackgroundGradient bg;
};

```

ofApp.cpp
```.cpp
void ofApp::draw() 
{
	bg.draw();
}
```

## Dependencies
* ofxGui
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*