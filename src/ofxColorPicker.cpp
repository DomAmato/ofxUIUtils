#include "ofxColorPicker.h"

ofxColorPicker::ofxColorPicker(){
	visible = true;
}

ofxColorPicker::~ofxColorPicker(){

}

void ofxColorPicker::setup(ColorPickerType type, float width, float height){
	float w = width;
	float h = height;
	float cx = w / 2;
	float cy = h / 2;

	pickerType = type;
	colorPicker.allocate(w, h, OF_IMAGE_COLOR);

	switch (type){
	case RADIAL:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float angle = atan2(y - cy, x - cy) + PI;
				float dist = ofDist(x, y, cx, cy);
				float hue = angle / TWO_PI * 255;
				float sat = ofMap(dist, 0, w / 4, 0, 255, true);
				float bri = ofMap(dist, w / 4, w / 2, 255, 0, true);

				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case RADIAL_BRIGHTNESS:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float angle = atan2(y - cy, x - cy) + PI;
				float dist = ofDist(x, y, cx, cy);
				float hue = angle / TWO_PI * 255;
				float sat = ofMap(dist, 0, w / 4, 0, 255, true);
				float bri = ofMap(dist, w / 4, w / 2, 255, 0, true);

				if (y < h*.12){
					dist = ofDist(x, y, 0, y);
					bri = ofMap(dist, 0, w, 0, 255, true);
					sat = 0;
				}

				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case HORIZONTAL_HALF:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {


				float dist = ofDist(x, y, 0, y);
				float hue = ofMap(dist, 0, w, 0, 255, true);

				dist = ofDist(x, 0, x, y);
				float sat = y <= h / 2 ? ofMap(dist, 0, h / 2, 0, 255, true) : ofMap(dist, h / 2, h, 255, 0, true);
				float bri = ofMap(dist, 0, h / 2, 0, 255, true);

				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case HORIZONTAL_HALF_BRIGHTNESS:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float bri;
				float sat;
				float dist = ofDist(x, y, 0, y);
				float hue = ofMap(dist, 0, w, 0, 255, true);
				if (y < h*.12){
					bri = ofMap(dist, 0, w, 0, 255, true);
					sat = 0;
				}
				else {
					dist = ofDist(x, 0, x, y);
					//sat = ofMap(dist, 0, h, 0, 255, true);
					sat = y <= h / 2 ? ofMap(dist, 0, h / 2, 0, 255, true) : ofMap(dist, h / 2, h, 255, 0, true);
					//bri = ofMap(dist, 0, h, 0, 255, true);
					bri = ofMap(dist, 0, h / 2, 0, 255, true);
				}
				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case HORIZONTAL:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {


				float dist = ofDist(x, y, 0, y);
				float hue = ofMap(dist, 0, w, 0, 255, true);

				dist = ofDist(x, 0, x, y);
				float sat = ofMap(dist, 0, h, 0, 255, true);
				float bri = ofMap(dist, 0, h, 0, 255, true);

				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case HORIZONTAL_BRIGHTNESS:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float bri;
				float sat;
				float dist = ofDist(x, y, 0, y);
				float hue = ofMap(dist, 0, w, 0, 255, true);
				if (y < h*.12){
					bri = ofMap(dist, 0, w, 0, 255, true);
					sat = 0;
				}
				else {
					dist = ofDist(x, 0, x, y);
					sat = ofMap(dist, 0, h, 0, 255, true);
					bri = ofMap(dist, 0, h, 0, 255, true);
				}
				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case VERTICAL_HALF:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float dist = ofDist(x, 0, x, y);
				float hue = ofMap(dist, 0, h, 0, 255, true);

				dist = ofDist(x, y, 0, y);
				float sat = x <= w / 2 ? ofMap(dist, 0, w / 2, 0, 255, true) : ofMap(dist, w / 2, w, 255, 0, true);
				float bri = ofMap(dist, 0, w / 2, 0, 255, true);

				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case VERTICAL_HALF_BRIGHTNESS:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float bri;
				float sat;
				float dist = ofDist(x, 0, x, y);
				float hue = ofMap(dist, 0, h, 0, 255, true);

				
				if (x < w*.12){
					bri = ofMap(dist, 0, h, 255, 0, true);
					sat = 0;
				}
				else {
					dist = ofDist(x, y, 0, y);
					sat = x <= w / 2 ? ofMap(dist, 0, w / 2, 0, 255, true) : ofMap(dist, w / 2, w, 255, 0, true);
					bri = ofMap(dist, 0, w / 2, 0, 255, true);
				}
				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case VERTICAL:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {


				float dist = ofDist(x, 0, x, y);
				float hue = ofMap(dist, 0, h, 0, 255, true);

				dist = ofDist(x, y, 0, y);
				float sat = ofMap(dist, 0, w, 0, 255, true);
				float bri = ofMap(dist, 0, w, 0, 255, true);

				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	case VERTICAL_BRIGHTNESS:
		for (float y = 0; y < h; y++) {
			for (float x = 0; x < w; x++) {

				float bri;
				float sat;
				float dist = ofDist(x, 0, x, y);
				float hue = ofMap(dist, 0, h, 0, 255, true);

				
				if (x < h*.12){
					bri = ofMap(dist, 0, w, 255, 0, true);
					sat = 0;
				}
				else {
					dist = ofDist(x, y, 0, y);
					sat = ofMap(dist, 0, w, 0, 255, true);
					 bri = ofMap(dist, 0, w, 0, 255, true);
				}
				if (x == 0 || y == 0 || y == h - 1 || x == w - 1)
					colorPicker.setColor(x, y, ofColor::black);
				else
					colorPicker.setColor(x, y, ofColor::fromHsb(hue, sat, bri));
			}
		}
		break;
	}

	colorPicker.update();// reloadTexture();


	pickerRect.x = 0;
	pickerRect.y = 0;
	pickerRect.width = width;
	pickerRect.height = height;

	ofRegisterMouseEvents(this, OF_EVENT_ORDER_BEFORE_APP);

}

void ofxColorPicker::draw(int x, int y, int w, int h){
	pickerRect.x != x ? pickerRect.x = x : 0;
	pickerRect.y != y ? pickerRect.y = y : 0;
	pickerRect.width != w ? pickerRect.width = w : 0;
	pickerRect.height != h ? pickerRect.height = h : 0;
	draw();
}
void ofxColorPicker::draw(int x, int y){
	pickerRect.x != x ? pickerRect.x = x : 0;
	pickerRect.y != y ? pickerRect.y = y : 0;
	draw();
}
void ofxColorPicker::draw(ofPoint p){
	pickerRect.x != p.x ? pickerRect.x = p.x : 0;
	pickerRect.y != p.y ? pickerRect.y = p.y : 0;
	draw();
}
void ofxColorPicker::draw(){
	if (visible) {
		ofPushStyle(); {
			ofSetColor(255);
			colorPicker.draw(pickerRect.x, pickerRect.y, pickerRect.width, pickerRect.height);
		} ofPopStyle();
	}
}

void ofxColorPicker::setWidth(int w){
	pickerRect.width = w;
}
void ofxColorPicker::setHeight(int h){
	pickerRect.height = h;
}
float ofxColorPicker::getWidth(){
	return pickerRect.width;
}
float ofxColorPicker::getHeight(){
	return pickerRect.height;
}

void  ofxColorPicker::setPosition(int x, int y){
	pickerRect.x = x;
	pickerRect.y = y;
}
void  ofxColorPicker::setPosition(ofPoint p){
	pickerRect.x = p.x;
	pickerRect.y = p.y;
}


/*
*****************************************************
Mouse Funcitons
*****************************************************
*/

void ofxColorPicker::mouseReleased(ofMouseEventArgs & args){
	if (visible && pickerRect.inside(args.x, args.y)){
		float x = ofMap(args.x, pickerRect.x, pickerRect.x + pickerRect.width, 0, pickerRect.width);
		float y = ofMap(args.y, pickerRect.y, pickerRect.y + pickerRect.height, 0, pickerRect.height);
		ofNotifyEvent(pickerPickEvent, colorPicker.getColor(x, y), this);
	}
}

void ofxColorPicker::mouseDragged(ofMouseEventArgs & args){
	if (visible && pickerRect.inside(args.x, args.y)){
		float x = ofMap(args.x, pickerRect.x, pickerRect.x + pickerRect.width, 0, pickerRect.width);
		float y = ofMap(args.y, pickerRect.y, pickerRect.y + pickerRect.height, 0, pickerRect.height);
		ofNotifyEvent(pickerMoveEvent, colorPicker.getColor(x, y), this);
	}
}

void ofxColorPicker::mouseMoved(ofMouseEventArgs & args) {
	if (visible && pickerRect.inside(args.x, args.y)) {
		float x = ofMap(args.x, pickerRect.x, pickerRect.x + pickerRect.width, 0, pickerRect.width);
		float y = ofMap(args.y, pickerRect.y, pickerRect.y + pickerRect.height, 0, pickerRect.height);
		ofNotifyEvent(pickerMoveEvent, colorPicker.getColor(x, y), this);
	}
}
