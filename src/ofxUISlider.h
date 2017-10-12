#pragma once

#include "ofMain.h"
#include "ofParameter.h"

enum ofxUISliderType {
	UI_SLIDER_BAR,
	UI_SLIDER_PROGRESS_BAR,
	UI_SLIDER_LINE,
	UI_SLIDER_RADIAL
};

enum ofxUISliderDirection {
	UI_SLIDER_VERTICAL,
	UI_SLIDER_HORIZONTAL
};

template<typename T>
class ofxUISlider {
public:
	ofxUISlider();
	~ofxUISlider();
	ofxUISlider(ofParameter<T> _val, int x, int y, float width = 100, float height = 30);
	void setup(T initialVal, T _min, T _max, int x = 0, int y = 0, float width = 100, float height = 30);
	void setup(ofxUISliderType _type, T initialVal, T _min, T _max, int x = 0, int y = 0, float width = 100, float height = 30);
	void setup(ofxUISliderDirection _dir, T initialVal, T _min, T _max, int x = 0, int y = 0, float width = 100, float height = 30);
	void setup(ofxUISliderDirection _dir, ofxUISliderType _type, T initialVal, T _min, T _max, int x = 0, int y = 0, float width = 100, float height = 30);

	void setMin(T min);
	T getMin();
	void setMax(T max);
	T getMax();

	void setUpdateOnReleaseOnly(bool bUpdate) { this->bUpdateOnReleaseOnly = bUpdate; }

	void setBackgroundColor(ofColor c) { this->bgc = c; generateDraw(); }
	void setFillColor(ofColor c) { this->fgc = c; generateDraw(); }

	void voidSetBarWidth(int width) { barWidth = width; generateDraw(); }

	ofEvent<const int> sliderEvent;

	void draw();
	void draw(int x, int y);
	void draw(int x, int y, int width, int height);

	double operator=(T v);
	operator const T & ();

	void setID(int newId) { ID = newId; }
	int getID() { return ID; }

	void setValue(T newVal) { value = newVal; }
	T getValue() { return value; }

	void mouseMoved(ofMouseEventArgs & args) {}
	void mousePressed(ofMouseEventArgs & args);
	void mouseDragged(ofMouseEventArgs & args);
	void mouseReleased(ofMouseEventArgs & args);
	void mouseScrolled(ofMouseEventArgs & args) {}
	void mouseEntered(ofMouseEventArgs & args) {}
	void mouseExited(ofMouseEventArgs & args) {}

private:

	ofColor bgc, fgc;
	ofxUISliderType type;
	ofxUISliderDirection dir;
	ofParameter<T> value;
	bool bUpdateOnReleaseOnly;
	void setValue(float mx, float my);
	void generateDraw();
	void valueChanged(T & value);
	ofPath bg, bar;
	ofRectangle sliderRect;
	int barWidth, ID;
};

typedef ofxUISlider<float> ofxUIFloatSlider;
typedef ofxUISlider<int> ofxUIIntSlider;
