#pragma once
#include <windows.h>
#include "XExt.h"

class FontsListModel : public XListModel {
public:
	FontsListModel(XExtComboBox *list);
	virtual int getItemCount();
	virtual int getItemHeight(int item);
	virtual ItemData *getItemData(int item);
	virtual bool isMouseTrack();
protected:
	XExtComboBox *mExtList;
	LOGFONT *mFonts;
	int mFontsNum;
};

class FontsSizeListModel : public XListModel {
public:
	FontsSizeListModel(XExtComboBox *list);
	virtual int getItemCount();
	virtual int getItemHeight(int item);
	virtual ItemData *getItemData(int item);
	virtual bool isMouseTrack();
protected:
	int *mFontSizeArr;
	int mFontSizeNum;
	XExtComboBox *mExtList;
};