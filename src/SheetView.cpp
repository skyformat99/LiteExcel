#include "SheetView.h"
#include "UIFactory.h"
#include "XmlParser.h"

static XComponent *XSheet_Creator(XmlNode *n) {return new XSheetView(n);}
void XSheetView::init() {
	UIFactory::registCreator("Sheet", XSheet_Creator);
}
XSheetView::XSheetView(XmlNode *node) : XExtComponent(node) {
	mEnableMemBuffer = true;
	mTranslateRow = mTranslateCol = 0;
	mSheetModel = new XSheetModel();
	mSelectAllConnerImage = XImage::load(mNode->getAttrValue("selectAllCorner"));
	mColHeaderImage = XImage::load(mNode->getAttrValue("colHeader"));

	LOGFONT ff = {0};
	HFONT font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	GetObject(font, sizeof(LOGFONT), &ff);
	mRowHeaderFont = mColHeaderFont = NULL;
	if (AttrUtils::parseFont(&ff, mNode->getAttrValue("rowHeaderFont"))) {
		mRowHeaderFont = CreateFontIndirect(&ff);
	}
	if (AttrUtils::parseFont(&ff, mNode->getAttrValue("colHeaderFont"))) {
		mColHeaderFont = CreateFontIndirect(&ff);
	}
}
XSheetView::~XSheetView() {

}
bool XSheetView::wndProc( UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *result ) {
	if (msg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(mWnd, &ps);
		HDC memDc = CreateCompatibleDC(dc);
		if (mMemBuffer == NULL) {
			mMemBuffer = XImage::create(mWidth, mHeight);
		}
		SelectObject(memDc, mMemBuffer->getHBitmap());
		drawRowHeader(memDc);
		drawColHeader(memDc);
		printf("rect=[%d %d %d %d] \n", ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom);
		/*BitBlt(dc, ps.rcPaint.left, ps.rcPaint.top, 
			ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, 
			memDc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);*/
		BitBlt(dc, 0, 0, mWidth, mHeight, memDc, 0, 0, SRCCOPY);
		EndPaint(mWnd, &ps);
		return true;
	} else if (msg == WM_LBUTTONDOWN) {
		if (mEnableFocus) SetFocus(mWnd);
		// InvalidateRect(mWnd, NULL, TRUE);
		SetCapture(mWnd);
		return true;
	} else if (msg == WM_LBUTTONUP) {
		ReleaseCapture();
		// InvalidateRect(mWnd, NULL, TRUE);
		POINT pt = {(LONG)(short)LOWORD(lParam), (LONG)(short)HIWORD(lParam)};
		return true;
	} else if (msg == WM_MOUSEMOVE) {
		return true;
	} else if (msg == WM_MOUSELEAVE) {
		return true;
	}
	return XExtComponent::wndProc(msg, wParam, lParam, result);
}

void XSheetView::drawRowHeader(HDC dc) {
	static HBRUSH bg = CreateSolidBrush(0xF7ECE4);
	static HBRUSH leftBg = CreateSolidBrush(0xF0D8C2);
	static HPEN pen = CreatePen(PS_SOLID, 1, 0xCEB69E);
	
	char lineNum[8];
	if (mSheetModel == NULL) {
		return;
	}
	
	HGDIOBJ oldPen = SelectObject(dc, pen);
	int y = XGlobalInfo::DEF_COL_HEADER_HEIGHT;
	RECT r = {0, 0, XGlobalInfo::DEF_ROW_HEADER_WIDTH, mHeight};
	RECT itemRect = {0, y, XGlobalInfo::DEF_ROW_HEADER_WIDTH, y};
	FillRect(dc, &r, bg);
	SetTextColor(dc, 0x3E4127);
	SetBkMode(dc, TRANSPARENT);
	if (mRowHeaderFont != NULL) {
		SelectObject(dc, mRowHeaderFont);
	}
	for (int i = 0; y < mHeight; ++i) {
		XRowHeaderModel *rowHdr = mSheetModel->getRowHeader(i, false);
		int h = rowHdr == NULL ? XGlobalInfo::DEF_ROW_HEIGHT : rowHdr->getRowHeight();
		itemRect.bottom += h;
		sprintf(lineNum, "%d", mTranslateRow + i + 1);
		DrawText(dc, lineNum, strlen(lineNum), &itemRect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
		MoveToEx(dc, 0, itemRect.bottom, NULL);
		LineTo(dc, r.right, itemRect.bottom);
		itemRect.bottom += 1;
		itemRect.top = itemRect.bottom;
		y = itemRect.top;
	}
	if (mSelectAllConnerImage != NULL) {
		mSelectAllConnerImage->draw(dc, 0, 0,
			XGlobalInfo::DEF_ROW_HEADER_WIDTH, XGlobalInfo::DEF_COL_HEADER_HEIGHT);
	}
	SelectObject(dc, pen);
	MoveToEx(dc, XGlobalInfo::DEF_ROW_HEADER_WIDTH - 1, XGlobalInfo::DEF_COL_HEADER_HEIGHT - 1, NULL);
	LineTo(dc, XGlobalInfo::DEF_ROW_HEADER_WIDTH - 1, mHeight);
	// SelectObject(dc, oldPen);
}
void XSheetView::drawColHeader(HDC dc) {
	char colName[8];
	if (mSheetModel == NULL) {
		return;
	}
	if (mColHeaderImage != NULL) {
		mColHeaderImage->draw(dc, XGlobalInfo::DEF_ROW_HEADER_WIDTH, 0, 
			mWidth - XGlobalInfo::DEF_ROW_HEADER_WIDTH, XGlobalInfo::DEF_COL_HEADER_HEIGHT);
	}
	if (mColHeaderFont != NULL) {
		SelectObject(dc, mColHeaderFont);
	}
	int x = XGlobalInfo::DEF_ROW_HEADER_WIDTH;
	RECT r = {x, 0, x, XGlobalInfo::DEF_COL_HEADER_HEIGHT};
	for (int i = 0; x < mWidth; ++i) {
		XColHeaderModel *colHdr = mSheetModel->getColHeader(i, false);
		int w = colHdr == NULL ? XGlobalInfo::DEF_COL_WIDTH : colHdr->getColWidth();
		r.right += w;
		XPos xp(0, i);
		xp.toString(colName);
		char *p = colName + strlen(colName) - 1;
		while (*p >= '0' && *p <= '9') {*p = 0; --p;}
		DrawText(dc, colName, -1, &r, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		MoveToEx(dc, r.right, 0, NULL);
		LineTo(dc, r.right, r.bottom);
		r.right += 1;
		r.left = r.right;
		x = r.right;
	}
}
