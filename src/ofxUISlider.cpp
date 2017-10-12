#include "ofxUISlider.h"

template<typename T>
ofxUISlider<T>::ofxUISlider() {
	bUpdateOnReleaseOnly = false;
	bgc = ofColor::darkGray;
	fgc = ofColor::lightGrey;
	type = UI_SLIDER_BAR;
	dir = UI_SLIDER_HORIZONTAL;
	barWidth = 16;
	ID = ofRandom(1000);
}

template<typename T>
ofxUISlider<T>::~ofxUISlider() {
}

template<typename T>
ofxUISlider<T>::ofxUISlider(ofParameter<T> _val, int x, int y, float width, float height) {
	bUpdateOnReleaseOnly = false;
	bgc = ofColor::darkGray;
	fgc = ofColor::lightGrey;
	type = UI_SLIDER_BAR;
	dir = UI_SLIDER_HORIZONTAL;
	barWidth = 16;
	setup(_val, x, y, width, height);
}

template<typename T>
void ofxUISlider<T>::setup(T _val, T _min, T _max, int x, int y, float width, float height) {
	value.set("", _val, _min, _max);
	if (dir == UI_SLIDER_HORIZONTAL && width < height) {
		dir = UI_SLIDER_VERTICAL;
	}
	else if (dir == UI_SLIDER_VERTICAL && width > height) {
		dir = UI_SLIDER_HORIZONTAL;
	}
	bUpdateOnReleaseOnly = false;
	sliderRect.x = x;
	sliderRect.y = y;
	sliderRect.width = width;
	sliderRect.height = height;

	value.addListener(this, &ofxUISlider::valueChanged);
	ofRegisterMouseEvents(this);
	generateDraw();
}

template<typename T>
void ofxUISlider<T>::setup(ofxUISliderType _type, T initialVal, T _min, T _max, int x, int y, float width, float height)
{
	type = _type;
	setup(initialVal, _min, _max, x, y, width, height);
}

template<typename T>
void ofxUISlider<T>::setup(ofxUISliderDirection _dir, T initialVal, T _min, T _max, int x, int y, float width, float height)
{
	dir = _dir;
	setup(initialVal, _min, _max, x, y, width, height);
}

template<typename T>
void ofxUISlider<T>::setup(ofxUISliderDirection _dir, ofxUISliderType _type, T initialVal, T _min, T _max, int x, int y, float width, float height)
{
	type = _type;
	dir = _dir;
	setup(initialVal, _min, _max, x, y, width, height);
}

template<typename T>
void ofxUISlider<T>::setMin(T min) {
	value.setMin(min);
}

template<typename T>
T ofxUISlider<T>::getMin() {
	return value.getMin();
}

template<typename T>
void ofxUISlider<T>::setMax(T max) {
	value.setMax(max);
}

template<typename T>
T ofxUISlider<T>::getMax() {
	return value.getMax();
}

template<typename T>
void ofxUISlider<T>::mousePressed(ofMouseEventArgs & args) {
	if (bUpdateOnReleaseOnly) {
		value.disableEvents();
	}
	setValue(args.x, args.y);
}

template<typename T>
void ofxUISlider<T>::mouseDragged(ofMouseEventArgs & args) {
	if (!bUpdateOnReleaseOnly) {
		setValue(args.x, args.y);
	}
}

template<typename T>
void ofxUISlider<T>::mouseReleased(ofMouseEventArgs & args) {
	if (bUpdateOnReleaseOnly) {
		value.enableEvents();
	}
	setValue(args.x, args.y);
}

template<typename T>
void ofxUISlider<T>::draw()
{
	bg.draw();
	bar.draw();
}

template<typename T>
void ofxUISlider<T>::draw(int x, int y)
{
	if (sliderRect.x != x || sliderRect.y != y) {
		sliderRect.x = x;
		sliderRect.y = y;
		generateDraw();
	}
	draw();
}

template<typename T>
void ofxUISlider<T>::draw(int x, int y, int width, int height)
{
	if (sliderRect.x != x || sliderRect.y != y || sliderRect.width != width || sliderRect.height != height) {
		sliderRect.x = x;
		sliderRect.y = y;
		sliderRect.width = width;
		sliderRect.height = height;
		generateDraw();
	}
	draw();
}

template<typename T>
double ofxUISlider<T>::operator=(T v) {
	value = v;
	return v;
}

template<typename T>
ofxUISlider<T>::operator const T & () {
	return value;
}

template<typename T>
void ofxUISlider<T>::generateDraw() {
	bg.clear();
	bar.clear();

	bg.setColor(bgc);
	bg.setFilled(true);
	if (type == UI_SLIDER_BAR || type == UI_SLIDER_PROGRESS_BAR)
		bg.rectangle(sliderRect);
	else if (type == UI_SLIDER_LINE) {
		if (dir == UI_SLIDER_HORIZONTAL) {
			bg.rectangle(sliderRect.x, sliderRect.y + sliderRect.height / 2 - 1, sliderRect.width, 3);
		}
		else if (dir == UI_SLIDER_VERTICAL) {
			bg.rectangle(sliderRect.x + sliderRect.width / 2 - 1, sliderRect.y, 3, sliderRect.height);
		}
	}


	if (type == UI_SLIDER_PROGRESS_BAR) {
		if (dir == UI_SLIDER_HORIZONTAL) {
			float valAsPct = ofMap(value, value.getMin(), value.getMax(), 0, sliderRect.width - 2, true);
			bar.setColor(fgc);
			bar.setFilled(true);
			bar.rectangle(sliderRect.x + 1, sliderRect.y + 1, valAsPct, sliderRect.height - 2);
		}
		else if (dir == UI_SLIDER_VERTICAL) {
			float valAsPct = ofMap(value, value.getMin(), value.getMax(), 0, sliderRect.height - 2, true);
			bar.setColor(fgc);
			bar.setFilled(true);
			bar.rectangle(sliderRect.x + 1, sliderRect.y + 1, sliderRect.width - 2, valAsPct);
		}
	}
	if (type == UI_SLIDER_BAR) {
		if (dir == UI_SLIDER_HORIZONTAL) {
			float valAsPct = ofMap(value, value.getMin(), value.getMax(), barWidth / 2, sliderRect.width - barWidth / 2, true);
			bar.setColor(fgc);
			bar.setFilled(true);
			bar.rectangle(sliderRect.x + valAsPct - barWidth / 2, sliderRect.y + 1, barWidth, sliderRect.height - 2);
		}
		else if (dir == UI_SLIDER_VERTICAL) {
			float valAsPct = ofMap(value, value.getMin(), value.getMax(), barWidth / 2, sliderRect.height - barWidth / 2, true);
			bar.setColor(fgc);
			bar.setFilled(true);
			bar.rectangle(sliderRect.x + 1, sliderRect.y + valAsPct - barWidth / 2, sliderRect.width - 2, barWidth);
		}
	}
	if (type == UI_SLIDER_LINE) {
		if (dir == UI_SLIDER_HORIZONTAL) {
			float valAsPct = ofMap(value, value.getMin(), value.getMax(), barWidth / 2, sliderRect.width - barWidth / 2, true);
			bar.setColor(fgc);
			bar.setFilled(true);
			bar.triangle(ofPoint(sliderRect.x + valAsPct, sliderRect.y + sliderRect.height),
				ofPoint(sliderRect.x + valAsPct - barWidth / 2, sliderRect.y), ofPoint(sliderRect.x + valAsPct + barWidth / 2, sliderRect.y));
		}
		else if (dir == UI_SLIDER_VERTICAL) {
			float valAsPct = ofMap(value, value.getMin(), value.getMax(), barWidth / 2, sliderRect.height - barWidth / 2, true);
			bar.setColor(fgc);
			bar.setFilled(true);
			bar.triangle(ofPoint(sliderRect.x, sliderRect.y + valAsPct), ofPoint(sliderRect.x + sliderRect.width, sliderRect.y + valAsPct - barWidth / 2),
				ofPoint(sliderRect.x + sliderRect.width, sliderRect.y + valAsPct + barWidth / 2));
		}
	}


}

template<typename T>
void ofxUISlider<T>::setValue(float mx, float my) {
	if (sliderRect.inside(mx, my)) {
		if (dir == UI_SLIDER_HORIZONTAL)
			value = ofMap(mx, sliderRect.x + 1, sliderRect.x + sliderRect.width - barWidth, value.getMin(), value.getMax(), true);
		else if (dir == UI_SLIDER_VERTICAL)
			value = ofMap(my, sliderRect.y + 1, sliderRect.y + sliderRect.height - barWidth, value.getMin(), value.getMax(), true);
		ofNotifyEvent(sliderEvent, ID, this);
	}
}

template<typename T>
void ofxUISlider<T>::valueChanged(T & value) {
	generateDraw();
}

template class ofxUISlider<int>;
template class ofxUISlider<unsigned int>;
template class ofxUISlider<float>;
template class ofxUISlider<double>;
template class ofxUISlider<signed char>;
template class ofxUISlider<unsigned char>;
template class ofxUISlider<unsigned short>;
template class ofxUISlider<short>;
