#pragma once
#include <windows.h>
#include "XAttrs.h"
#include "Manager.h"
#include "XString.h"

class XRowHeaderModel;
class XColHeaderModel;
class XSheetModel;

class XCellModel : public Ref {
public:
	XCellModel(XSheetModel *sh);
	int getRow();
	int getCol();
	XFont *getFont();
	void setFont(XFont *font) {mFont = font;}
	int getRowSpan() {return mRowSpan;}
	int getColSpan() {return mColSpan;}
	XRange getRange();
	COLORREF getColor() {return mColor;}
	void setColor(COLORREF color) {mColor = color;}
	COLORREF getBgColor() {return mBgColor;}
	void setBgColor(COLORREF bg) {mBgColor = bg;}
	void clearBgColor() {mBgColor = XGlobalInfo::DEF_CELL_BGCOLOR;}
	bool isBgColorEmpty() {return mBgColor == XGlobalInfo::DEF_CELL_BGCOLOR;}
	char *getText() {return mText.str();}
	void setText(const char *text) {mText = text;}

protected:
	virtual void destroy();
protected:
	friend class XSheetModel;
	XSheetModel *mSheet;
	XFont *mFont;
	COLORREF mColor, mBgColor;
	XHorAlign mHorAlign;
	XVerAlign mVerAlign;
	XBorder mBorder;
	bool mAutoLinefeed; // ×Ô¶¯»»ÐÐ
	XString mText;

	XRowHeaderModel *mRowHeader;
	XColHeaderModel *mColHeader;
	// default is 1; 0 means has been merged
	int mRowSpan, mColSpan;
	XCellModel *mMerger;
};

class XRowHeaderModel : public XCellModel {
public:
	XRowHeaderModel(XSheetModel *sh);
	int getRowHeight();
protected:
	friend class XSheetModel;
	friend class XCellModel;
	int mRowHeight;
	int mRow;
	XCellModel **mRowCells; // an array, has XRange::MAX_COL items
};

class XColHeaderModel : public XCellModel {
public:
	XColHeaderModel(XSheetModel *sh);
	int getColWidth();
protected:
	friend class XSheetModel;
	friend class XCellModel;
	int mColWidth;
	int mCol;
};

class XSheetModel : public Ref {
public:
	XSheetModel();
	XRowHeaderModel *getRowHeader(int row, bool create);
	XColHeaderModel *getColHeader(int col, bool create);
	XCellModel *getCellAt(int row, int col, bool create);
	XCellModel *mergeCell(XRange *r); // return the merger
	void unmergeCell(XRange *r);
	bool existMergedCell(XRange *r);
	int getRowCount();
	int getColCount();
	virtual ~XSheetModel();
protected:
	virtual void destroy();
protected:
	XRowHeaderModel *mRowHeaders[XRange::MAX_ROW];
	XColHeaderModel *mColHeaders[XRange::MAX_COL];
	int mRowCount, mColCount;
};



