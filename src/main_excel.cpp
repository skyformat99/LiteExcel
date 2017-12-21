#include "UIFactory.h"
#include "XComponent.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "XmlParser.h"
#include "XExt.h"
#include "OptButtons.h"

XWindow *win;
XDialog *dlg;
XExtPopup *popup;
XExtMenuItemList *mlist;

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
	XArrowColorButton::init();
	XAnchorButton::init();

	win = (XWindow *) UIFactory::fastBuild("file://skin/excel.xml", "main-page", NULL);
	

	win->show(nCmdShow);
	win->messageLoop();
	UIFactory::destory(win->getNode());
	return 0;
}