# ofxBackgroundGradient 

## Overview
*ofxBackgroundGradient* is an add-on for *openFrameworks* to easy generate different gradiented background styles.  

## Screenshots
![image](readme_images/Capture1.PNG?raw=true "Capture1.PNG")  

## Features
- 5 different types: Linear, Circular, Bar, Image and 3D editor dark-style with 3D-Grid-Floor.  
- Two colors with offset transforms: scale and x,y position for cicrcular type.
- Transform animator to auto rotator with variable speed.  
- Mouse control with wheel.  
- Helpers: randomize colors, transforms, reset, swap colors...  
- Control GUI.  
- Store and handle different presets. WIP  

## Usage

ofApp.h
```.cpp
#include "ofxBackgroundGradient.h"

ofxBackgroundGradient bg;
```

ofApp.cpp
```.cpp
bg.draw();
```

## Dependencies
* ofxGui [Core]
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  

## NOTE
* Uses an fbo to refresh only when updated.  
* Not using any shader. This would be more performant.  

## Tested systems
- **Windows10** / **VS2017** / **OF ~0.11**
- **macOS High Sierra** / **Xcode 9/10** / **OF ~0.11**

## Author
Addon by **@moebiusSurfing**  
*(ManuMolina). 2020.*

## License
*MIT License.*