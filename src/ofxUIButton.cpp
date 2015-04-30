#include "ofxUIBUtton.h"

ofxUIButton::ofxUIButton() {
	_x, _y, _w, _h = 100;
	_r = 20;
	visible = true;
	hovering = false;
	clickable = true;
	toggle = false;
	ID = 0;
	title = "";
	color.set(255, 255, 255);
	hoverColor.set(128, 128, 128);
	textColor.set(ofColor::black);
	togglable = false;
	dist = 0;

	buttonStyle = UI_BUTTON_RECT;

	fontRef = NULL;
	fontRef = new ofxUIUtils::BitmapFontRenderer();

	ofRegisterMouseEvents(this);
}


ofxUIButton::~ofxUIButton(){

}

void ofxUIButton::setFont(OFX_TEXTFIELD_FONT_RENDERER & font){
	if (fontRef->isBitmapFont()) {
		delete fontRef;
	}
	fontRef = new ofxUIUtils::TypedFontRenderer(&font);
}

ofxUIUtils::FontRenderer * ofxUIButton::getFontRenderer() {
	return this->fontRef;
}

void ofxUIButton::draw(){

	if (visible){

		if (hovering || toggle){
			ofSetColor(hoverColor);
		}
		else {
			ofSetColor(color);
		}
		switch (buttonStyle){
		case UI_BUTTON_RECT:
			ofRect(_x, _y, _w, _h);
			break;
		case UI_BUTTON_ROUNDED_RECT:
			ofRectRounded(_x, _y, _w, _h, _r);
			break;
		case UI_BUTTON_ELLIPSE:
			ofEllipse(_x, _y, _w, _h);
			break;
		default:
			ofRect(_x, _y, _w, _h);
			break;
		}
		

		ofSetColor(textColor);
		fontRef->drawString(title, _x + ((_w / 2) - (title.size() * 5)), _y + (_h / 2) + (_h / 4));


		ofSetColor(255);
	}
}

/*
*****************************************************
Mouse Funcitons
*****************************************************
*/

void ofxUIButton::mouseReleased(ofMouseEventArgs & args){
	if (clickable && args.button == 0){
		if (hovering){
			if (togglable){
				toggle = !toggle;
				pair<bool, int> temp(toggle, ID);
				ofNotifyEvent(buttonEvent, temp, this);
			}
			else {
				pair<bool, int> temp(false, ID);
				ofNotifyEvent(buttonEvent, temp, this);
			}
		}
	}
}

void ofxUIButton::mouseMoved(ofMouseEventArgs & args){
	if (clickable){
		dist = ofDist(_x + (_w / 2), 0, args.x, 0);
		float disty = ofDist(0, _y + (_h / 2), 0,args.y);
		if (dist < _w / 2 && disty < _h / 2){
			hovering = true;
		}
		else {
			hovering = false;
		}
	}
}

void ofxUIButton::mousePressed(ofMouseEventArgs & args){
	if (clickable && args.button == 0 && !togglable && hovering){
		pair<bool, int> temp(true, ID);
		ofNotifyEvent(buttonEvent, temp, this);
	}
}

void ofxUIButton::mouseDragged(ofMouseEventArgs & args){
	//
}