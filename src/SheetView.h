#pragma once
#include "XExt.h"
#include "SheetModel.h"

class XSheetView : public XExtComponent {
public:
	XSheetView(XmlNode *node);
	static void init();
	~XSheetView();
protected:
	virtual bool wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
	void drawRowHeader(HDC dc);
	void drawColHeader(HDC dc);
	void drawCells(HDC dc);
	int getCellStartX(int col);
	int getCellStartY(int row);
	void getCellRectAt(int row, int col, RECT *r);
	int getRowHeight(int row);
	int getColWidth(int col);
	virtual void drawCell(HDC dc, int row, int col, XCellModel *cell, RECT *r);
	void drawText(HDC dc, char *text, RECT *r);
protected:
	int mTranslateRow, mTranslateCol; // always >= 0
	XSheetModel *mSheetModel;
	XImage *mSelectAllConnerImage, *mColHeaderImage;
	HFONT mRowHeaderFont, mColHeaderFont;
};

