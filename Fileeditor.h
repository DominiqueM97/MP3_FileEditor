#include "User_Types.h"
#include <windows.h>
int FileeditorMenu(HINSTANCE hInstance, HWND HandleWinMain);
int FileEditorViewer(HINSTANCE hInstance, HWND LastWindowHandle, char vFilePath[vFilePathLenght], int vViewtype);
int SaveFile(HINSTANCE hInstance, HWND hwnd, char vFilePath[vFilePathLenght], int vMaxColNum, INT64 vFileLenght, int vViewtype, bool vNewFile);