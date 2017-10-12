//
//  textInput.h
//  textInput
//
//  Created by Elliot Woods on 09/12/2011.
//  modified by James George 12/2/2011
//  modified by Momo the Monster 7/10/2012
//  swappable fonts added by James George 9/11/2012
//	refactoring and modifications by Elliot Woods on 30/11/2014
//  Other changes made by Dom Amato 11/22/2015
//
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

// jg : TODO: text wrapping

#pragma once

#include "ofMain.h"

#include "ofxUIFontRenderer.h"
#include "ofxClipboard.h"

class ofxTextInputField {
public:

	ofxTextInputField();

	//swap in a font!
	void setFont(OFX_TEXTFIELD_FONT_RENDERER& font);
	ofxUIUtils::FontRenderer * getFontRenderer();

	void		setup();

	void		setHorizontalPadding(int val);
	void		setVerticalPadding(int val);
	void		setCapsVerticalOffset(int val);
	float		getVerticalPadding() const { return verticalPadding; }
	float		getHorizontalPadding() const { return horizontalPadding; }

	void		enable();
	void		disable();
	bool		isEnabled() { return bEnabled; }

	bool		isEditing() { return bEditing; }

	void		setIsPhoneNumber(bool val) { bIsPhoneNumber = val; }

	void		draw();
	void		clear();

	bool		isMultiline() { return multiline; }
	void		setMultiline(bool state) { multiline = state; }

	bool		doesDrawCursor() { return drawCursor; }
	void		setDrawCursor(bool state) { drawCursor = state; }

	bool		doesDrawBounds() { return drawBounds; }
	void		setDrawBounds(bool state) { drawBounds = state; }

	ofRectangle getBounds() { return bounds; }
	void		setBounds(int x, int y, int w, int h) { setBounds(ofRectangle(x, y, w, h)); }
	void		setBounds(ofPoint p, int w, int h) { setBounds(ofRectangle(p, w, h)); }
	void		setBounds(ofPoint p1, ofPoint p2) { setBounds(ofRectangle(p1, p2)); }
	void		setBounds(ofRectangle r) { bounds = r; }

	void		setText(string s) { if (!multiline) ofStringReplace(text, "\n", ""); text = s; if (text.length() < cursorPosition) { cursorPosition = text.length(); } }
	string		getText() { return text; }

	void		setPlaceholderText(string s) { placeholderText = s; }
	string		getPlaceholderText() { return placeholderText; }

	void		setTextColor(ofColor c) { textColor = c; }
	void		setPlaceholderTextColor(ofColor c) { placeholderColor = c; }
	void		setTextSelectionColor(ofColor c) { selectionColor = c; }
	void		setBoundsColor(ofColor c) { boundsColor = c; }
	void		setBackgroundColor(ofColor c) { bgColor = c; }

	ofColor		getTextColor() { return textColor; }
	ofColor		getPlaceholderTextColor() { return placeholderColor; }
	ofColor		getTextSelectionColor() { return selectionColor; }
	ofColor		getBoundsColor() { return boundsColor; }
	ofColor		getBackgroundColor() { return bgColor; }

	ofEvent<void> editingBegan;
	ofEvent<void> editingEnded;
	ofEvent<string> textChanged;

protected:
	bool drawCursor, drawBounds, selecting, autoClear, multiline, bEnabled, 
		bEditing, mouseDownInRect, shiftHeld, commandHeld, bIsPhoneNumber;

	ofRectangle bounds;

	string text, placeholderText;

	ofColor textColor, placeholderColor, selectionColor, boundsColor, bgColor;

	int cursorPosition, selectionBegin, selectionEnd, verticalPadding, capsVerticalOffset, horizontalPadding;

	float lastTimeCursorMoved;
	
	ofxUIUtils::FontRenderer* fontRef;
	ofxClipboard clipboard;
	
	void	keyPressed(ofKeyEventArgs & a);
	void	keyReleased(ofKeyEventArgs & a);
	void    mousePressed(ofMouseEventArgs& args);
	void    mouseDragged(ofMouseEventArgs& args);
	void    mouseReleased(ofMouseEventArgs& args);

	void	getCursorCoords(int pos, int &cursorX, int &cursorY);
	int		getCursorPositionFromMouse(int x, int y);

	void		beginEditing();
	void		endEditing();
};
