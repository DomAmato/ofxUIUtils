//
//  textInput.cpp
//
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

#include "ofxTextInputField.h"

//----------
ofxTextInputField::ofxTextInputField() {
	text = "";
	multiline = false;
	cursorPosition = 0;
	selectionBegin = 0;
	selectionEnd = 0;
	textPos = 0;
	selecting = false;

	fontRef = NULL;
	this->enabled = false;
	this->editing = false;
	this->useListeners = false;
	this->hasListeners = false;
	bounds = ofRectangle(0, 0, 100, 22);

	drawCursor = false;
	autoClear = false;
	mouseDownInRect = false;

	fontRef = new ofxUIUtils::BitmapFontRenderer();

	verticalPadding = 3;
	horizontalPadding = 3;
	lastTimeCursorMoved = ofGetElapsedTimef();
	commandHeld = false;

	bgColor = ofColor::white;
	fontColor = ofColor::black;
}

//----------
ofxTextInputField::~ofxTextInputField() {
	this->removeListeners();
}

//----------
void ofxTextInputField::setup(bool enableListeners) {
	enable();
	this->setUseListeners(enableListeners);
}

//----------
void ofxTextInputField::setFont(OFX_TEXTFIELD_FONT_RENDERER & font) {
	if (fontRef->isBitmapFont()) {
		delete fontRef;
	}
	fontRef = new ofxUIUtils::TypedFontRenderer(&font);
	renderString();
}

//----------
ofxUIUtils::FontRenderer * ofxTextInputField::getFontRenderer() {
	return this->fontRef;
}

//----------
void ofxTextInputField::enable() {
	if (!this->enabled) {
		this->enabled = true;
	}
}

//----------
void ofxTextInputField::disable() {
	if (this->editing) {
		endEditing();
	}
	if (this->enabled) {
		this->enabled = false;
	}

}

//----------
bool ofxTextInputField::isEnabled() const {
	return this->enabled;
}

//----------
void ofxTextInputField::beginEditing() {
	if (!this->editing) {
		ofSendMessage(TEXTFIELD_IS_ACTIVE);
		this->editing = true;
		drawCursor = true;
		if (autoClear) {
			clear();
		}
	}
}

//----------
void ofxTextInputField::endEditing() {
	if (this->editing) {
		ofSendMessage(TEXTFIELD_IS_INACTIVE);
		this->editing = false;
		this->drawCursor = false;
		this->commandHeld = false;
	}
}

//----------
bool ofxTextInputField::isEditing() const {
	return this->editing;
}

//----------
void ofxTextInputField::setUseListeners(bool useListeners) {
	if (useListeners) {
		this->addListeners();
	}
	else {
		this->removeListeners();
	}
}

//----------
bool ofxTextInputField::getUseListeners() const {
	return this->useListeners;
}

//----------
void ofxTextInputField::draw() {
	ofPushMatrix();
	{
		ofPushStyle();
		{
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
			ofTranslate(bounds.x, bounds.y);

			if (selecting) {
				ofPushStyle();
				{
					int beginCursorX, beginCursorY;
					int endCursorX, endCursorY;
					getCursorCoords(selectionBegin, beginCursorX, beginCursorY);
					getCursorCoords(selectionEnd, endCursorX, endCursorY);

					float startX = fontRef->stringWidth(mLines[beginCursorY].substr(0, beginCursorX));
					float endX = fontRef->stringWidth(mLines[endCursorY].substr(0, endCursorX));

					ofSetHexColor(0x6988db);
					ofFill();

					if (beginCursorY == endCursorY) {
						// single line selection
						ofRect(horizontalPadding + startX, verticalPadding + fontRef->getLineHeight()*beginCursorY,
							endX - startX, fontRef->getLineHeight());
					}
					else {

						// multiline selection.
						// do first line to the end
						ofRect(horizontalPadding + startX, verticalPadding + 3 + fontRef->getLineHeight()*beginCursorY,
							fontRef->stringWidth(mLines[beginCursorY]) - startX,
							fontRef->getLineHeight()
							);

						// loop through entirely selected lines
						for (int i = beginCursorY + 1; i < endCursorY; i++) {
							ofRect(horizontalPadding, verticalPadding + 3 + fontRef->getLineHeight()*i,
								fontRef->stringWidth(mLines[i]),
								fontRef->getLineHeight()
								);
						}
						// do last line up to endX
						ofRect(horizontalPadding, verticalPadding + fontRef->getLineHeight()*endCursorY,
							endX, fontRef->getLineHeight()
							);
					}
				}
				ofPopStyle();
				//draw cursor line
			}
			else if (drawCursor) {
				ofPushMatrix();
				ofPushStyle();
				// cursor should only blink when its been idle, and animation
				// should be a clipped sine wave
				float timeFrac = 0.5 * ofClamp(cos(6.0f * (ofGetElapsedTimef() - lastTimeCursorMoved)) * 4, -1, 1) + 0.5;

				ofColor col = ofGetStyle().color;
				ofSetColor(col.r * timeFrac, col.g * timeFrac, col.b * timeFrac);

				// calculate this every loop.
				int cursorX, cursorY;
				getCursorCoords(cursorPosition, cursorX, cursorY);
				float cursorPos = horizontalPadding - .5 + fontRef->stringWidth(mLines[cursorY].substr(0, cursorX));

				float cursorTop = verticalPadding + 3 + fontRef->getLineHeight()*cursorY;
				float cursorBottom = cursorTop + fontRef->getLineHeight();

				ofSetLineWidth(1.5f);
				//TODO: multiline with fontRef
				ofLine(cursorPos, cursorTop, cursorPos, cursorBottom);
				ofPopStyle();
				ofPopMatrix();
			}

			for (int i = 0; i < mLines.size(); i++) {
				ofSetColor(fontColor);
				fontRef->drawString(mLines[i], horizontalPadding, fontRef->getLineHeight() + fontRef->getLineHeight()*i + verticalPadding);
			}
		}
		ofPopStyle();
	}
	ofPopMatrix();
}

//----------
void ofxTextInputField::clear() {
	text.clear();
	cursorPosition = 0;
	this->notifyTextChange();
}

//----------
void ofxTextInputField::keyPressed(ofKeyEventArgs& args) {
	if (!this->editing) {
		return;
	}

	lastTimeCursorMoved = ofGetElapsedTimef();
	int key = args.key;

#if defined(TARGET_WIN32)
	if (args.key == OF_KEY_CONTROL) {
#else
	if (args.key == OF_KEY_COMMAND) {
#endif
		this->commandHeld = true;
	}

	if (args.key == 'c' || args.keycode == 'C' && this->commandHeld) {
		clipboard.setString(text.substr(selectionBegin, selectionEnd - selectionBegin));
		return;
	}

	if (args.key == 'v' || args.keycode == 'V' && this->commandHeld) {
		text.insert(textPos, clipboard.getString());
		return;
	}

	if ((key >= 32 && key <= 126) || key == '\t' || key == OF_KEY_RETURN) {
		if (selecting) {
			text.erase(text.begin() + selectionBegin,
				text.begin() + selectionEnd
				);
			cursorPosition = selectionBegin;
			selecting = false;
			this->notifyTextChange();
		}
	}

	if (key == OF_KEY_RETURN) {
		if (!multiline) {
			endEditing();
			this->notifyHitReturn();
			return;
		}
		text.insert(text.begin() + textPos++, '\n');
		cursorPosition++;
		this->notifyTextChange();
		return;
	}

	if ((key >= 32 && key <= 126) || key == '\t') {
		text.insert(text.begin() + textPos++, key);
		cursorPosition++;
		this->notifyTextChange();
	}

	if (key == OF_KEY_BACKSPACE) {
		if (selecting) {
			text.erase(text.begin() + selectionBegin,
				text.begin() + selectionEnd
				);
			cursorPosition = selectionBegin;
			selecting = false;
			this->notifyTextChange();
		}
		else {
			if (textPos > 0) {
				text.erase(text.begin() + textPos - 1);
				--cursorPosition;
				--textPos;
				this->notifyTextChange();
			}
		}
	}

	if (key == OF_KEY_DEL) {
		if (selecting) {
			text.erase(text.begin() + selectionBegin,
				text.begin() + selectionEnd
				);
			cursorPosition = selectionBegin;
			textPos = selectionBegin;
			selecting = false;
			this->notifyTextChange();
		}
		else {
			if (text.size() > textPos) {
				text.erase(text.begin() + textPos);
				this->notifyTextChange();
			}
		}
	}

	if (key == OF_KEY_LEFT) {
		if (selecting) {
			cursorPosition = selectionBegin;
			textPos = selectionBegin;
			selecting = false;

		}
		else {
			if (cursorPosition > 0) {
				--textPos;
				--cursorPosition;
			}
		}
	}

	if (key == OF_KEY_RIGHT) {
		if (selecting) {
			cursorPosition = selectionEnd;
			textPos = selectionEnd;
			selecting = false;
		}
		else {
			if (cursorPosition < text.size()) {
				++cursorPosition;
				textPos++;
			}
		}
	}

	if (key == OF_KEY_UP) {
		if (selecting) {
			cursorPosition = selectionBegin;
			textPos = selectionBegin;
			selecting = false;

		}
		else if (this->multiline) {
			if (cursorPosition > 0) {
				int xx, yy;
				getCursorCoords(cursorPosition, xx, yy);
				if (yy > 0) {
					yy--;
					xx = MIN(mLines[yy].size() - 1, xx);
					cursorPosition = xx;
					for (int i = 0; i < yy; i++) cursorPosition += mLines[i].size() + 1;
					vector<string> lines = ofSplitString(text, "\n");
					xx = MIN(lines[yy].size() - 1, xx);
					textPos = xx;
					for (int i = 0; i < yy; i++) textPos += lines[i].size() + 1;
				}
				else {
					cursorPosition = 0;
					textPos = 0;
				}
			}
		}
	}

	if (key == OF_KEY_DOWN) {
		if (selecting) {
			cursorPosition = selectionEnd;
			textPos = selectionEnd;
			selecting = false;
		}
		else if (this->multiline) {
			int xx, yy;
			getCursorCoords(cursorPosition, xx, yy);
			yy++;
			if (yy < mLines.size() - 1) {

				xx = MIN(mLines[yy].size() - 1, xx);
				cursorPosition = xx;
				for (int i = 0; i < yy; i++) cursorPosition += mLines[i].size() + 1;
			}
			vector<string> lines = ofSplitString(text, "\n");
			yy++;
			if (yy < lines.size() - 1) {
				xx = MIN(lines[yy].size() - 1, xx);
				textPos = xx;
				for (int i = 0; i < yy; i++) textPos += lines[i].size() + 1;
			}
			else {
				cursorPosition = text.size() - 1;
				textPos = text.size() - 1;
			}
		}
	}
	renderString();
}

//----------
void ofxTextInputField::keyReleased(ofKeyEventArgs &a) {

#if defined(TARGET_WIN32)
	if (a.key == OF_KEY_CONTROL) {
#else
	if (a.key == OF_KEY_COMMAND) {
#endif
		this->commandHeld = false;
	}
	//if we're not focused, then ignore the keypress
	if (!this->editing) {
		return;
	}
}

//----------
void ofxTextInputField::mousePressed(ofMouseEventArgs& args) {
	if (!this->enabled) {
		return;
	}

	mouseDownInRect = bounds.inside(args.x, args.y);
	if (mouseDownInRect) {
		cursorPosition = getCursorPositionFromMouse(args.x, args.y);
		textPos = getTextPositionFromMouse(args.x, args.y);
		lastTimeCursorMoved = ofGetElapsedTimef();
		selecting = false;
	}
}

//----------
void ofxTextInputField::mouseDragged(ofMouseEventArgs& args) {
	if (!this->enabled) {
		return;
	}

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

//----------
void ofxTextInputField::mouseReleased(ofMouseEventArgs& args) {
	if (!this->enabled) {
		return;
	}

	if (bounds.inside(args.x, args.y)) {
		if (!this->editing && mouseDownInRect) {
			beginEditing();
		}
	}
	else {
		endEditing();
	}
}

//----------
float ofxTextInputField::getVerticalPadding() const {
	return this->verticalPadding;
}

//----------
float ofxTextInputField::getHorizontalPadding() const {
	return this->horizontalPadding;
}


//----------
void ofxTextInputField::notifyTextChange() {
	renderString();
	ofNotifyEvent(this->onTextChange, this->text, this);
}

//----------
void ofxTextInputField::notifyHitReturn() {
	ofNotifyEvent(this->onHitReturn, this->text, this);
}

//----------
void ofxTextInputField::getCursorCoords(int pos, int &cursorX, int &cursorY) {
	int c = 0;
	for (int i = 0; i < mLines.size(); i++) {
		if (pos <= c + mLines[i].size()) {
			cursorY = i;
			cursorX = pos - c;
			return;
		}
		c += mLines[i].size() + 1;
	}
}

//----------
int ofxTextInputField::getCursorPositionFromMouse(int x, int y) {
	int cursorX = 0;
	int cursorY = 0;
	float pos = y - bounds.y - verticalPadding;
	pos /= fontRef->getLineHeight();
	int line = pos;
	cursorY = line;

	if (cursorY >= mLines.size() - 1) cursorY = mLines.size() - 1;
	if (mLines.size() > 0) {
		cursorX = fontRef->getPosition(mLines[cursorY], x - horizontalPadding - bounds.x);
	}
	int c = 0;
	for (int i = 0; i < cursorY; i++) {
		c += mLines[i].size() + 1;
	}
	c += cursorX;
	return c;
}

int ofxTextInputField::getTextPositionFromMouse(int x, int y) {
	int cursorX = 0;
	int cursorY = 0;
	float pos = y - bounds.y - verticalPadding;
	pos /= fontRef->getLineHeight();
	int line = pos;
	cursorY = line;

	vector<string> lines = ofSplitString(text, "\n");
	for (auto line : lines) {
		ofStringReplace(line, "\n", "");
	}
	if (cursorY >= lines.size() - 1) cursorY = lines.size() - 1;
	if (lines.size() > 0) {
		cursorX = fontRef->getPosition(lines[cursorY], x - horizontalPadding - bounds.x);
	}
	int c = 0;
	for (int i = 0; i < cursorY; i++) {
		c += lines[i].size() + 1;
	}
	c += cursorX;
	//cout << "Text Position: " << c << endl;
	return c;
}

//----------
void ofxTextInputField::addListeners() {
	if (this->hasListeners) {
		return;
	}
	ofAddListener(ofEvents().keyPressed, this, &ofxTextInputField::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxTextInputField::keyReleased);
	ofAddListener(ofEvents().mousePressed, this, &ofxTextInputField::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxTextInputField::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxTextInputField::mouseReleased);
}

//----------
void ofxTextInputField::removeListeners() {
	if (!this->hasListeners) {
		return;
	}
	ofRemoveListener(ofEvents().keyPressed, this, &ofxTextInputField::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxTextInputField::keyReleased);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxTextInputField::mousePressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxTextInputField::mouseDragged);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxTextInputField::mouseReleased);
}

void ofxTextInputField::renderString()
{
	if (!fontRef->isBitmapFont()) {
		mWords.clear();
		int mLineHeight = 0;
		string str = text;
		bool newLine = str.find(" ") < str.find("\n") ? false : true;
		bool space = str.find(" ") < str.find("\n") ? true : false;
		//bool tab = 
		// break paragraph into words //
		int position = str.find(" ") < str.find("\n") ? str.find(" ") : str.find("\n");

		while (position != string::npos)
		{
			string s = str.substr(0, position);
			if (newLine) {
				s += "\n";
			}
			if (space) {
				s += " ";
			}
			word w = { s, fontRef->getRenderer()->getStringBoundingBox(s, 0, 0) };
			mWords.push_back(w);
			str.erase(0, position + 1);
			position = str.find(" ") < str.find("\n") ? str.find(" ") : str.find("\n");
			newLine = str.find(" ") < str.find("\n") ? false : true;
			space = str.find(" ") < str.find("\n") ? true : false;
			if (w.rect.height > mLineHeight) mLineHeight = w.rect.height;
		}
		// append the last word //
		word w = { str, fontRef->getRenderer()->getStringBoundingBox(str, 0, 0) };
		mWords.push_back(w);
		if (w.rect.height > mLineHeight) mLineHeight = w.rect.height;

		// assign words to lines //
		int y = 0;
		int x = 0;
		mLines.clear();
		string line;

		for (int i = 0; i < mWords.size(); i++) {
			if (x + mWords[i].rect.width < bounds.width) {
				mWords[i].rect.x = x;
				mWords[i].rect.y = y;
				x += mWords[i].rect.width + 6;
				line.append(mWords[i].text);
				if (ofStringTimesInString(mWords[i].text, "\n") > 0) {
					y += mLineHeight + 16;
					mWords[i].rect.x = 0;
					mWords[i].rect.y = y;
					x = 0;
					ofStringReplace(line, "\n", "");
					mLines.push_back(line);
					line.clear();
				}
			}
			else {
				y += mLineHeight + 16;
				mWords[i].rect.x = 0;
				mWords[i].rect.y = y;
				x = mWords[i].rect.width + 6;
				ofStringReplace(line, "\n", "");
				mLines.push_back(line);
				line.clear();
				line.append(mWords[i].text);
			}
		}
		// append the last line //
		mLines.push_back(line);
		text.clear();
		for (int i = 0; i < mLines.size(); i++) {
			if (i < mLines.size() - 1)
				text.append(mLines[i] + "\n");
			else
				text.append(mLines[i]);
		}
	}
	else {
		if (multiline) {
			mLines = ofSplitString(text, "\n");
		}
		else {
			mLines.push_back(text);
		}
	}

}
