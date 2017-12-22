#include "UIFactory.h"
#include "XComponent.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "XmlParser.h"
#include "XExt.h"
#include "OptButtons.h"
#include "OptFuncImpl.h"
#include "Manager.h"
#include "SheetView.h"

static XWindow *win;
XWindow *GetMainWin() {
	return win;
}

void InitOptTool() {
	XExtComboBox *fontName = (XExtComboBox*)(win->findById("opt_font_name"));
	fontName->getExtList()->setModel(new FontsListModel(fontName));
	EventManager::addEventListener(EVENT_TYPE_CELL_SELECT_RANGE_CHANGE, new FontNameListener(fontName));

	XExtComboBox *fontSize = (XExtComboBox*)(win->findById("opt_font_size"));
	fontSize->getExtList()->setModel(new FontsSizeListModel(fontName));
	EventManager::addEventListener(EVENT_TYPE_CELL_SELECT_RANGE_CHANGE, new FontSizeListener(fontSize));
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	// ---- debug -----
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	char path[256];
	/*GetModuleFileName(NULL, path, 256);
	char *p = strrchr(path, '\\') + 1;
	*p = 0;*/
	strcpy(path, "E:\\ExcelProj\\ExcelProj");
	SetCurrentDirectory(path);

	/*HRSRC mm = FindResource(NULL, "MY_ANY_MM", "ANY");
	HGLOBAL hmm = LoadResource(NULL, mm);
	char *mmDat = (char *)LockResource(hmm);
	int mmLen = SizeofResource(NULL, mm);
	FreeResource(hmm);*/

	UIFactory::init();
	ArrowColorButton::init();
	AnchorButton::init();
	XSheetView::init();

	win = (XWindow *) UIFactory::fastBuild("file://skin/excel.xml", "main-page", NULL);
	InitOptTool();

	win->show(nCmdShow);
	win->messageLoop();
	UIFactory::destory(win->getNode());
	return 0;
}