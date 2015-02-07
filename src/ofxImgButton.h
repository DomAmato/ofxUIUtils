#pragma once

#include "ofParameter.h"
#include "ofMain.h"

class ofxImgButton{
	
public:
	ofxImgButton();
	~ofxImgButton();
	void setup(string imageName, float width = -1, float height = -1);
	void setup(string imgName, string imgToggleName, float width = -1, float height = -1);
	void draw(int x, int y, int w, int h);
	void draw(int x, int y);
	void draw(ofPoint p);
	void draw();
	void setImage(string imageName);
	void setToggleImage(string imageName);
	void setWidth(int w);
	void setHeight(int h);
	float getWidth();
	float getHeight();
	void setWidthPercent(float w);
	void setHeightPercent(float h);
	void setPosition(int x, int y);
	void setPosition(ofPoint p);
	void setToggle(bool state);


	ofRectangle button;
	bool		isToggle;
	bool		value;
	ofImage		buttonImg;
	ofImage		buttonImgToggle;

	void mouseReleased(ofMouseEventArgs & args);
	void mouseMoved(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);

	ofEvent<const bool> buttonEvent;

};
