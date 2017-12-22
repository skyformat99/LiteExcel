#pragma once
#include <windows.h>
#include "XAttrs.h"
#include "Manager.h"

class XRowHeader;
class XColHeader;
class XSheet;

class XCell : public Ref {
public:
	XCell(XSheet *sh);
	/*int getRow();
	int getCol();*/
	XFont *getFont();
	void setFont(XFont *font) {mFont = font;}

protected:
	virtual void destroy();
protected:
	friend class XSheet;
	XSheet *mSheet;
	XFont *mFont;
	COLORREF mColor, mBgColor;
	XRowHeader *mRowHeader;
	XColHeader *mColHeader;
};

class XRowHeader : public XCell {
public:
	XRowHeader(XSheet *sh);
protected:
	friend class XSheet;
	int mRowHeight;
	int mRow;
	XCell **mRowCells; // an array, has XRange::MAX_COL items
};

class XColHeader : public XCell {
public:
	XColHeader(XSheet *sh);
protected:
	friend class XSheet;
	int mColWidth;
	int mCol;
};

class XSheet : public Ref {
public:
	XSheet();
	XRowHeader *getRowHeader(int row, bool create);
	XColHeader *getColHeader(int col, bool create);
	XCell *getCell(int row, int col, bool create);
	~XSheet();
protected:
	XRowHeader *mRowHeaders[XRange::MAX_ROW];
	XColHeader *mColHeaders[XRange::MAX_COL];
	int mRowCount, mColCount;
};

class ModifyCellAction : public Action {
public:
	ModifyCellAction();
	virtual void onDo();
	virtual void onUndo();
};

