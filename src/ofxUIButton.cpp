#include "ofxUIBUtton.h"

ofxUIButton::ofxUIButton() {
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
	edgeColor.set(ofColor::darkGray);
	hasEdge = true;
	togglable = false;
	edgeWidth = 1;
	_autoSize = false;

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
		ofPushStyle(); {
			ofFill();

			if (hovering || toggle) {
				ofSetColor(hoverColor);
			}
			else {
				ofSetColor(color);
			}
			if (button.width < fontRef->stringWidth(title) + 10)
				button.width = fontRef->stringWidth(title) + 10;
			switch (buttonStyle) {
			case UI_BUTTON_RECT:
				ofRect(button.x, button.y, button.width, button.height);
				break;
			case UI_BUTTON_ROUNDED_RECT:
				ofRectRounded(button.x, button.y, button.width, button.height, _r);
				break;
			case UI_BUTTON_ELLIPSE:
				ofEllipse(button.x, button.y, button.width, button.height);
				break;
			default:
				ofRect(button.x, button.y, button.width, button.height);
				break;
			}


			ofSetColor(textColor);
			fontRef->drawString(title, button.x + ((button.width / 2) - fontRef->stringWidth(title) / 2), button.y + (button.height / 2) + (button.height / 4));

			if (hasEdge) {
				ofNoFill();
				ofSetColor(edgeColor);
				ofSetLineWidth(edgeWidth);
				switch (buttonStyle) {
				case UI_BUTTON_RECT:
					ofRect(button.x, button.y, button.width, button.height);
					break;
				case UI_BUTTON_ROUNDED_RECT:
					ofRectRounded(button.x, button.y, button.width, button.height, _r);
					break;
				case UI_BUTTON_ELLIPSE:
					ofEllipse(button.x, button.y, button.width, button.height);
					break;
				default:
					ofRect(button.x, button.y, button.width, button.height);
					break;
				}
			}

			ofSetColor(255);
		}
		ofPopStyle();
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
		if (button.inside(args.x, args.y)){
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

