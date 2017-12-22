#include "SheetModel.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

XCellModel::XCellModel(XSheetModel *sh) {
	mSheet = sh;
	mColor = XGlobalInfo::DEF_COLOR;
	mBgColor = XGlobalInfo::DEF_BGCOLOR;
	mRowHeader = NULL;
	mColHeader = NULL;
	mFont = NULL;
	mRowSpan = mColSpan = 1;
	mMerger = NULL;
}
void XCellModel::destroy() {
	delete this;
}
XFont * XCellModel::getFont() {
	if (mFont != NULL) {
		return mFont;
	}
	mFont = XGlobalInfo::getDefaultFont();
	return mFont;
}
int XCellModel::getRow() {
	return mRowHeader->mRow;
}
int XCellModel::getCol() {
	return mColHeader->mCol;
}
XRange XCellModel::getRange() {
	return XRange(getRow(), getCol(), getRow() + mRowSpan - 1, getCol() + mColSpan - 1);
}

//-----------------------------------------------
XRowHeaderModel::XRowHeaderModel(XSheetModel *sh) : XCellModel(sh) {
	mRow = 0;
	mRowHeight = XGlobalInfo::DEF_ROW_HEIGHT;
	mRowCells = NULL;
}
int XRowHeaderModel::getRowHeight() {
	return mRowHeight;
}

XColHeaderModel::XColHeaderModel(XSheetModel *sh) : XCellModel(sh) {
	mCol = 0;
	mColWidth = XGlobalInfo::DEF_COL_WIDTH;
}
int XColHeaderModel::getColWidth() {
	return mColWidth;
}

//-----------------------------------------------
XSheetModel::XSheetModel() {
	memset(mRowHeaders, 0, sizeof(mRowHeaders));
	memset(mColHeaders, 0, sizeof(mColHeaders));
	mRowCount = mColCount = 0;
}
XRowHeaderModel* XSheetModel::getRowHeader(int row, bool create) {
	if (row < 0 || row >= XRange::MAX_ROW ) {
		return NULL;
	}
	if (!create && row >= mRowCount) {
		return NULL;
	}
	XRowHeaderModel *hdr = mRowHeaders[row];
	if (hdr == NULL && create) {
		hdr = mRowHeaders[row] = new XRowHeaderModel(this);
		hdr->mRow = row;
	}
	return hdr;
}
XColHeaderModel * XSheetModel::getColHeader(int col, bool create) {
	if (col < 0 || col >= XRange::MAX_COL ) {
		return NULL;
	}
	if (!create && col >= mColCount) {
		return NULL;
	}
	XColHeaderModel *hdr = mColHeaders[col];
	if (hdr == NULL && create) {
		hdr = mColHeaders[col] = new XColHeaderModel(this);
		hdr->mCol = col;
	}
	return hdr;
}
XCellModel* XSheetModel::getCellAt(int row, int col, bool create) {
	if (row < 0 || col < 0 || row >= XRange::MAX_ROW || col >= XRange::MAX_COL) {
		return NULL;
	}
	if (!create && (row >= mRowCount || col >= mColCount)) {
		return NULL;
	}
	XRowHeaderModel *rowHdr = getRowHeader(row, create);
	if (rowHdr == NULL && !create) {
		return NULL;
	}
	XColHeaderModel *colHdr = getColHeader(col, create);
	if (colHdr == NULL && !create) {
		return NULL;
	}
	if (rowHdr->mRowCells == NULL) {
		if (!create) return NULL;
		rowHdr->mRowCells = (XCellModel**)malloc(sizeof(XCellModel*) * XRange::MAX_COL);
		memset(rowHdr->mRowCells, 0, sizeof(XCellModel*) * XRange::MAX_COL);
	}
	XCellModel** items = rowHdr->mRowCells;
	if (items[col] == NULL && create) {
		XCellModel *it = items[col] = new XCellModel(this);
		it->mRowHeader = rowHdr;
		it->mColHeader = colHdr;
	}
	XCellModel *it = items[col];
	/*if (it->getRowSpan() == 0) {
		// it was a merged cell
		it = it->mMerger;
	}*/
	return it;
}
XSheetModel::~XSheetModel() {

}
void XSheetModel::destroy() {
	delete this;
}
XCellModel * XSheetModel::mergeCell(XRange *r) {
	if (r == NULL) return NULL;
	XPos lt = r->leftTop();
	XCellModel *cell = getCellAt(lt.mRow, lt.mCol, true);
	if (cell->getRowSpan() == 0) {
		// it was a merged cell
		cell = cell->mMerger;
	}
	XRange cur = cell->getRange();
	cur = cur.unionRange(r);
	lt = cur.leftTop();
	XPos rb = cur.rightBottom();
	XCellModel *merger = getCellAt(lt.mRow, lt.mCol, true);
	merger->mMerger = NULL;
	merger->mRowSpan = rb.mRow - lt.mRow + 1;
	merger->mColSpan = rb.mCol - lt.mCol + 1;
	for (int i = lt.mRow; i <= rb.mRow; ++i) {
		for (int j = lt.mCol; j <= rb.mCol; ++j) {
			if (i == lt.mRow && j == lt.mCol) {
				continue;
			}
			cell = getCellAt(i, j, true);
			cell->mRowSpan = cell->mColSpan = 0;
			cell->mMerger = merger;
		}
	}
	return merger;
}
void XSheetModel::unmergeCell(XRange *r) {
	if (r == NULL) return;
	XPos lt = r->leftTop();
	XPos rb = r->rightBottom();
	for (int i = lt.mRow; i <= rb.mRow; ++i) {
		for (int j = lt.mCol; j <= rb.mCol; ++j) {
			XCellModel *cell = getCellAt(i, j, true);
			cell->mRowSpan = cell->mColSpan = 1;
			cell->mMerger = NULL;
		}
	}
}
bool XSheetModel::existMergedCell(XRange *r) {
	if (r == NULL) return false;
	XPos lt = r->leftTop();
	XPos rb = r->rightBottom();
	for (int i = lt.mRow; i <= rb.mRow; ++i) {
		for (int j = lt.mCol; j <= rb.mCol; ++j) {
			XCellModel *cell = getCellAt(i, j, false);
			if (cell == NULL) continue;
			if (cell->getRowSpan() != 1) {
				return true;
			}
		}
	}
	return false;
}
