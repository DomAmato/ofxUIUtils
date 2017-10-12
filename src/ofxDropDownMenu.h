#pragma once

#include "ofMain.h"
#include "ofxUIButton.h"
#include "ofParameter.h"
#include "ofBitmapFont.h"


class ofxUIMenuItem {
private:
	bool isActive_;
	float dist, timer, timerDir;
	bool bVisible;

	ofColor color, textColor;
	bool clickable;

	int ID;
	string title;

public:


	ofxUIMenuItem(){

		bVisible = false;
		clickable = true;
		isActive_ = false;
		timer = 0;
		timerDir = ofRandom(1, -1);
		ID = 0;
		title = "";
		color.set(100, 100, 245);
		textColor = ofColor::black;
		dist = 0;

	}
	~ofxUIMenuItem(){


	}

	void draw(int x, int y, int w, int h, ofxUIUtils::FontRenderer * renderer){
		ofPushStyle();
		if (clickable){
			dist = ofDist(x + (w / 2), 0, ofGetMouseX(), 0);
			float    disty = ofDist(0, y + (h / 2), 0, ofGetMouseY());
			if (dist < w / 2 && disty < h / 2)
				isActive_ = true;
			else
				isActive_ = false;
		}
		if (isActive_){
			ofSetColor(color);
			ofRect(x, y, w, h);
		}
		ofSetColor(textColor);
		renderer->drawString(title, x + 5, y + (h / 2) + (h / 4));
		//ofDrawBitmapString(title, x + 5, y + (h / 2) + (h / 4));

		ofSetColor(0, 255);
		ofSetLineWidth(.25);
		ofLine(x, y + h, x + w, y + h);
		ofPopStyle();
	}

	bool isActive(){ return isActive_; }
	void setColor(int r, int g, int b, int al = 255){ color.set(r, g, b, al); }
	void setColor(ofColor c){ color.set(c); }
	void setTextColor(int r, int g, int b, int al = 255){ textColor.set(r, g, b, al); }
	void setTextColor(ofColor c){ textColor.set(c); }
	void setClickable(bool state){ clickable = state; }
	void setID(int newId) { ID = newId; }
	void setTitle(string newTitle) { title = newTitle; }
	void setVisible(bool isVisible) { bVisible = isVisible; }

	int getID() { return ID; }
	string getTitle() { return title; }
	ofColor getColor() { return color; }
	ofColor getTextColor() { return textColor; }
};

class ofxDropDownMenu{

public:

	ofxDropDownMenu();
	~ofxDropDownMenu();

	void setButton(ofxUIButton * but){ mainPanel = but; }
	void setPosition(ofPoint p){ setPosition(p.x, p.y); }
	void setPosition(int x, int y){ mainPanel->setPosition(x, y); }
	void setWidth(float width){ mainPanel->setWidth(width); }
	void setHeight(float height){ mainPanel->setHeight(height); }
	void setButtonFont(ofTrueTypeFont &newFont) { mainPanel->setFont(newFont); }
	void setButtonTitle(string name){ mainPanel->setTitle(name); }
	void setButtonColor(ofColor c) { mainPanel->setColor(c); }
	void setButtonColor(int r, int g, int b, int a = 255) { mainPanel->setColor(r, g, b, a); }
	void setButtonHoverColor(ofColor c) { mainPanel->setHoverColor(c); }
	void setButtonHoverColor(int r, int g, int b, int a = 255) { mainPanel->setHoverColor(r, g, b, a); }
	void setButtonTextColor(ofColor c) { mainPanel->setTextColor(c); }
	void setButtonTextColor(int r, int g, int b, int a = 255) { mainPanel->setTextColor(r, g, b, a); }
	void setID(int newID){ _ID = newID; }
	void setAutoSizing(bool state) { mainPanel->buttonAutoSizes(state); }

	void draw();

	void mouseReleased(ofMouseEventArgs& eventArgs);
	void mouseMoved(ofMouseEventArgs & args){}
	void mouseDragged(ofMouseEventArgs & args){}
	void mousePressed(ofMouseEventArgs & args){}
	void mouseScrolled(ofMouseEventArgs & args);
	void mouseEntered(ofMouseEventArgs & args) {}
	void mouseExited(ofMouseEventArgs & args) {}

	void keyPressed(ofKeyEventArgs& eventArgs){ toggled = false; }
	void keyReleased(ofKeyEventArgs& eventArgs) {}

	void addMenuItem(string name);
	void removeMenuItem(string name);
	void removeMenuItem(int ID);
	void clearMenuItems();

	void Hide(){ toggled = false; }
	string getSelection() { return selection; }
	bool getToggled() { return toggled; }
	void setToggled(bool mode) { toggled = mode; mainPanel->setToggle(mode); }
	int getID() { return _ID; }
	int getHeight() { return _height; }
	int getWidth() { return _width; }
	ofPoint getPosition() { return mainPanel->getPosition(); }

	ofEvent<const tuple<int, string, int> > menuEvent;

	void UIButPressed(const pair<bool, int> & state);

	double verticalOffset;

private:
	ofTrueTypeFont font;
	bool toggled;
	vector<ofxUIMenuItem> menuItems;
	ofxUIButton * mainPanel;
	ofPoint menuPos;
	string selection;
	int IDs, _ID;
	long toggleTimer;
	int _width, _height, _cellHeight;
};



