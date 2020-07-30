# ofxBackgroundGradient 

## Overview
*ofxBackgroundGradient* is an addon for *openFrameworks* to easy generate different background styles.  

## Screenshot
![image](readme_images/Capture1.PNG?raw=true "Capture1.PNG")  
![image](readme_images/Capture2.PNG?raw=true "Capture2.PNG")  
![image](readme_images/Capture3.PNG?raw=true "Capture3.PNG")  

## Features
- Four different types: Linear, Circular, Bar and 3D editor dark style with grid floor.  
- Two colors with transforms: scale, x,y position for cicrcular type.
- Store and handle different presets.  
- Helpers: randomize colors, transforms, reset, swap colors and mouse control for position/scale.  
- Control GUI.  

## Usage

```.cpp
	#include "ofxBackgroundGradient.h"
    
    ofxBackgroundGradient myBackground;
```

```.cpp
//setup()
    myBackground.setAutoSaveLoad(true);
    myBackground.setup();
    myBackground.setVisibleGui(true);

//draw()
    myBackground.draw();
```

### TODO
-

## Dependencies
- 

## Tested systems
- Windows10 / VS2017 / OF 0.11  

## Author
MoebiusSurfing, July 2020.  

## License
MIT License.