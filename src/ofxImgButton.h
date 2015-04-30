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

	void setImage(string imageName) { buttonImg.loadImage(imageName); }
	void setToggleImage(string imageName){ buttonImgToggle.loadImage(imageName); }
	void setWidth(int w) { button.width = w; }
	void setHeight(int h) { button.height = h; }
	void setID(int newId) { ID = newId; }
	void setWidthPercent(float w){ button.width *= w; }
	void setHeightPercent(float h){ button.height *= h; }
	void setPosition(int x, int y){
		button.x = x;
		button.y = y;
	}
	void setPosition(ofPoint p) { setPosition(p.x, p.y); }
	void setToggle(bool state){ value = state; }

	float getWidth() { return button.width; }
	float getHeight(){ return button.height; }



	void mouseReleased(ofMouseEventArgs & args);
	void mouseMoved(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);

	ofEvent<const pair<bool, int>> imgButtonEvent;

private:

	ofRectangle button;
	bool		isToggle;
	bool		value;
	ofImage		buttonImg;
	ofImage		buttonImgToggle;

	int			ID;



};
