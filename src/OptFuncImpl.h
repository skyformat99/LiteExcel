#pragma once
#include <windows.h>
#include "XExt.h"
#include "Manager.h"

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
class FontNameListener : public EventListener {
public:
	FontNameListener(XExtComboBox *c);
	virtual void onEvent(EventType et, void *evtSrc, void *newVal, void *oldVal);
protected:
	XExtComboBox *mFontNameComboBox;
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
class FontSizeListener : public EventListener {
public:
	FontSizeListener(XExtComboBox *c);
	virtual void onEvent(EventType et, void *evtSrc, void *newVal, void *oldVal);
protected:
	XExtComboBox *mFontSizeComboBox;
};