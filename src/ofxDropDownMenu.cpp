#include "ofxDropDownMenu.h"

ofxDropDownMenu::ofxDropDownMenu(){

	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
	//font.loadFont(OF_TTF_SERIF, 12);
	mainPanel = new ofxUIButton();
	selection = "-- Menu --";
	mainPanel->setTitle(selection);
	mainPanel->setToggleMode(true);
	//mainPanel->setFont(font);
	toggled = false;
	toggleTimer = 0;
	IDs = 0;
	_ID = 0;
	_cellHeight = 25;
	ofAddListener(mainPanel->buttonEvent, this, &ofxDropDownMenu::UIButPressed);
}

void ofxDropDownMenu::draw(){

	ofPushStyle();
	int x, y;
	mainPanel->draw();

	if (toggled){
		ofPushMatrix();
		ofTranslate(0, 0, 1);
		x = menuPos.x;
		y = menuPos.y;

		ofSetColor(225, 240);
		ofRect(x - 5, y - 5, _width, _height);

		int color_forText = 0;

		for (int i = 0; i < menuItems.size(); i++) {
			if (menuItems[i].isActive()){
				menuItems[i].setTextColor(ofColor::white);
			}
			else {
				menuItems[i].setTextColor(ofColor::black);
			}
			menuItems[i].draw(x - 5, i*_cellHeight + y, _width, _cellHeight);

		}
		ofTranslate(0, 0, -1);
	//	glDisable(GL_DEPTH_TEST);
		ofPopMatrix();
	}
	
	ofPopStyle();
}

ofxDropDownMenu::~ofxDropDownMenu(){

	ofUnregisterMouseEvents(this);
	ofUnregisterKeyEvents(this);
}

void ofxDropDownMenu::addMenuItem(string name){

	ofxUIMenuItem temp;
	temp.setTitle(name);
	temp.setVisible(true);
	temp.setID(IDs++);
	menuItems.push_back(temp);
	if (_width < ofBitmapStringGetBoundingBox(name, 0, 0).width)
		_width = ofBitmapStringGetBoundingBox(name, 0, 0).width + 10;
	_height = (_cellHeight * menuItems.size() + 5);
}

void ofxDropDownMenu::removeMenuItem(string name){
	for (int i = 0; i < menuItems.size(); i++){
		if (menuItems[i].getTitle() == name)
			menuItems.erase(menuItems.begin() + i);
	}
}
void ofxDropDownMenu::removeMenuItem(int ID){
	for (int i = 0; i < menuItems.size(); i++){
		if (menuItems[i].getID() == ID)
			menuItems.erase(menuItems.begin() + i);
	}
}
void ofxDropDownMenu::clearMenuItems(){
	menuItems.clear();
}

void ofxDropDownMenu::mouseReleased(ofMouseEventArgs& eventArgs){

	if (eventArgs.button == 0 && toggled){
		for (int i = 0; i < menuItems.size(); i++) {
			if (menuItems[i].isActive()){
				toggleTimer = ofGetElapsedTimeMillis();
				selection = menuItems[i].getTitle();
				pair<string, int> temp(selection, _ID);
				ofNotifyEvent(menuEvent, temp, this);
			}
		}
		mainPanel->setToggle(false);
		toggled = false;
	}
}

void ofxDropDownMenu::UIButPressed(const pair<bool, int> & state){

	if (state.first && !toggled && ofGetElapsedTimeMillis() - toggleTimer > 500){
		selection = mainPanel->getTitle();
		toggled = true;
		menuPos.x = ofGetMouseX();
		if (menuPos.x + _width > ofGetWidth())
			menuPos.x = ofGetWidth() - (_width + 5);
		menuPos.y = ofGetMouseY();
		if (menuPos.y + _height > ofGetHeight())
			menuPos.y = ofGetHeight() - (_height + 5);
	}
	else {
		toggled = false;
		mainPanel->setToggle(false);
	}
}