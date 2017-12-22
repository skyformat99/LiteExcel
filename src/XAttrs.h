#pragma once
#include <windows.h>

class XFont {
public:
	char *getFaceName();
	int getSize();
	bool isBold();
	bool isItalic();
	bool isUnderline();
	bool isStrikeout();
	HFONT getHFont();
protected:
	XFont(LOGFONT *f);
	LOGFONT mLogFont;
	HFONT mHFont;
	int mID;
	friend class XFontManager;
};

class XFontManager {
public:
	static XFont *getFont(const char *faceName, int size, bool b, bool i, bool u, bool strikeout);
	static LOGFONT *getLocalFonts(int *num);
protected:
	XFontManager();
	XFont *mFonts[256];
	int mFontNum, mLocalFontNum;
};

struct XAnchor {
	enum HorAnchor {LEFT, HCENTER, RIGHT};
	enum VerAnchor {TOP, VCENTER, BOTTOM};
	XAnchor() {mHorAnchor = (short)LEFT; mVerAnchor = (short)VCENTER;}
	short mHorAnchor;
	short mVerAnchor;
};

class XGlobalInfo {
public:
	const static int DEF_ROW_HEIGHT = 25;
	const static int DEF_COL_WIDTH = 80;
	const static int DEF_COLOR = RGB(0, 0, 0);
	const static int DEF_BGCOLOR = RGB(0xfa, 0xfa, 0xfa);
	const static int DEF_FONT_SIZE = 12;
	static XFont* getDefaultFont();
};

struct XPos {
	XPos();
	int mRow, mCol;
	inline bool operator == (const XPos &r2);
	// change to example : A12
	void toString(char *strBuf);
	// parse A12 to (11, 0)
	static XPos prase(const char *str);
};

class XRange {
public:
	XRange();
	XRange(int startRow, int startCol, int endRow, int endCol);
	int mStartRow, mStartCol; // first cell
	int mEndRow, mEndCol; // last cell

	const static int MAX_ROW = 5000;
	const static int MAX_COL = 64;

	XPos leftTop();
	XPos rightBottom();

	bool isValid();
	// whether constains a Cell
	bool constains(int row, int col);
	XRange unionRange(XRange *r);
	XRange intersectionRange(XRange *r);
	// whether full constains another Range r
	bool fullConstains(XRange *r);
	bool hasIntersection(XRange *r);
	bool isEquals(XRange *r);
	bool hasSameCells(XRange *r);
};

class Ref {
public:
	Ref();
	void incRef();
	void decRef();
	virtual ~Ref() {}
protected:
	virtual void destroy() = 0;
	int mRefCount;
};