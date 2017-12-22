#pragma once
#include "SheetModel.h"

class ModifyCellAction : public XAction {
public:
	enum ModifyFlags {
		MF_FONT = 1, MF_HOR_ALIGN = 2, MF_VER_ALIGN = 4,
		MF_BORDER = 8, MF_COLOR = 16, MF_BGCOLOR = 32
	};
	ModifyCellAction(XCellModel *cell, XFont *newFont, XHorAlign newHorAlign, XVerAlign newVerAlign,
		XBorder newBorder, COLORREF newColor, COLORREF newBgColor, ModifyFlags flags);
	virtual void onDo();
	virtual void onUndo();
protected:
	XCellModel *mCell;
	XFont *mFont[2];
	XHorAlign mHorAlign[2];
	XVerAlign mVerAlign[2];
	XBorder mBorder[2];
	COLORREF mColor[2];
	COLORREF mBgColor[2];
	ModifyFlags mFlags;
};

