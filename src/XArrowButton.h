#pragma once
#include "XExt.h"
#include <windef.h>

class XArrowButton : public XExtButton {
public:
	XArrowButton(XmlNode *node);
	virtual bool wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result);
	static void init();
	virtual BtnImage getBtnImage();
protected:
	bool isPointInArrow(int x, int y);
	enum {
		BTN_ARROW_IMG_HOVER = 5,
		BTN_ARROW_IMG_PUSH = 6
	};
	bool mMouseAtArrow;
	int mArrowWidth;
};

class XArrowColorButton : public XArrowButton {
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

