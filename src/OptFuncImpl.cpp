#include "OptFuncImpl.h"
#include <stdio.h>

static LOGFONT s_fonts[300];

static BOOL CALLBACK EnumFontsCallBack(CONST LOGFONT* lplf, CONST TEXTMETRIC *lptm,DWORD dwType,LPARAM lparam) {
	static char *filter[] = {"宋体", "新宋体", "仿宋", "楷体", "方正舒体", "隶书", "华文宋体", "华文行楷",
					"Consolas", "Courier", "Courier New", "Roman", "Terminal", "Times New Roman", "MS Serif"};
	int idx = *(int*)lparam;
	if (strchr(lplf->lfFaceName, '@') != NULL) {
		return true;
	}
	for (int i = 0; i < sizeof(filter)/sizeof(char*); ++i) {
		if (strcmp(filter[i], lplf->lfFaceName) == 0) {
			s_fonts[idx] = *lplf;
			*(int*)lparam = idx + 1;
			// printf("[%3d] [%s] \n", idx, lplf->lfFaceName);
			break;
		}
	}
	return idx < 299;
}

static LOGFONT* GetLocalFonts(HDC dc, int *num) {
	int idx = 0;
	EnumFonts(dc, NULL, EnumFontsCallBack, (LPARAM)&idx);
	if (num != NULL) *num = idx;
	return s_fonts;
}

FontsListModel::FontsListModel(XExtComboBox *list) {
	mExtList = list;
	mFontsNum = 0;
	HWND wnd = list->getWnd();
	HDC dc = GetDC(wnd);
	mFonts = GetLocalFonts(dc, &mFontsNum);
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
	data.mText = s_fonts[item].lfFaceName;
	return &data;
}
bool FontsListModel::isMouseTrack() {
	return true;
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
