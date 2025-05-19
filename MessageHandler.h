#include <windows.h>

LRESULT CALLBACK MessageHandler(HWND hMessageHandler, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListViewSubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK InPlaceEditControl_SubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);