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

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);

	void setUpdateOnReleaseOnly(bool bUpdateOnReleaseOnly) { this->bUpdateOnReleaseOnly = bUpdateOnReleaseOnly; }

	void setBackgroundColor(ofColor c) { this->bgc = c; generateDraw(); }
	void setFillColor(ofColor c) { this->fgc = c; generateDraw(); }

	void voidSetBarWidth(int width) { barWidth = width; generateDraw(); }

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method) {
		value.addListener(listener, method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method) {
		value.removeListener(listener, method);
	}

	void draw();
	void draw(int x, int y);
	void draw(int x, int y, int width, int height);

	double operator=(T v);
	operator const T & ();



	ofAbstractParameter & getParameter();

protected:
	ofColor bgc, fgc;
	ofxUISliderType type;
	ofxUISliderDirection dir;
	ofParameter<T> value;
	bool bUpdateOnReleaseOnly;
	bool setValue(float mx, float my);
	void generateDraw();
	void valueChanged(T & value);
	ofPath bg, bar;
	ofRectangle sliderRect;
	int barWidth;
};

typedef ofxUISlider<float> ofxUIFloatSlider;
typedef ofxUISlider<int> ofxUIIntSlider;
