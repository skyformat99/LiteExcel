#include "OptFuncImpl.h"
#include <stdio.h>
#include "XAttrs.h"

FontsListModel::FontsListModel(XExtComboBox *list) {
	mExtList = list;
	mFontsNum = 0;
	HWND wnd = list->getWnd();
	HDC dc = GetDC(wnd);
	mFonts = XFontManager::getLocalFonts(&mFontsNum);
	ReleaseDC(wnd, dc);
}
int FontsListModel::getItemCount() {
	return mFontsNum;
}
int FontsListModel::getItemHeight(int item) {
	return 25;
}
XListModel::ItemData* FontsListModel::getItemData(int item) {
	static ItemData data;
	data.mSelectable = true;
	data.mText = mFonts[item].lfFaceName;
	return &data;
}
bool FontsListModel::isMouseTrack() {
	return true;
}

FontNameListener::FontNameListener(XExtComboBox *c) {
	mFontNameComboBox = c;
}
void FontNameListener::onEvent(EventType et, void *evtSrc, void *newVal, void *oldVal) {
	if (et == EVENT_TYPE_CELL_SELECT_RANGE_CHANGE) {
		// TODO: change combobox value
	}
}

FontsSizeListModel::FontsSizeListModel(XExtComboBox *list) {
	mExtList = list;
	static int items[] = {9, 10, 11, 12, 14, 16, 18, 20, 24, 26, 28, 30, 34, 38, 40, 46, 56, 66};
	mFontSizeArr = items;
	mFontSizeNum = sizeof(items)/sizeof(int);
}
int FontsSizeListModel::getItemCount() {
	return mFontSizeNum;
}
int FontsSizeListModel::getItemHeight(int item) {
	return 25;
}
XListModel::ItemData* FontsSizeListModel::getItemData(int item) {
	static char str[20];
	static ItemData data;
	data.mSelectable = true;
	sprintf(str, "%d", mFontSizeArr[item]);
	data.mText = str;
	return &data;
}
bool FontsSizeListModel::isMouseTrack() {
	return true;
}

FontSizeListener::FontSizeListener(XExtComboBox *c) {
	mFontSizeComboBox = c;
}
void FontSizeListener::onEvent(EventType et, void *evtSrc, void *newVal, void *oldVal) {
	if (et == EVENT_TYPE_CELL_SELECT_RANGE_CHANGE) {
		// TODO: change combobox value
	}
}
