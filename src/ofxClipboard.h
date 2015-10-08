#pragma once
/*

	Adapted from Cinder's clipboard implementation

*/
#include "ofMain.h"


class ofxClipboard {
public:
	ofxClipboard();

	bool hasString();

	string getString();

	void setString(string clipstr);
};

