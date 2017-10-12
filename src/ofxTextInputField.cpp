//
//  textInput.cpp
//
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

#include "ofxTextInputField.h"

//----------
ofxTextInputField::ofxTextInputField() {
	placeholderText = "";
	text = "";

	placeholderColor = ofColor::darkGray;
	textColor = ofColor::black;
	selectionColor.setHex(0x6988db);
	boundsColor = ofColor::gray;
	bgColor = ofColor::white;

	multiline = false;
	cursorPosition = 0;
	selectionBegin = 0;
	selectionEnd = 0;
	selecting = false;

	commandHeld = false;
	shiftHeld = false;

	fontRef = NULL;
	bEnabled = false;
	bEditing = false;
	bounds = ofRectangle(0, 0, 100, 22);

	drawCursor = false;
	drawBounds = true;
	autoClear = false;
	mouseDownInRect = false;

	fontRef = new ofxUIUtils::BitmapFontRenderer();

	verticalPadding = 3;
	capsVerticalOffset = 0;
	horizontalPadding = 10;
	lastTimeCursorMoved = ofGetElapsedTimef();

	bIsPhoneNumber = false;
}

void ofxTextInputField::setup() {
	enable();
}


void ofxTextInputField::enable() {
	if (!bEnabled) {
		ofAddListener(ofEvents().mousePressed, this, &ofxTextInputField::mousePressed);
		ofAddListener(ofEvents().mouseDragged, this, &ofxTextInputField::mouseDragged);
		ofAddListener(ofEvents().mouseReleased, this, &ofxTextInputField::mouseReleased);
		bEnabled = true;
	}
}

void ofxTextInputField::disable() {
	if (bEditing) {
		endEditing();
	}
	if (bEnabled) {
		ofRemoveListener(ofEvents().mousePressed, this, &ofxTextInputField::mousePressed);
		ofRemoveListener(ofEvents().mouseDragged, this, &ofxTextInputField::mouseDragged);
		ofRemoveListener(ofEvents().mouseReleased, this, &ofxTextInputField::mouseReleased);
		bEnabled = false;
	}

}
void ofxTextInputField::beginEditing() {
	if (!bEditing) {
		ofAddListener(ofEvents().keyPressed, this, &ofxTextInputField::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &ofxTextInputField::keyReleased);
		ofNotifyEvent(editingBegan, this);
		bEditing = true;
		drawCursor = true;
		if (autoClear) {
			clear();
		}
		else {


		}
	}
}

void ofxTextInputField::endEditing() {
	if (bEditing) {
		ofRemoveListener(ofEvents().keyPressed, this, &ofxTextInputField::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &ofxTextInputField::keyReleased);
		ofNotifyEvent(editingEnded, this);
		bEditing = false;
		drawCursor = false;
	}
}

void ofxTextInputField::setFont(OFX_TEXTFIELD_FONT_RENDERER & font) {
	if (fontRef->isBitmapFont()) {
		delete fontRef;
	}
	fontRef = new ofxUIUtils::TypedFontRenderer(&font);
}

ofxUIUtils::FontRenderer * ofxTextInputField::getFontRenderer() {
	return this->fontRef;
}

void ofxTextInputField::setHorizontalPadding(int val) {
	horizontalPadding = val;
}

void ofxTextInputField::setVerticalPadding(int val) {
	verticalPadding = val;
}

void ofxTextInputField::setCapsVerticalOffset(int val) {
	capsVerticalOffset = val;
}

//----------
void ofxTextInputField::draw() {
	if (drawBounds) {
		ofPushStyle(); {
			ofSetColor(bgColor);
			ofRect(bounds);
			ofPushStyle();
			{
				ofNoFill();
				ofSetLineWidth(1);
				ofSetColor(ofGetStyle().color.invert());
				ofRect(bounds);

			}
			ofPopStyle();
		}
		ofPopStyle();
	}
	float slineHeight = fontRef->isBitmapFont() ? fontRef->getLineHeight() + 2 : fontRef->getRenderer()->getGlyphBBox().height;
	float lineHeight = fontRef->getLineHeight();

	ofPushMatrix(); {
		ofTranslate(bounds.x, bounds.y);
		if (selecting) {
			ofPushStyle(); {
				vector<string> lines = ofSplitString(text, "\n");
				int beginCursorX, beginCursorY;
				int endCursorX, endCursorY;
				getCursorCoords(selectionBegin, beginCursorX, beginCursorY);
				getCursorCoords(selectionEnd, endCursorX, endCursorY);

				float startX = beginCursorX > 0 ? fontRef->stringWidth(lines[beginCursorY].substr(0, beginCursorX)) : 0;
				float endX = fontRef->stringWidth(lines[endCursorY].substr(0, endCursorX));

				ofSetColor(selectionColor);
				ofFill();

				if (beginCursorY == endCursorY) {
					// single line selection
					ofRect(horizontalPadding + startX - 2, verticalPadding + lineHeight*beginCursorY,
						endX - startX + 4, slineHeight);
				}
				else {

					// multiline selection.
					// do first line to the end
					ofRect(horizontalPadding + startX - 2, verticalPadding + lineHeight*beginCursorY,
						fontRef->stringWidth(lines[beginCursorY]) - startX + 4, slineHeight);

					// loop through entirely selected lines
					for (int i = beginCursorY + 1; i < endCursorY; i++) {
						ofRect(horizontalPadding - 2, verticalPadding + lineHeight*i,
							fontRef->stringWidth(lines[i]) + 4, slineHeight);
					}
					// do last line up to endX
					ofRect(horizontalPadding - 2, verticalPadding + lineHeight*endCursorY,
						endX + 4, slineHeight);
				}
			}
			ofPopStyle();

		}
		//draw cursor line
		else if (drawCursor) {
			ofPushStyle();
			{
				// cursor should only blink when its been idle, and animation
				// should be a clipped sine wave
				int timeFrac = 0.5 * ofClamp(cos(6.0f * (ofGetElapsedTimef() - lastTimeCursorMoved)) * 4, -1, 1) + 0.5;
				//either its on or its off...

				timeFrac ? ofSetColor(textColor) : ofSetColor(bgColor);


				// argh, splitting all the time.
				vector<string> lines = ofSplitString(text, "\n");

				// calculate this every loop.
				int cursorX, cursorY;
				getCursorCoords(cursorPosition, cursorX, cursorY);

				float textWidth = 0;
				if (text.length() > 0) {
					textWidth = fontRef->stringWidth(text);
				}
				float cursorPos = horizontalPadding + (fontRef->stringWidth(lines[cursorY].substr(0, cursorX)) > 0 ? fontRef->stringWidth(lines[cursorY].substr(0, cursorX)) : 0);

				if (cursorPos < 0) {
					cursorPos = 0;
				}

				int cursorTop = verticalPadding + lineHeight*cursorY + abs(lineHeight - slineHeight);
				int cursorBottom = cursorTop + lineHeight;

				ofSetLineWidth(1.0f);
				ofLine(cursorPos, cursorTop, cursorPos, cursorBottom);
			}
			ofPopStyle();
		}
		else if (placeholderText != "" && text == "") {
			ofSetColor(placeholderColor);
			fontRef->drawString(placeholderText, horizontalPadding, lineHeight + verticalPadding);
		}

		ofSetColor(textColor);
		fontRef->drawString(text, horizontalPadding, lineHeight + verticalPadding + capsVerticalOffset);
	}
	ofPopMatrix();
}

void ofxTextInputField::clear() {
	text.clear();
	cursorPosition = 0;
}

void ofxTextInputField::getCursorCoords(int pos, int &cursorX, int &cursorY) {
	vector<string> lines = ofSplitString(text, "\n");

	int c = 0;

	for (int i = 0; i < lines.size(); i++) {
		if (pos <= c + lines[i].size()) {
			cursorY = i;
			cursorX = pos - c;
			return;
		}
		c += lines[i].size() + 1;
	}
}

int ofxTextInputField::getCursorPositionFromMouse(int x, int y) {
	int cursorX = 0;
	int cursorY = 0;
	float pos = y - bounds.y - verticalPadding;
	pos /= fontRef->getLineHeight();
	int line = pos;
	cursorY = line;

	vector<string> lines = ofSplitString(text, "\n");
	if (cursorY >= lines.size() - 1) cursorY = lines.size() - 1;
	if (lines.size() > 0) {
		cursorX = fontRef->getPosition(lines[cursorY], x - horizontalPadding - bounds.x);
	}
	int c = 0;
	for (int i = 0; i < cursorY; i++) {
		c += lines[i].size() + 1;
	}
	c += cursorX;
	return c;
}


void ofxTextInputField::mousePressed(ofMouseEventArgs& args) {
	mouseDownInRect = bounds.inside(args.x, args.y);
	if (mouseDownInRect) {
		cursorPosition = getCursorPositionFromMouse(args.x, args.y);
		lastTimeCursorMoved = ofGetElapsedTimef();
		selecting = false;
	}
}


void ofxTextInputField::mouseDragged(ofMouseEventArgs& args) {
	if (bounds.inside(args.x, args.y)) {
		int pos = getCursorPositionFromMouse(args.x, args.y);
		if (pos != cursorPosition) {
			selecting = true;
			selectionBegin = MIN(pos, cursorPosition);
			selectionEnd = MAX(pos, cursorPosition);

		}
		else {
			selecting = false;
		}
	}
}

void ofxTextInputField::mouseReleased(ofMouseEventArgs& args) {
	if (bounds.inside(args.x, args.y)) {
		if (!bEditing && mouseDownInRect) {
			beginEditing();
		}
	}
	else if (bEditing) {
		endEditing();
	}
}



void ofxTextInputField::keyPressed(ofKeyEventArgs & a)
{
	lastTimeCursorMoved = ofGetElapsedTimef();

	if (a.key == OF_KEY_SHIFT || a.key == OF_KEY_LEFT_SHIFT || a.key == OF_KEY_RIGHT_SHIFT) {
		shiftHeld = true;
		return;
	}

#if defined(TARGET_WIN32)
	if (a.key == OF_KEY_CONTROL || a.key == OF_KEY_LEFT_CONTROL || a.key == OF_KEY_RIGHT_CONTROL) {
#else
	if (a.key == OF_KEY_COMMAND) {
#endif
		commandHeld = true;
		return;
	}

	if ((a.key == 'a' || a.keycode == 'A') && commandHeld) {
		selecting = true;
		selectionBegin = 0;
		selectionEnd = text.size();
		return;
	}

	if ((a.key == 'c' || a.keycode == 'C') && commandHeld) {
		clipboard.setString(text.substr(selectionBegin, selectionEnd - selectionBegin));
		return;
	}

	if ((a.key == 'x' || a.keycode == 'X') && commandHeld) {
		clipboard.setString(text.substr(selectionBegin, selectionEnd - selectionBegin));
		text.erase(text.begin() + selectionBegin, text.begin() + selectionEnd);
		selecting = false;
		cursorPosition = selectionBegin;
		return;
	}

	if ((a.key == 'v' || a.keycode == 'V') && commandHeld) {
		string paste = clipboard.getString();
		if (!multiline)
			ofStringReplace(paste, "\n", "");
		text.insert(cursorPosition, paste);
		return;
	}

	if ((a.key >= 32 && a.key <= 126) || a.key == '\t' || a.key == OF_KEY_RETURN) {
		if (selecting) {
			text.erase(text.begin() + selectionBegin, text.begin() + selectionEnd);
			cursorPosition = selectionBegin;
			selecting = false;
		}
	}


	if (a.key == OF_KEY_RETURN) {
		if (!multiline) {
			endEditing();
			return;
		}
		text.insert(text.begin() + cursorPosition, '\n');
		cursorPosition++;


		ofNotifyEvent(textChanged, text, this);
		return;
	}


	if (a.key == OF_KEY_BACKSPACE) {
		if (selecting) {
			text.erase(text.begin() + selectionBegin, text.begin() + selectionEnd);
			cursorPosition = selectionBegin;
			selecting = false;
		}
		else {
			if (cursorPosition > 0) {
				text.erase(text.begin() + cursorPosition - 1);
				--cursorPosition;
			}
		}

		ofNotifyEvent(textChanged, text, this);
		return;
	}

	if (a.key == OF_KEY_DEL) {
		if (selecting) {
			text.erase(text.begin() + selectionBegin, text.begin() + selectionEnd);
			cursorPosition = selectionBegin;
			selecting = false;
		}
		else {
			if (text.size() > cursorPosition) {
				text.erase(text.begin() + cursorPosition);
			}
		}

		ofNotifyEvent(textChanged, text, this);
		return;
	}

	if (a.key == OF_KEY_LEFT) {
		if (selecting) {
			cursorPosition = selectionBegin;
			selecting = false;

		}
		else {
			if (cursorPosition > 0) {
				--cursorPosition;
			}
		}
		return;
	}
	if (a.key == OF_KEY_RIGHT) {
		if (selecting) {
			cursorPosition = selectionEnd;
			selecting = false;
		}
		else {
			if (cursorPosition < text.size()) {
				++cursorPosition;
			}
		}
		return;
	}
	if (a.key == OF_KEY_UP) {
		if (selecting) {
			cursorPosition = selectionBegin;
			selecting = false;

		}
		else {
			if (cursorPosition > 0) {
				int xx, yy;
				getCursorCoords(cursorPosition, xx, yy);
				if (yy > 0) {
					yy--;
					vector<string> lines = ofSplitString(text, "\n");
					xx = MIN(lines[yy].size() - 1, xx);
					cursorPosition = xx;
					for (int i = 0; i < yy; i++) cursorPosition += lines[i].size() + 1;
					printf("Cursor position: %d\n", cursorPosition);
				}
				else {
					cursorPosition = 0;
				}
			}
		}
		return;
	}
	if (a.key == OF_KEY_DOWN) {
		if (selecting) {
			cursorPosition = selectionEnd;
			selecting = false;
		}
		else {
			int xx, yy;
			getCursorCoords(cursorPosition, xx, yy);
			vector<string> lines = ofSplitString(text, "\n");
			yy++;
			if (yy < lines.size() - 1) {

				xx = MIN(lines[yy].size() - 1, xx);
				cursorPosition = xx;
				for (int i = 0; i < yy; i++) cursorPosition += lines[i].size() + 1;
				printf("Cursor position: %d\n", cursorPosition);
			}
			else {
				cursorPosition = text.size() - 1;
			}
		}
		return;
	}
	text.insert(text.begin() + cursorPosition, a.key);
	cursorPosition++;

	if (bIsPhoneNumber) {
		if (text.size() == 3 || text.size() == 7) {
			text.insert(text.begin() + cursorPosition, 45);
			cursorPosition++;
		}
	}
	ofNotifyEvent(textChanged, text, this);
	}

void ofxTextInputField::keyReleased(ofKeyEventArgs & a)
{
#if defined(TARGET_OSX)
	if (a.key == OF_KEY_COMMAND) {  // EZ: or 4532, test this on Mac
#elif defined(TARGET_WIN32)
	if (a.key == OF_KEY_CONTROL) {
#endif
		commandHeld = false;
	}

	if (a.key == OF_KEY_SHIFT) {
		shiftHeld = false;
	}
}
