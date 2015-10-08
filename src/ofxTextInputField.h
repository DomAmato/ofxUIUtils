//
//  textInput.h
//  textInput
//
//  Created by Elliot Woods on 09/12/2011.
//  modified by James George 12/2/2011
//  modified by Momo the Monster 7/10/2012
//  swappable fonts added by James George 9/11/2012
//	refactoring and modifications by Elliot Woods on 30/11/2014
//
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

// jg : TODO: text wrapping

#pragma once

#include "ofMain.h"

#define TEXTFIELD_IS_ACTIVE "textfieldIsActive"
#define TEXTFIELD_IS_INACTIVE "textfieldIsInactive"

#include "ofxUIFontRenderer.h"
#include "ofxClipboard.h"

class ofxTextInputField {
public:
	ofxTextInputField();
	virtual ~ofxTextInputField();

	/// Always call this first
	void setup(bool enableListeners = true);

	/// Change the font used to draw the text
	void setFont(OFX_TEXTFIELD_FONT_RENDERER & font);
	ofxUIUtils::FontRenderer * getFontRenderer();

	void enable();
	void disable();
	bool isEnabled() const;

	/// Whether the text box is focused and capturing keys
	void beginEditing();
	void endEditing();
	bool isEditing() const;

	void setUseListeners(bool);
	bool getUseListeners() const;

	/// Draw inside this->bounds
	void draw();

	/// Clear text
	void clear();

	bool getCMDHeld() { return commandHeld; }

	void getCursorCoords(int pos, int &cursorX, int &cursorY);

	void addListeners();
	void removeListeners();
	bool hasListeners;


	float getVerticalPadding() const;
	float getHorizontalPadding() const;

	void setBounds(int x, int y, int width, int height) { bounds.set(x, y, width, height); }
	void setBounds(ofRectangle r) { bounds.set(r); }
	void setBounds(ofPoint p, int width, int height) { bounds.set(p, width, height); }
	ofRectangle getBounds() { return bounds; }

	void setText(string text) { this->text = text; renderString(); }
	string getText() { return text; }

	void setMultiline(bool multiline) { this->multiline = multiline; }
	bool isMultiline() { return multiline; }

	void setBackgroundColor(ofColor c) { bgColor = c; }
	void setFontColor(ofColor c) { fontColor = c; }
	ofColor setBackgroundColor() { return bgColor; }
	ofColor setFontColor() { return fontColor; }

private:

	int getCursorPositionFromMouse(int x, int y);
	int getTextPositionFromMouse(int x, int y);

	ofColor bgColor, fontColor;

	bool autoClear;

	bool multiline;

	bool drawCursor;

	string text;

	ofRectangle bounds;
	int cursorPosition;
	int textPos;
	int selectionBegin;
	int selectionEnd;
	bool selecting;

	ofEvent<string> onTextChange;
	ofEvent<string> onHitReturn;

	void keyPressed(ofKeyEventArgs &a);
	void keyReleased(ofKeyEventArgs &a);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);

	float lastTimeCursorMoved;

	float verticalPadding;
	float horizontalPadding;

	ofxUIUtils::FontRenderer* fontRef;

	bool enabled;
	bool editing;
	bool useListeners;

	bool mouseDownInRect;

	void notifyTextChange();
	void notifyHitReturn();

	bool shiftHeld, commandHeld;
	map<int, char> shiftMap;

	ofxClipboard clipboard;
	void renderString();

	struct word {
		string text;
		ofRectangle rect;
	};
	vector< word > mWords;
	vector<string> mLines;
	//vector< vector<string> > mLines;
};
