#include "ofxDropDownMenu.h"

ofxDropDownMenu::ofxDropDownMenu() {

	ofRegisterMouseEvents(this);
	ofRegisterKeyEvents(this);
	mainPanel = new ofxUIButton();
	selection = "-- Menu --";
	mainPanel->setTitle(selection);
	mainPanel->setToggleMode(true);
	mainPanel->setPosition(0, 0);
	//evidently if this isn't set it can sometimes never be initialized?
	mainPanel->setWidth(30);
	toggled = false;
	toggleTimer = 0;
	IDs = 0;
	_cellHeight = 25;
	ofAddListener(mainPanel->buttonEvent, this, &ofxDropDownMenu::UIButPressed);
}

void ofxDropDownMenu::draw() {
	ofPushStyle();
	ofPushMatrix();
	int x, y;
	mainPanel->draw();

	if (toggled) {
		ofPushMatrix();
		ofTranslate(0, 0, 1);
		x = menuPos.x;
		y = menuPos.y;

		ofSetColor(225, 240);
		/*glEnable(GL_SCISSOR_TEST);
		glScissor(x - 5, y - 5, _width, _cellHeight * 5);*/

		ofRect(x - 5, y - 5, _width, _cellHeight*MIN(5, menuItems.size()) + 5);

		int color_forText = 0;

		int startIndex = 0;
		if (menuItems.size() > 5) {
			startIndex = MIN(floor(menuItems.size() * verticalOffset), menuItems.size()-5);
		} 
		for (int i = startIndex; i-startIndex < MIN(5, menuItems.size()-startIndex); i++) {
			if (menuItems[i].isActive()) {
				menuItems[i].setTextColor(ofColor::white);
			}
			else {
				menuItems[i].setTextColor(ofColor::black);
			}
			menuItems[i].draw(x - 5, (i - startIndex)*_cellHeight + y, _width, _cellHeight, mainPanel->getFontRenderer());

		}
		//glDisable(GL_SCISSOR_TEST);
		ofTranslate(0, 0, -1);
		ofPopMatrix();
	}
	ofPopMatrix();
	ofPopStyle();
}

ofxDropDownMenu::~ofxDropDownMenu() {

	ofUnregisterMouseEvents(this);
	ofUnregisterKeyEvents(this);
}

void ofxDropDownMenu::addMenuItem(string name) {

	ofxUIMenuItem temp;
	temp.setTitle(name);
	temp.setVisible(true);
	temp.setID(IDs++);
	menuItems.push_back(temp);
	//ofBitmapFont bFont;
	/*if (_width < bFont.getBoundingBox(name, 0, 0).width)
		_width = bFont.getBoundingBox(name, 0, 0).width + 10;*/

	if (_width < mainPanel->getFontRenderer()->stringWidth(name)) {
		_width = mainPanel->getFontRenderer()->stringWidth(name) + 10;
	}
	_height = (_cellHeight * menuItems.size() + 5);
}

void ofxDropDownMenu::removeMenuItem(string name) {
	for (int i = 0; i < menuItems.size(); i++) {
		if (menuItems[i].getTitle() == name) {
			menuItems.erase(menuItems.begin() + i);
			_height = (_cellHeight * menuItems.size() + 5);
		}
	}
}
void ofxDropDownMenu::removeMenuItem(int ID) {
	for (int i = 0; i < menuItems.size(); i++) {
		if (menuItems[i].getID() == ID) {
			menuItems.erase(menuItems.begin() + i);
			_height = (_cellHeight * menuItems.size() + 5);
		}
	}
}
void ofxDropDownMenu::clearMenuItems() {
	menuItems.clear();
}

void ofxDropDownMenu::mouseReleased(ofMouseEventArgs& eventArgs) {

	if (eventArgs.button == 0 && toggled) {
		int startIndex = 0;
		if (menuItems.size() > 5) {
			startIndex = MIN(floor(menuItems.size() * verticalOffset), menuItems.size() - 5);
		}
		for (int i = startIndex; i - startIndex < MIN(5, menuItems.size() - startIndex); i++) {
			if (menuItems[i].isActive()) {
				toggleTimer = ofGetElapsedTimeMillis();
				selection = menuItems[i].getTitle();
				tuple<int, string, int> temp(getID(), selection, menuItems[i].getID());
				ofNotifyEvent(menuEvent, temp, this);
			}
		}
		mainPanel->setToggle(false);
		toggled = false;
	}
}

void ofxDropDownMenu::mouseScrolled(ofMouseEventArgs & args) {
	verticalOffset = ofClamp(verticalOffset+args.scrollY/20, 0, 1);

}

void ofxDropDownMenu::UIButPressed(const pair<bool, int> & state) {

	if (state.first && !toggled && ofGetElapsedTimeMillis() - toggleTimer > 500) {
		selection = mainPanel->getTitle();
		toggled = true;
		menuPos.x = ofGetMouseX();
		if (menuPos.x + _width > ofGetWidth())
			menuPos.x = ofGetWidth() - (_width + 5);
		menuPos.y = ofGetMouseY();
		if (menuPos.y + _cellHeight * 5 > ofGetHeight())
			menuPos.y = ofGetHeight() - (_cellHeight * 5 + 5);
	}
	else {
		toggled = false;
		mainPanel->setToggle(false);
	}
}