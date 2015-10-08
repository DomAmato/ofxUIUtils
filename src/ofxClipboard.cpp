#include "ofxClipboard.h"

#if defined( TARGET_OSX )
#import <Cocoa/Cocoa.h>
#import <AppKit/NSPasteboard.h>
#import <AppKit/NSImage.h>
#elif defined( TARGET_WIN32 )
#include <windows.h>
#include <locale>
#include <sstream>
#include <string>

std::string convertWideToNarrow(const wchar_t *s, char dfault = '?',
	const std::locale& loc = std::locale())
{
	std::ostringstream stm;

	while (*s != L'\0') {
		stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, dfault);
	}
	return stm.str();
}

std::wstring convertNarrowToWide(const std::string& as) {
	// deal with trivial case of empty string
	if (as.empty())    return std::wstring();

	// determine required length of new string
	size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

	// construct new string of required length
	std::wstring ret(reqLength, L'\0');

	// convert old string to new string
	::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

	// return new string ( compiler should optimize this away )
	return ret;
}
#endif


#if defined( TARGET_WIN32 )
bool clipboardContainsFormat(const set<UINT> formats)
{
	bool result = false;
	if (!::OpenClipboard(NULL))
		return false;
	int numFormats = ::CountClipboardFormats();
	for (int f = 0; f < numFormats; ++f) {
		if (formats.find(::EnumClipboardFormats(f)) != formats.end()) {
			result = true;
			break;
		}
	}
	::CloseClipboard();
	return result;
}

#endif

ofxClipboard::ofxClipboard()
{
}

bool ofxClipboard::hasString()
{
#if defined( TARGET_OSX )
	NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
	NSArray *classArray = [NSArray arrayWithObject : [NSString class]];
	NSDictionary *options = [NSDictionary dictionary];

	return[pasteboard canReadObjectForClasses : classArray options : options];
#elif defined( TARGET_WIN32 )
	set<UINT> textFormats;
	textFormats.insert(CF_TEXT); textFormats.insert(CF_UNICODETEXT); textFormats.insert(CF_OEMTEXT);
	return clipboardContainsFormat(textFormats);
#endif
}

string ofxClipboard::getString()
{
#if defined( TARGET_OSX )
	NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
	NSArray *classArray = [NSArray arrayWithObject : [NSString class]];
	NSDictionary *options = [NSDictionary dictionary];

	if ([pasteboard canReadObjectForClasses : classArray options : options]) {
		NSArray *objectsToPaste = [pasteboard readObjectsForClasses : classArray options : options];
		NSString *text = [objectsToPaste firstObject];
		if (!text)
			return string();
		else
			return cocoa::convertNsString(text);
	}
	else {
		return string();
	}

#elif defined( TARGET_WIN32 )
	string result;
	if (!::OpenClipboard(NULL))
		return result;
	HANDLE dataH = ::GetClipboardData(CF_UNICODETEXT);
	if (dataH) {
		wchar_t *pstr = reinterpret_cast<wchar_t*>(::GlobalLock(dataH));
		if (pstr) {
			wstring wstr(pstr);
			result = convertWideToNarrow(wstr.c_str());
			::GlobalUnlock(dataH);
		}
	}
	::CloseClipboard();
	return result;
#endif
}

void ofxClipboard::setString(string str)
{
#if defined( TARGET_OSX )
	[[NSPasteboard generalPasteboard] declareTypes:[NSArray arrayWithObject : NSStringPboardType] owner : nil];
	[[NSPasteboard generalPasteboard] setString:[NSString stringWithUTF8String : str.c_str()] forType : NSStringPboardType];
#elif defined( TARGET_WIN32 )
	if (!::OpenClipboard(NULL)) {
		ofLogError("Clipboard") << "Failed to open clipboard";
		return;
	}
	::EmptyClipboard();
	wstring wstr = convertNarrowToWide(str);
	HANDLE hglbCopy = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(uint16_t) * (wstr.length() + 1));
	// Lock the handle and copy the text to the buffer. 
	void *lptstrCopy = ::GlobalLock(hglbCopy);
	memcpy(lptstrCopy, &wstr[0], sizeof(uint16_t) * (wstr.length() + 1));
	::GlobalUnlock(hglbCopy);
	::SetClipboardData(CF_UNICODETEXT, hglbCopy);
	::CloseClipboard();
#endif
}