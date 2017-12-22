#include "OptButtons.h"
#include "XmlParser.h"
#include "UIFactory.h"

static XComponent *XArrowColorButton_Creator(XmlNode *n) {return new ArrowColorButton(n);}
static XComponent *XAnchorButton_Creator(XmlNode *n) {return new AnchorButton(n);}

ArrowColorButton::ArrowColorButton(XmlNode *node) : XExtArrowButton(node) {
	mSelectColor = RGB(0, 0, 0);
	mEmptyColor = AttrUtils::parseBool(mNode->getAttrValue("emptyColor"));
	mSelectColorBrush = NULL;
	mEmptyOutBrush = CreateSolidBrush(RGB(0x7b, 0x7b, 0x7b));
	mEmptyInnerBrush = CreateSolidBrush(RGB(0xfd, 0xfd, 0xfd));
}

COLORREF ArrowColorButton::getSelectColor() {
	return mSelectColor;
}

void ArrowColorButton::setSelectColor(COLORREF color) {
	if (mSelectColor != color && mSelectColorBrush != NULL) {
		DeleteObject(mSelectColorBrush);
		mSelectColorBrush = NULL;
	}
	mSelectColor = color;
}

bool ArrowColorButton::wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(mWnd, &ps);
		RECT r = {0, 0, mWidth, mHeight};

		XImage *cur = mStateImages[getStateImage()];
		if (cur != NULL) {
			cur->draw(dc, 0, 0, mWidth, mHeight);
		}
		if (mSelectColorBrush == NULL) {
			mSelectColorBrush = CreateSolidBrush(mSelectColor);
		}
		RECT sr = {5, 15, 19, 19};
		if (! mEmptyColor) {
			FillRect(dc, &sr, mSelectColorBrush);
		} else {
			FillRect(dc, &sr, mEmptyOutBrush);
			RECT ir = {6, 16, 18, 18};
			FillRect(dc, &ir, mEmptyInnerBrush);
		}
		
		EndPaint(mWnd, &ps);
		return true;
	}
	return XExtArrowButton::wndProc(msg, wParam, lParam, result);
}

void ArrowColorButton::init() {
	UIFactory::registCreator("ArrowColorButton", XArrowColorButton_Creator);
}

bool ArrowColorButton::isEmptyColor() {
	return mEmptyColor;
}

void ArrowColorButton::setEmptyColor(bool empty) {
	mEmptyColor = empty;
}

ArrowColorButton::~ArrowColorButton() {
	if (mSelectColorBrush) {
		DeleteObject(mSelectColorBrush);
	}
	DeleteObject(mEmptyOutBrush);
	DeleteObject(mEmptyInnerBrush);
}

AnchorButton::AnchorButton(XmlNode *node) : XExtOption(node) {
	mAutoSelect = false;
	mLinePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	mAnchor = (Anchor)AttrUtils::parseInt(mNode->getAttrValue("anchor"));
}

bool AnchorButton::wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(mWnd, &ps);
		RECT r = {0, 0, mWidth, mHeight};

		XImage *cur = mStateImages[getStateImage()];
		if (cur != NULL) {
			cur->draw(dc, 0, 0, mWidth, mHeight);
		}
		SelectObject(dc, mLinePen);
		switch (mAnchor) {
		case ANCHOR_TOP:
			MoveToEx(dc, 5, 4, NULL);
			LineTo(dc, 18, 4);
			MoveToEx(dc, 6, 6, NULL);
			LineTo(dc, 17, 6);
			MoveToEx(dc, 5, 8, NULL);
			LineTo(dc, 18, 8);
			MoveToEx(dc, 6, 10, NULL);
			LineTo(dc, 17, 10);
			break;
		case ANCHOR_VCENTER:
			MoveToEx(dc, 5, 8, NULL);
			LineTo(dc, 18, 8);
			MoveToEx(dc, 6, 10, NULL);
			LineTo(dc, 17, 10);
			MoveToEx(dc, 5, 12, NULL);
			LineTo(dc, 18, 12);
			MoveToEx(dc, 6, 14, NULL);
			LineTo(dc, 18, 14);
			break;
		case ANCHOR_BOTTOM:
			MoveToEx(dc, 3, 11, NULL);
			LineTo(dc, 16, 11);
			MoveToEx(dc, 4, 13, NULL);
			LineTo(dc, 15, 13);
			MoveToEx(dc, 3, 15, NULL);
			LineTo(dc, 16, 15);
			MoveToEx(dc, 4, 17, NULL);
			LineTo(dc, 15, 17);
			break;
		case ANCHOR_LEFT:
			MoveToEx(dc, 5, 5, NULL);
			LineTo(dc, 18, 5);
			MoveToEx(dc, 5, 7, NULL);
			LineTo(dc, 14, 7);
			MoveToEx(dc, 5, 9, NULL);
			LineTo(dc, 18, 9);
			MoveToEx(dc, 5, 11, NULL);
			LineTo(dc, 14, 11);
			MoveToEx(dc, 5, 13, NULL);
			LineTo(dc, 18, 13);
			MoveToEx(dc, 5, 15, NULL);
			LineTo(dc, 14, 15);
			break;
		case ANCHOR_HCENTER:
			MoveToEx(dc, 5, 5, NULL);
			LineTo(dc, 18, 5);
			MoveToEx(dc, 7, 7, NULL);
			LineTo(dc, 16, 7);
			MoveToEx(dc, 5, 9, NULL);
			LineTo(dc, 18, 9);
			MoveToEx(dc, 7, 11, NULL);
			LineTo(dc, 16, 11);
			MoveToEx(dc, 5, 13, NULL);
			LineTo(dc, 18, 13);
			MoveToEx(dc, 7, 15, NULL);
			LineTo(dc, 16, 15);
			break;
		case ANCHOR_RIGHT:
			MoveToEx(dc, 4, 5, NULL);
			LineTo(dc, 17, 5);
			MoveToEx(dc, 8, 7, NULL);
			LineTo(dc, 17, 7);
			MoveToEx(dc, 4, 9, NULL);
			LineTo(dc, 17, 9);
			MoveToEx(dc, 8, 11, NULL);
			LineTo(dc, 17, 11);
			MoveToEx(dc, 4, 13, NULL);
			LineTo(dc, 17, 13);
			MoveToEx(dc, 8, 15, NULL);
			LineTo(dc, 17, 15);
			break;
		}

		EndPaint(mWnd, &ps);
		return true;
	}
	return XExtOption::wndProc(msg, wParam, lParam, result);
}

void AnchorButton::init() {
	UIFactory::registCreator("AnchorButton", XAnchorButton_Creator);
}

AnchorButton::Anchor AnchorButton::getAnchor() {
	return mAnchor;
}

void AnchorButton::setAnchor(Anchor anchor) {
	mAnchor = anchor;
}

AnchorButton::~AnchorButton() {
	DeleteObject(mLinePen);
}
