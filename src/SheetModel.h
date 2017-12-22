#pragma once
#include <windows.h>
#include "XAttrs.h"
#include "Manager.h"

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

protected:
	virtual void destroy();
protected:
	friend class XSheetModel;
	XSheetModel *mSheet;
	XFont *mFont;
	COLORREF mColor, mBgColor;
	XRowHeaderModel *mRowHeader;
	XColHeaderModel *mColHeader;
	// default is 1; 0 means has been merged
	int mRowSpan, mColSpan;
	XCellModel *mMerger;
};

class XRowHeaderModel : public XCellModel {
public:
	XRowHeaderModel(XSheetModel *sh);
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
	virtual ~XSheetModel();
protected:
	virtual void destroy();
protected:
	XRowHeaderModel *mRowHeaders[XRange::MAX_ROW];
	XColHeaderModel *mColHeaders[XRange::MAX_COL];
	int mRowCount, mColCount;
};



