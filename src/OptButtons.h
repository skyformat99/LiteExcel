#pragma once
#include "XExt.h"
#include <windef.h>

class XArrowColorButton : public XExtArrowButton {
public:
	XArrowColorButton(XmlNode *node);
	virtual bool wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
	static void init();
	COLORREF getSelectColor();
	void setSelectColor(COLORREF color);
	bool isEmptyColor();
	void setEmptyColor(bool empty);
	virtual ~XArrowColorButton();
protected:
	COLORREF mSelectColor;
	HBRUSH mSelectColorBrush, mEmptyOutBrush, mEmptyInnerBrush;
	bool mEmptyColor;
};

class XAnchorButton : public XExtOption {
public:
	enum Anchor {
		ANCHOR_NONE = 0,
		ANCHOR_LEFT, ANCHOR_HCENTER, ANCHOR_RIGHT,
		ANCHOR_TOP, ANCHOR_VCENTER, ANCHOR_BOTTOM
	};
	XAnchorButton(XmlNode *node);
	virtual bool wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
	static void init();
	Anchor getAnchor();
	void setAnchor(Anchor anchor);
	virtual ~XAnchorButton();
protected:
	HPEN mLinePen;
	Anchor mAnchor;
};


