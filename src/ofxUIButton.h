#pragma once

#include "ofMain.h"
#include "ofParameter.h"
#include "ofxUIFontRenderer.h"

enum UI_Button_Style{
	UI_BUTTON_RECT,
	UI_BUTTON_ROUNDED_RECT,
	UI_BUTTON_ELLIPSE
};

class ofxUIButton {

public:
	ofxUIButton();
	~ofxUIButton();

	void setColor(ofColor c){ color.set(c); }
	void setColor(int r, int g, int b, int al = 255){ color.set(r, g, b, al); }
	void setHoverColor(int r, int g, int b, int al = 255){ hoverColor.set(r, g, b, al); }
	void setHoverColor(ofColor c){ hoverColor.set(c); }
	void setTextColor(int r, int g, int b, int al = 255){ textColor.set(r, g, b, al); }
	void setTextColor(ofColor c){ textColor.set(c); }
	void setPosition(ofPoint p){ setPosition(p.x, p.y); }
	void setPosition(int x, int y){ _x = x; _y = y; }
	void setWidth(float width){ _w = width; }
	void setHeight(float height){ _h = height; }
	void setClickable(bool state){ clickable = state; }
	void setID(int newId) { ID = newId; }
	void setTitle(string newTitle) { title = newTitle; if (_autoSize) _w = 0; }
	void setVisible(bool state) { visible = state; }
	void setToggleMode(bool mode){ togglable = mode; }
	void setToggle(bool mode){ toggle = mode; }
	void setButtonStyle(UI_Button_Style style){ buttonStyle = style; }
	void setRadius(float r) {}

	void buttonAutoSizes(bool state) { _autoSize = state; }

	int getID() { return ID; }
	string getTitle() { return title; }
	bool getVisible() { return visible; }
	bool getToggleMode(){ return togglable; }
	ofPoint getPosition(){ return ofPoint(_x, _y); }

	void mouseMoved(ofMouseEventArgs & args); 
	void mouseDragged(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs& eventArgs);

	ofEvent<const pair<bool, int>> buttonEvent;

	void draw(int x, int y, int w, int h, ofColor textCol = ofColor::black){ _x = x; _y = y; _w = w; _h = h; draw(); }
	void draw(int x, int y) { _x = x; _y = y; draw(); }
	void draw(ofPoint p) { _x = p.x; _y = p.y; draw(); }
	void draw();

	/// Change the font used to draw the text
	void setFont(OFX_TEXTFIELD_FONT_RENDERER & font);
	ofxUIUtils::FontRenderer * getFontRenderer();


private:
	ofxUIUtils::FontRenderer* fontRef;

	UI_Button_Style buttonStyle;

	int _x, _y;
	float _w, _h, _r, dist;

	bool visible, hovering, togglable, toggle, clickable, _autoSize;
	ofColor color, hoverColor, textColor;

	int ID;
	string title;
};