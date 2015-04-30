#include "ofxDropDownMenu.h"

ofxDropDownMenu::ofxDropDownMenu(){

	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
	font.loadFont(OF_TTF_SERIF, 18);
	mainPanel = new ofxUIButton();
	selection = "-- Menu --";
	mainPanel->setTitle(selection);
	mainPanel->setToggleMode(true);
	mainPanel->setFont(font);
	toggled = false;
	toggleTimer = 0;
	IDs = 0;
	ofAddListener(mainPanel->buttonEvent, this, &ofxDropDownMenu::UIButPressed);
}

void ofxDropDownMenu::draw(){

	ofPushStyle();
	int x, y, w, h, cellHeight;
	cellHeight = 25;
	mainPanel->draw();

	if (toggled){
		x = menuPos.x;
		y = menuPos.y;
		w = 100;
		h = cellHeight*menuItems.size() + 5;

		ofSetColor(225, 240);
		ofRect(x - 5, y - 5, w, h);

		int color_forText = 0;

		for (int i = 0; i < menuItems.size(); i++) {
			if (menuItems[i].isActive()){
				menuItems[i].setTextColor(ofColor::white);
			}
			else {
				menuItems[i].setTextColor(ofColor::black);
			}
			menuItems[i].draw(x - 5, (i*cellHeight) + y, w, cellHeight);

		}
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
				pair<string, int> temp(selection, menuItems[i].getID());
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
		menuPos.y = ofGetMouseY();
	}
	else {
		toggled = false;
		mainPanel->setToggle(false);
	}
}