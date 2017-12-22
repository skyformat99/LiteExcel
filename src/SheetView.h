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
protected:
	int mTranslateRow, mTranslateCol; // always >= 0
	XSheetModel *mSheetModel;
	XImage *mSelectAllConnerImage, *mColHeaderImage;
	HFONT mRowHeaderFont, mColHeaderFont;
};

