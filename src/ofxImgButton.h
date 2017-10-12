#pragma once

#include "ofParameter.h"
#include "ofMain.h"
#include "ofxUIButton.h"

class ofxImgButton : public ofxUIButton {

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
	void setWidthPercent(float w){ button.width *= w; }
	void setHeightPercent(float h){ button.height *= h; }
	void setToggle(bool state){ toggle = state; }
	void setToggleMode(bool mode) { togglable = mode; }


	float getWidth() { return button.width; }
	float getHeight(){ return button.height; }

	void setClickable(bool mode) { clickable = mode; }

	void mouseReleased(ofMouseEventArgs & args);
	void mouseMoved(ofMouseEventArgs & args);
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args) {}
	void mouseScrolled(ofMouseEventArgs & args) {}
	void mouseEntered(ofMouseEventArgs & args) {}
	void mouseExited(ofMouseEventArgs & args) {}

private:
	ofImage		buttonImg;
	ofImage		buttonImgToggle;
};
