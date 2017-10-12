#pragma once

#include "ofMain.h"
#include "ofParameter.h"
#include "ofxUIFontRenderer.h"

enum UI_Button_Style {
	UI_BUTTON_RECT,
	UI_BUTTON_ROUNDED_RECT,
	UI_BUTTON_ELLIPSE
};

class ofxUIButton {

public:
	ofxUIButton();
	~ofxUIButton();

	void setColor(ofColor c) { color.set(c); }
	void setColor(int r, int g, int b, int al = 255) { color.set(r, g, b, al); }
	void setEdgeColor(ofColor c) { edgeColor.set(c); }
	void setEdgeColor(int r, int g, int b, int al = 255) { edgeColor.set(r, g, b, al); }
	void setHoverColor(int r, int g, int b, int al = 255) { hoverColor.set(r, g, b, al); }
	void setHoverColor(ofColor c) { hoverColor.set(c); }
	void setTextColor(int r, int g, int b, int al = 255) { textColor.set(r, g, b, al); }
	void setTextColor(ofColor c) { textColor.set(c); }
	void setPosition(ofPoint p) { setPosition(p.x, p.y); }
	void setPosition(int x, int y) { button.x = x; button.y = y; }
	void setWidth(float width) { button.width = width; }
	void setHeight(float height) { button.height = height; }
	void setClickable(bool state) { clickable = state; }
	void setID(int newId) { ID = newId; }
	void setTitle(string newTitle) { title = newTitle; if (_autoSize) button.width = 0; }
	void setVisible(bool state) { visible = state; }
	void setToggleMode(bool mode) { togglable = mode; }
	void setToggle(bool mode) { toggle = mode; }
	void setButtonStyle(UI_Button_Style style) { buttonStyle = style; }
	void setRadius(float r) { _r = r; }
	void setDoesDrawEdge(bool state) { hasEdge = state; }
	void setEdgeWidth(float width) { edgeWidth = width; }

	void buttonAutoSizes(bool state) { _autoSize = state; }

	int getID() { return ID; }
	string getTitle() { return title; }
	bool getVisible() { return visible; }
	bool getToggleMode() { return togglable; }
	ofPoint getPosition() { return ofPoint(button.x, button.y); }
	bool getIsHovering() { return hovering; }
	ofRectangle getBoundingRect() { return button; }

	void mouseMoved(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args) {}
	void mousePressed(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs& eventArgs);
	void mouseScrolled(ofMouseEventArgs & args) {}
	void mouseEntered(ofMouseEventArgs & args) {}
	void mouseExited(ofMouseEventArgs & args) {}

	ofEvent<const pair<bool, int> > buttonEvent;

	void draw(int x, int y, int w, int h, ofColor textCol = ofColor::black) { button.x = x; button.y = y; button.width = w; button.height = h; draw(); }
	void draw(int x, int y) { button.x = x; button.y = y; draw(); }
	void draw(ofPoint p) { button.x = p.x; button.y = p.y; draw(); }
	void draw();

	/// Change the font used to draw the text
	void setFont(OFX_TEXTFIELD_FONT_RENDERER & font);
	ofxUIUtils::FontRenderer * getFontRenderer();


protected:
	ofxUIUtils::FontRenderer * fontRef;

	UI_Button_Style buttonStyle;

	ofRectangle button;
	float  _r, edgeWidth;

	bool visible, hovering, togglable, toggle, clickable, hasEdge, _autoSize;
	ofColor color, hoverColor, textColor, edgeColor;

	int ID;
	string title;
};