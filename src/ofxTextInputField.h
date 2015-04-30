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

    //can be set manually or otherwise is controlled by enable/disable
    bool drawCursor;
	
	string text;

	ofRectangle bounds;
	int cursorPosition;
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

	bool autoClear;
	bool autoTab;
	
	bool multiline;
    
	float getVerticalPadding() const;
	float getHorizontalPadding() const;

	#ifdef USE_GLFW_CLIPBOARD
    void setClipboard(string clippy);
    string getClipboard();
	#endif
	
  protected:
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
	void getCursorCoords(int pos, int &cursorX, int &cursorY);
	int getCursorPositionFromMouse(int x, int y);
    
	void addListeners();
	void removeListeners();
	bool hasListeners;

    bool shiftHeld, commandHeld;
    map<int, char> shiftMap;
};
