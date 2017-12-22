#pragma once
#include "XExt.h"
#include <windef.h>

class ArrowColorButton : public XExtArrowButton {
public:
	ArrowColorButton(XmlNode *node);
	virtual bool wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
	static void init();
	COLORREF getSelectColor();
	void setSelectColor(COLORREF color);
	bool isEmptyColor();
	void setEmptyColor(bool empty);
	virtual ~ArrowColorButton();
protected:
	COLORREF mSelectColor;
	HBRUSH mSelectColorBrush, mEmptyOutBrush, mEmptyInnerBrush;
	bool mEmptyColor;
};

class AnchorButton : public XExtOption {
public:
	enum Anchor {
		ANCHOR_NONE = 0,
		ANCHOR_LEFT, ANCHOR_HCENTER, ANCHOR_RIGHT,
		ANCHOR_TOP, ANCHOR_VCENTER, ANCHOR_BOTTOM
	};
	AnchorButton(XmlNode *node);
	virtual bool wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
	static void init();
	Anchor getAnchor();
	void setAnchor(Anchor anchor);
	virtual ~AnchorButton();
protected:
	HPEN mLinePen;
	Anchor mAnchor;
};


