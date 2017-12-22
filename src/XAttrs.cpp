#include "XAttrs.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "XExt.h"

extern XWindow *GetMainWin();

XFont::XFont(LOGFONT *f) {
	mLogFont = *f;
	mID = -1;
	mHFont = NULL;
}
char * XFont::getFaceName() {
	return mLogFont.lfFaceName;
}
int XFont::getSize() {
	return mLogFont.lfHeight;
}
bool XFont::isBold() {
	return mLogFont.lfWeight > 400;
}
bool XFont::isItalic() {
	return mLogFont.lfItalic;
}
bool XFont::isUnderline() {
	return mLogFont.lfUnderline;
}
bool XFont::isStrikeout() {
	return mLogFont.lfStrikeOut;
}
HFONT XFont::getHFont() {
	if (mHFont == NULL) {
		mHFont = CreateFontIndirect(&mLogFont);
	}
	return mHFont;
}

static XFontManager *mFontMgr;
static LOGFONT sLocalFonts[64];

static BOOL CALLBACK EnumFontsCallBack(CONST LOGFONT* lplf, CONST TEXTMETRIC *lptm,DWORD dwType,LPARAM lparam) {
	static char *filter[] = {"宋体", "新宋体", "仿宋", "楷体", "方正舒体", "隶书", "华文宋体", "华文行楷",
		"Consolas", "Courier", "Courier New", "Roman", "Terminal", "Times New Roman", "MS Serif"};
	int idx = *(int*)lparam;
	if (strchr(lplf->lfFaceName, '@') != NULL) {
		return true;
	}
	for (int i = 0; i < sizeof(filter)/sizeof(char*); ++i) {
		if (strcmp(filter[i], lplf->lfFaceName) == 0) {
			sLocalFonts[idx] = *lplf;
			*(int*)lparam = idx + 1;
			// printf("[%3d] [%s] \n", idx, lplf->lfFaceName);
			break;
		}
	}
	return idx < sizeof(sLocalFonts)/sizeof(LOGFONT);
}
XFontManager::XFontManager() {
	mFontNum = 0;
	mLocalFontNum = 0;
	HWND wnd = GetMainWin()->getWnd();
	HDC dc = GetDC(wnd);
	EnumFonts(dc, NULL, EnumFontsCallBack, (LPARAM)&mLocalFontNum);
	ReleaseDC(wnd, dc);
}

XFont * XFontManager::getFont(const char *faceName, int size, bool b, bool i, bool u, bool strikeout) {
	if (faceName == NULL) {
		return NULL;
	}
	if (mFontMgr == NULL) mFontMgr = new XFontManager();
	for (int i = 0; i < mFontMgr->mFontNum; ++i) {
		XFont *f = mFontMgr->mFonts[i];
		if (f->getSize() != size || f->isBold() != b || f->isItalic() != i
			|| f->isUnderline() != u || f->isStrikeout() != strikeout) {
			continue;
		}
		if (strcmp(f->getFaceName(), faceName) == 0) {
			// here find cache font
			return f;
		}
	}

	// not find cache font
	for (int i = 0; i < mFontMgr->mLocalFontNum; ++i) {
		if (strcmp(sLocalFonts[i].lfFaceName, faceName) != 0) {
			continue;
		}
		XFont *f = new XFont(&sLocalFonts[i]);
		mFontMgr->mFonts[mFontMgr->mFontNum++] = f;
		return f;
	}
	return NULL;
}

LOGFONT* XFontManager::getLocalFonts(int *num) {
	if (mFontMgr == NULL) mFontMgr = new XFontManager();
	if (num != NULL) *num = mFontMgr->mLocalFontNum;
	return sLocalFonts;
}

//-------------------------------------------------------------
XPos::XPos() {
	mRow = mCol = 0;
}
bool XPos::operator == (const XPos &r2) {
	return mRow == r2.mRow && mCol == r2.mCol;
}
void XPos::toString(char *strBuf) {
	if (strBuf == NULL) return;
	*strBuf = 0;
	char *p = strBuf;
	int c = mCol;
	char tmp[8] = {0};
	char *t = &tmp[7];
	while (c > 0) {
		--t;
		*t = (c % 26) + 'A';
		c /= 26;
	}
	strcpy(strBuf, tmp);
	p = strBuf + strlen(strBuf);
	sprintf(p, "%d", mRow + 1);
}
XPos XPos::prase(const char *str) {
	XPos v;
	if (str == NULL) {
		return v;
	}
	int r = 0, c = 0;
	const char *p = str;
	while (toupper(*p) >= 'A' && toupper(*p) <= 'Z') {
		c = c * 26 + (toupper(*p) - 'A' + 1);
	}
	--c;
	while (*p >= '0' && *p <= '9') {
		r = r * 10 + (*p - '1' + 1);
	}
	--r;
	v.mRow = r;
	v.mCol = c;
	return v;
}
//-----------------------------------------------
XRange::XRange() {
	mStartRow = mStartCol = -1;
	mEndRow = mEndCol = -1;
}
XRange::XRange(int startRow, int startCol, int endRow, int endCol) {
	mStartRow = startRow;
	mStartCol = startCol;
	mEndRow = endRow;
	mEndCol = endCol;
}
XPos XRange::leftTop() {
	XPos v;
	v.mRow = __min(mStartRow, mEndRow);
	v.mCol = __min(mStartCol, mEndCol);
	return v;
}
XPos XRange::rightBottom() {
	XPos v;
	v.mRow = __max(mStartRow, mEndRow);
	v.mCol = __max(mStartCol, mEndCol);
	return v;
}
bool XRange::isValid() {
	if (mStartRow < 0 || mStartCol < 0 || mEndRow < 0 || mEndCol < 0)
		return false;
	if (mStartRow >= MAX_ROW || mEndRow >= MAX_ROW || mStartCol >= MAX_COL || mEndCol >= MAX_COL)
		return false;
	return true;
}
bool XRange::constains(int row, int col) {
	if (row < 0 || col < 0)
		return false;
	if (! isValid())
		return false;
	int r1 = __min(mStartRow, mEndRow), r2 = __max(mStartRow, mEndRow);
	int c1 = __min(mStartCol, mEndCol), c2 = __max(mStartCol, mEndCol);
	return row >= r1 && row <= r2 && col >= c1 && col <= c2;
}
XRange XRange::unionRange(XRange *r) {
	if (r == NULL || !r->isValid())
		return *this;
	if (!isValid())
		return *r;
	XRange ret;
	ret.mStartRow = __min(leftTop().mRow, r->leftTop().mRow);
	ret.mEndRow = __max(rightBottom().mRow, r->rightBottom().mRow);
	ret.mStartCol = __min(leftTop().mCol, r->leftTop().mCol);
	ret.mEndCol = __max(rightBottom().mCol, r->rightBottom().mCol);
	return ret;
}
XRange XRange::intersectionRange(XRange *r) {
	if (r == NULL || !r->isValid() || !isValid())
		return XRange();
	XRange u = unionRange(r);
	XRange v;
	v.mStartRow = leftTop().mRow == u.mStartRow ? r->leftTop().mRow : leftTop().mRow;
	v.mEndRow = rightBottom().mRow == u.mStartRow ? r->rightBottom().mRow : rightBottom().mRow;
	v.mStartCol = leftTop().mCol == u.mStartRow ? r->leftTop().mCol : leftTop().mCol;
	v.mEndCol = rightBottom().mCol == u.mStartRow ? r->rightBottom().mCol : rightBottom().mCol;
	if (v.mStartRow > v.mEndRow || v.mStartCol > v.mEndCol) {
		return XRange();
	}
	return v;
}
bool XRange::fullConstains(XRange *r) {
	if (r == NULL || !r->isValid() || !isValid()) 
		return false;
	XRange u = unionRange(r);
	return hasSameCells(&u);
}
bool XRange::hasIntersection(XRange *r) {
	XRange v = intersectionRange(r);
	return v.isValid();
}
bool XRange::isEquals(XRange *r) {
	if (r == NULL || !r->isValid() || !isValid())
		return false;
	return mStartRow == r->mStartRow && mEndRow == r->mEndRow &&
		mStartCol == r->mStartCol && mEndCol == r->mEndCol;
}
bool XRange::hasSameCells(XRange *r) {
	if (r == NULL || !r->isValid() || !isValid())
		return false;
	return leftTop() == r->leftTop() && 
		rightBottom() == r->rightBottom();
}
int XRange::getRowSpan() {
	if (! isValid()) return 0;
	XPos lt = leftTop();
	XPos rb = rightBottom();
	return rb.mRow - lt.mRow + 1;
}
int XRange::getColSpan() {
	if (! isValid()) return 0;
	XPos lt = leftTop();
	XPos rb = rightBottom();
	return rb.mCol - lt.mCol + 1;
}

//-----------------------------------------------
Ref::Ref() {
	mRefCount = 1;
}
void Ref::incRef() {
	++mRefCount;
}
void Ref::decRef() {
	--mRefCount;
	if (mRefCount == 0) {
		destroy();
	}
}
void Ref::destroy() {
	delete this;
}
//-----------------------------------------------
XFont *XGlobalInfo::getDefaultFont() {
	// TODO:
	return NULL;
}
