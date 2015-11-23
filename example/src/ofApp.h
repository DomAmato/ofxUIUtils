#pragma once

#include "ofMain.h"
#include "ofxUIUtils.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		//	Buttons
		ofxUIButton ui_button;
		ofxImgButton but;
		void UIButPressed(const pair<bool, int> & state);

		//	Dropdown Menu
		ofxDropDownMenu dropMenu;
		void UIMenuSelection(const pair<string, int> & selection);

		//	Color Picker
		ofxColorPicker colorPick;
		ofxColorPicker colorPick2;
		void pickerSelected(const ofColor & color);

		//	Text Fields 
		ofxTextInputField monoLineTextInput, multilineTextInput;
		ofTrueTypeFont font;

		//Sliders
		ofxUIIntSlider iSlider;
		ofxUISlider<unsigned int> uiSlider;
		ofxUIFloatSlider fSlider;
		void sliderMoved(const int & sliderID);
};
