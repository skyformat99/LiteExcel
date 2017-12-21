#include "XArrowButton.h"
#include "XmlParser.h"
#include "UIFactory.h"

static XComponent *XArrowButton_Creator(XmlNode *n) {return new XArrowButton(n);}
static XComponent *XArrowColorButton_Creator(XmlNode *n) {return new XArrowColorButton(n);}

XArrowButton::XArrowButton(XmlNode *node) : XExtButton(node) {
	mMouseAtArrow = false;
	mArrowWidth = 0;
	for (int i = 0; i < mNode->getAttrsCount(); ++i) {
		XmlNode::Attr *attr = mNode->getAttr(i);
		if (strcmp(attr->mName, "arrowPushImage") == 0) {
			mImages[BTN_ARROW_IMG_PUSH] = XImage::load(attr->mValue);
		} else if (strcmp(attr->mName, "arrowHoverImage") == 0) {
			mImages[BTN_ARROW_IMG_HOVER] = XImage::load(attr->mValue);
		} else if (strcmp(attr->mName, "arrowWidth") == 0) {
			mArrowWidth = AttrUtils::parseInt(attr->mValue);
		}
	}
}

bool XArrowButton::wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(mWnd, &ps);
		RECT r = {0, 0, mWidth, mHeight};

		XImage *cur = mImages[getBtnImage()];
		if (cur != NULL)
			cur->draw(dc, 0, 0, mWidth, mHeight);

		if (mAttrFlags & AF_COLOR)
			SetTextColor(dc, mAttrColor);
		SetBkMode(dc, TRANSPARENT);
		SelectObject(dc, getFont());
		DrawText(dc, mNode->getAttrValue("text"), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EndPaint(mWnd, &ps);
		return true;
	} else if (msg == WM_LBUTTONDOWN) {
		mIsMouseDown = true;
		mIsMouseMoving = false;
		mIsMouseLeave = false;
		mMouseAtArrow = isPointInArrow((short)LOWORD(lParam), (short)HIWORD(lParam));
		InvalidateRect(mWnd, NULL, TRUE);
		SetCapture(mWnd);
		if (mEnableFocus) SetFocus(mWnd);
		return true;
	} else if (msg == WM_LBUTTONUP) {
		bool md = mIsMouseDown;
		mIsMouseDown = false;
		mIsMouseMoving = false;
		mMouseAtArrow = isPointInArrow((short)LOWORD(lParam), (short)HIWORD(lParam));
		ReleaseCapture();
		InvalidateRect(mWnd, NULL, TRUE);
		UpdateWindow(mWnd);
		POINT pt = {(LONG)(short)LOWORD(lParam), (LONG)(short)HIWORD(lParam)};
		RECT r = {0, 0, mWidth, mHeight};
		if (md && PtInRect(&r, pt)) {
			SendMessage(mWnd, WM_COMMAND_SELF, 0, 0);
		}
		return true;
	} else if (msg == WM_MOUSEMOVE) {
		if (! mIsMouseMoving) {
			TRACKMOUSEEVENT a = {0};
			a.cbSize = sizeof(TRACKMOUSEEVENT);
			a.dwFlags = TME_LEAVE;
			a.hwndTrack = mWnd;
			TrackMouseEvent(&a);
		}

		BtnImage bi = getBtnImage();
		POINT pt = {(LONG)(short)LOWORD(lParam), (LONG)(short)HIWORD(lParam)};
		RECT r = {0, 0, mWidth, mHeight};
		if (PtInRect(&r, pt)) {
			mIsMouseMoving = true;
			mIsMouseLeave = false;
		} else {
			// mouse leave ( mouse is down now)
			mIsMouseMoving = false;
			mIsMouseLeave = true;
		}
		mMouseAtArrow = isPointInArrow((short)LOWORD(lParam), (short)HIWORD(lParam));
		if (bi != getBtnImage()) {
			InvalidateRect(mWnd, NULL, TRUE);
		}
		return true;
	} else if (msg == WM_MOUSELEAVE) {
		mIsMouseMoving = false;
		mIsMouseLeave = true;
		mMouseAtArrow = isPointInArrow((short)LOWORD(lParam), (short)HIWORD(lParam));
		InvalidateRect(mWnd, NULL, TRUE);
		return true;
	}
	return XExtComponent::wndProc(msg, wParam, lParam, result);
}

void XArrowButton::init() {
	UIFactory::registCreator("ArrowButton", XArrowButton_Creator);
}

bool XArrowButton::isPointInArrow(int x, int y) {
	return y >= 0 && y < mHeight &&
		x >= mWidth - mArrowWidth && x < mWidth;
}

XArrowButton::BtnImage XArrowButton::getBtnImage() {
	if (GetWindowLong(mWnd, GWL_STYLE) & WS_DISABLED)
		return BTN_IMG_DISABLE;
	if (mIsMouseDown && ! mIsMouseLeave) {
		if (mMouseAtArrow)
			return BtnImage(BTN_ARROW_IMG_PUSH);
		return BTN_IMG_PUSH;
	}
	if (!mIsMouseDown && mIsMouseMoving) {
		if (mMouseAtArrow)
			return BtnImage(BTN_ARROW_IMG_HOVER);
		return BTN_IMG_HOVER;
	}
	if (mIsMouseLeave) {
		return BTN_IMG_NORMAL;
	}

	return BTN_IMG_NORMAL;
}

XArrowColorButton::XArrowColorButton(XmlNode *node) : XArrowButton(node) {
	mSelectColor = RGB(0, 0, 0);
	mEmptyColor = AttrUtils::parseBool(mNode->getAttrValue("emptyColor"));
	mSelectColorBrush = NULL;
	mEmptyOutBrush = CreateSolidBrush(RGB(0x7b, 0x7b, 0x7b));
	mEmptyInnerBrush = CreateSolidBrush(RGB(0xfd, 0xfd, 0xfd));
}

COLORREF XArrowColorButton::getSelectColor() {
	return mSelectColor;
}

void XArrowColorButton::setSelectColor(COLORREF color) {
	if (mSelectColor != color && mSelectColorBrush != NULL) {
		DeleteObject(mSelectColorBrush);
		mSelectColorBrush = NULL;
	}
	mSelectColor = color;
}

bool XArrowColorButton::wndProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(mWnd, &ps);
		RECT r = {0, 0, mWidth, mHeight};

		XImage *cur = mImages[getBtnImage()];
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
	return XArrowButton::wndProc(msg, wParam, lParam, result);
}

void XArrowColorButton::init() {
	UIFactory::registCreator("ArrowColorButton", XArrowColorButton_Creator);
}

bool XArrowColorButton::isEmptyColor() {
	return mEmptyColor;
}

void XArrowColorButton::setEmptyColor(bool empty) {
	mEmptyColor = empty;
}

XArrowColorButton::~XArrowColorButton() {
	if (mSelectColorBrush) {
		DeleteObject(mSelectColorBrush);
	}
	DeleteObject(mEmptyOutBrush);
	DeleteObject(mEmptyInnerBrush);
}
