#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackground(0);

	//font.loadFont("HelveticaNeueLTStd-Md.otf", 12);
	font.loadFont(OF_TTF_SANS, 14);

	ui_button.setTitle("test");
	ui_button.setFont(font);
	ui_button.setID(200);
	ui_button.setButtonStyle(UI_BUTTON_ROUNDED_RECT);

	dropMenu.setPosition(400, 50);
	dropMenu.setWidth(150);
	dropMenu.setHeight(30);

	dropMenu.addMenuItem("Test 1");
	dropMenu.addMenuItem("Test 2");
	dropMenu.addMenuItem("Test 3");


	monoLineTextInput.setup();
	monoLineTextInput.setPlaceholderText("place holder");
	monoLineTextInput.setBounds(80, 100, 188, 40);
	multilineTextInput.setup();

	multilineTextInput.setText("0: #include \"testApp.h\"\n"
		"1: \n"
		"2: //--------------------------------------------------------------\n"
		"3: void testApp::setup() {\n"
		"4: \n"
		"5: monoLineTextInput.setup();\n"
		"6: monoLineTextInput.text = \"hello world\";\n"
		"7: monoLineTextInput.bounds.x = 100;\n"
		"8: monoLineTextInput.bounds.y = 100;\n"
		"9:\n"
		"10: multilineTextInput.setup();\n"
		"11:\n"
		"12: monoLineTextInput.text = \"hello world\";\n"
		"13: monoLineTextInput.bounds.x = 100;\n"
		"14: monoLineTextInput.bounds.y = 100;\n"
		"15:\n"
		"16:\n"
		"17: }");
	multilineTextInput.setBounds(280, 100, 400, 500);
	multilineTextInput.setMultiline(true);

	multilineTextInput.setFont(font);

	but.setup("pictureButton.png", "pictureButtonPressed.png");

	colorPick.setup(RADIAL, 300, 300);
	colorPick2.setup(VERTICAL_HALF_BRIGHTNESS, 300, 300);

	iSlider.setup(UI_SLIDER_VERTICAL, UI_SLIDER_LINE, 50, 0, 100, 40, 160, 30, 100);
	iSlider.setBackgroundColor(ofColor::darkBlue);
	iSlider.setFillColor(ofColor::greenYellow);
	uiSlider.setup(50, 0, 1000, 100, 200);
	fSlider.setup(50, 0, 1, 100, 200);

	ofAddListener(colorPick.pickerEvent, this, &ofApp::pickerSelected);
	ofAddListener(colorPick2.pickerEvent, this, &ofApp::pickerSelected);
	ofAddListener(ui_button.buttonEvent, this, &ofApp::UIButPressed);
	ofAddListener(but.imgButtonEvent, this, &ofApp::UIButPressed);
	ofAddListener(dropMenu.menuEvent, this, &ofApp::UIMenuSelection);
	ofAddListener(iSlider.sliderEvent, this, &ofApp::sliderMoved);
	ofAddListener(fSlider.sliderEvent, this, &ofApp::sliderMoved);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);

	int width = 150;
	int height = 30;

	ui_button.draw(200, 50, width, height);

	ofPushStyle();
	monoLineTextInput.draw();
	multilineTextInput.draw();
	ofPopStyle();

	ofPushStyle();
	colorPick.draw(700, 100);
	colorPick2.draw(700, 410);
	but.draw(30, 300);
	ofPopStyle();

	dropMenu.draw();

	fSlider.draw();
	iSlider.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::UIButPressed(const pair<bool, int> & state) {
	state.first ? cout << "button " + ofToString(state.second) + " pressed down" << endl : cout << "button " + ofToString(state.second) + " depressed" << endl;
}

void ofApp::UIMenuSelection(const pair<string, int> & selection) {
	dropMenu.setButtonTitle(selection.first);
	dropMenu.removeMenuItem(selection.second);
}

void ofApp::pickerSelected(const ofColor & color) {
	ofBackground(color);
}

void ofApp::sliderMoved(const int & sliderID) {
	if(sliderID == iSlider.getID())
		cout << "Slider " + ofToString(sliderID) + " moved with value of: " + ofToString(iSlider.getValue()) << endl;
	if (sliderID == fSlider.getID())
		cout << "Slider " + ofToString(sliderID) + " moved with value of: " + ofToString(fSlider.getValue()) << endl;
}
