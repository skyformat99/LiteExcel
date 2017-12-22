#include "Sheet.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

XCell::XCell(XSheet *sh) {
	mSheet = sh;
	mColor = XGlobalInfo::DEF_COLOR;
	mBgColor = XGlobalInfo::DEF_BGCOLOR;
	mRowHeader = NULL;
	mColHeader = NULL;
	mFont = NULL;
}
void XCell::destroy() {
	delete this;
}

XFont * XCell::getFont() {
	if (mFont != NULL) {
		return mFont;
	}
	mFont = XGlobalInfo::getDefaultFont();
	return mFont;
}

/*
int XCell::getRow() {
	// TODO:
}
int XCell::getCol() {
	// TODO:
}*/


//-----------------------------------------------
XRowHeader::XRowHeader(XSheet *sh) : XCell(sh) {
	mRow = 0;
	mRowHeight = XGlobalInfo::DEF_ROW_HEIGHT;
	mRowCells = NULL;
}
XColHeader::XColHeader(XSheet *sh) : XCell(sh) {
	mCol = 0;
	mColWidth = XGlobalInfo::DEF_COL_WIDTH;
}
//-----------------------------------------------
XSheet::XSheet() {
	memset(mRowHeaders, 0, sizeof(mRowHeaders));
	memset(mColHeaders, 0, sizeof(mColHeaders));
	mRowCount = mColCount = 0;
}
XRowHeader* XSheet::getRowHeader(int row, bool create) {
	if (row < 0 || row >= XRange::MAX_ROW ) {
		return NULL;
	}
	if (!create && row >= mRowCount) {
		return NULL;
	}
	XRowHeader *hdr = mRowHeaders[row];
	if (hdr == NULL && create) {
		hdr = mRowHeaders[row] = new XRowHeader(this);
		hdr->mRow = row;
	}
	return hdr;
}
XColHeader * XSheet::getColHeader(int col, bool create) {
	if (col < 0 || col >= XRange::MAX_COL ) {
		return NULL;
	}
	if (!create && col >= mColCount) {
		return NULL;
	}
	XColHeader *hdr = mColHeaders[col];
	if (hdr == NULL && create) {
		hdr = mColHeaders[col] = new XColHeader(this);
		hdr->mCol = col;
	}
	return hdr;
}
XCell* XSheet::getCell(int row, int col, bool create) {
	if (row < 0 || col < 0 || row >= XRange::MAX_ROW || col >= XRange::MAX_COL) {
		return NULL;
	}
	if (!create && (row >= mRowCount || col >= mColCount)) {
		return NULL;
	}
	XRowHeader *rowHdr = getRowHeader(row, create);
	if (rowHdr == NULL && !create) {
		return NULL;
	}
	XColHeader *colHdr = getColHeader(col, create);
	if (colHdr == NULL && !create) {
		return NULL;
	}
	if (rowHdr->mRowCells == NULL) {
		if (!create) return NULL;
		rowHdr->mRowCells = (XCell**)malloc(sizeof(XCell*) * XRange::MAX_COL);
		memset(rowHdr->mRowCells, 0, sizeof(XCell*) * XRange::MAX_COL);
	}
	XCell** items = rowHdr->mRowCells;
	if (items[col] == NULL && create) {
		XCell *it = items[col] = new XCell(this);
		it->mRowHeader = rowHdr;
		it->mColHeader = colHdr;
	}
}
