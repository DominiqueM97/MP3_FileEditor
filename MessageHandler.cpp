// Include Basic Funktions
#include <windows.h>
#include <string>
#include "commctrl.h"
#include <windowsx.h>

// Include additional Modules / Functions
#include "User_Types.h"
#include "MessageHandler.h"
#include "Settings.h"

// Activate Namespace
using namespace std;

// Global Variables
extern string vLanguagepack[];
extern int vActiveViewtypeCharacterCount;
extern int vActiveViewtype;

// Later defined Functions
// Idea from Stackoverflow - Edited to fit to this Programm
LRESULT CALLBACK ListViewSubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK InPlaceEditControl_SubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
bool Validade_User_Input(char User_Input[7]);

// Message Handler
// Handels the actions of Buttons
LRESULT CALLBACK MessageHandler(HWND hMessageHandler, UINT uMsg, WPARAM wParam, LPARAM lParam) {


    // Switch Message
    switch (uMsg) {


        // Standard
    case WM_CLOSE:

        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:

        //PostQuitMessage(0);
        return 0;

        // Buttons
    case WM_COMMAND:

        switch (LOWORD(wParam)) {

        case ID_BUTTON_CLOSE:

            // Close Programm / Close Window
            PostQuitMessage(0);
            break;

        
        // User Buttons
        case ID_BUTTON_START_EDITOR:
        case ID_BUTTON_START_SETTINGS:
        case ID_BUTTON_SETTINGS_SET_LANGUAGE:
        case ID_BUTTON_SETTINGS_SET_DEFAULTTYPE:
        case ID_BUTTON_SETTINGS_SET_TEXTCOLOR:
        case ID_BUTTON_SETTINGS_SAVE:
        case ID_BUTTON_SETTINGS_CLOSE:
        case ID_BUTTON_EDITOR_CLOSE:
        case ID_BUTTON_EDITOR_OPEN_NEW_FILE:
        case ID_BUTTON_EDITOR_OPEN_LAST_FILE:
        case ID_BUTTON_EDITOR_SAVE:
        case ID_BUTTON_EDITOR_SAVE_NEW:


            // Return the Message Details to the running GetMessage Call
            // to decide next steps in Prorgamm
            PostMessage(nullptr, uMsg, wParam, lParam);
            return 0;


        default:
            break;

        };
        break;


        // Case WM_NOTIFY is for enebaling the GridViewEditor
        // Idea from Stackoverflow - Edited to fit to this Programm
    case WM_NOTIFY: {

        if (((LPNMHDR)lParam)->code == NM_DBLCLK) {

            switch (((LPNMHDR)lParam)->idFrom) {

            case ID_LISTVIEW: {

                LPNMITEMACTIVATE lpnmia = (LPNMITEMACTIVATE)lParam;

                // SHIFT/ALT/CTRL/their combination, must not be pressed
                if ((lpnmia->uKeyFlags || 0) == 0) {

                    // store item/subitem rectangle
                    RECT rc = { 0 };
                    // helper values, needed for handling partially visible items
                    int topIndex = ListView_GetTopIndex(lpnmia->hdr.hwndFrom);
                    int visibleCount = ListView_GetCountPerPage(lpnmia->hdr.hwndFrom);
                    // if item is vertically partially visible, make it fully visible
                    if ((topIndex + visibleCount) == lpnmia->iItem)
                    {
                        // get the rectangle of the above item -> lpnmia->iItem - 1
                        ListView_GetSubItemRect(lpnmia->hdr.hwndFrom, lpnmia->iItem - 1, lpnmia->iSubItem, LVIR_LABEL, &rc);
                        // ensure clicked item is visible
                        ListView_EnsureVisible(lpnmia->hdr.hwndFrom, lpnmia->iItem, FALSE);
                    }
                    else // item is fully visible, just get its ectangle
                        ListView_GetSubItemRect(lpnmia->hdr.hwndFrom, lpnmia->iItem, lpnmia->iSubItem, LVIR_LABEL, &rc);

                    RECT rcClient = { 0 };  // listview client rectangle, needed if item partially visible
                    GetClientRect(lpnmia->hdr.hwndFrom, &rcClient);
                    // item is horizontally partially visible -> from the right side
                    if (rcClient.right < rc.right)
                    {
                        // show the whole item
                        ListView_Scroll(lpnmia->hdr.hwndFrom, rc.right - rcClient.right, 0);
                        // adjust rectangle so edit control is properly displayed
                        rc.left -= rc.right - rcClient.right;
                        rc.right = rcClient.right;
                    }
                    // item is horizontally partially visible -> from the left side
                    if (rcClient.left > rc.left)
                    {
                        // show the whole item
                        ListView_Scroll(lpnmia->hdr.hwndFrom, rc.left - rcClient.left, 0);
                        // adjust rectangle so edit control is properly displayed
                        rc.right += rcClient.left - rc.left;
                        rc.left = rcClient.left;
                    }
                    // it is time to position edit control, we start by getting its window handle
                    HWND HandleGridViewEditor = GetDlgItem(hMessageHandler, ID_LISTVIEW_EDITOR);
                    // get item text and set it as edit control's text
                    string text;
                    ListView_GetItemText(lpnmia->hdr.hwndFrom, lpnmia->iItem, lpnmia->iSubItem, (LPSTR)text.c_str(), vActiveViewtypeCharacterCount + 1);
                    Edit_SetText(HandleGridViewEditor, text.c_str());
                    // select entire text
                    Edit_SetSel(HandleGridViewEditor, 0, -1);
                    // map listview client rectangle to parent rectangle
                    // so edit control can be properly placed above the item
                    MapWindowPoints(lpnmia->hdr.hwndFrom, hMessageHandler, (LPPOINT)&rc, (sizeof(RECT) / sizeof(POINT)));
                    // move the edit control
                    SetWindowPos(HandleGridViewEditor, HWND_TOP, rc.left, rc.top, rc.right - rc.left,
                        rc.bottom - rc.top, SWP_SHOWWINDOW);
                    // set focus to our edit control
                    HWND previousWnd = SetFocus(HandleGridViewEditor);
                };
                break;
            };
            default:

                break;

            };
        };
    };
    default:

        break;

    };


return DefWindowProc(hMessageHandler, uMsg, wParam, lParam);


};




// For GridViewEditor:
// Idee from Stackoverflow - Edited to fit to this Programm
LRESULT CALLBACK ListViewSubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {

    switch (message)
    {
    case WM_VSCROLL:
    case WM_HSCROLL:
        // if edit control has the focus take it away and give to listview
        if (GetFocus() == GetDlgItem(GetParent(hwnd), ID_LISTVIEW_EDITOR))
            SetFocus(hwnd);
        break;
    case WM_NCDESTROY:
        ::RemoveWindowSubclass(hwnd, ListViewSubclassProc, uIdSubclass);
        return DefSubclassProc(hwnd, message, wParam, lParam);
    }
    return ::DefSubclassProc(hwnd, message, wParam, lParam);
};

// For GridViewEditor:
// Idee from Stackoverflow - Edited to fit to this Programm
LRESULT CALLBACK InPlaceEditControl_SubclassProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {

    switch (message)
    {
    case WM_GETDLGCODE:
        return (DLGC_WANTALLKEYS | DefSubclassProc(hwnd, message, wParam, lParam));
    case WM_KILLFOCUS:
        ShowWindow(hwnd, SW_HIDE);
        return DefSubclassProc(hwnd, message, wParam, lParam);
    case WM_CHAR:
        //Process this message to avoid message beeps.
        switch (wParam)
        {
        case VK_RETURN:
            return 0L;
        case VK_ESCAPE:
            return 0L;
        default:
            return ::DefSubclassProc(hwnd, message, wParam, lParam);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RETURN:
        {
            // get listview handle
            HWND hwndLV = GetDlgItem(GetParent(hwnd), ID_LISTVIEW);
            // get edit control's client rectangle
            RECT rc = { 0 };
            GetClientRect(hwnd, &rc);
            // since edit control lies inside item rectangle
            // we can test any coordinate inside edit control's
            // client rectangle
            MapWindowPoints(hwnd, hwndLV, (LPPOINT)&rc, (sizeof(RECT) / sizeof(POINT)));
            // get item and subitem indexes
            LVHITTESTINFO lvhti = { 0 };
            lvhti.pt.x = rc.left;
            lvhti.pt.y = rc.top;
            ListView_SubItemHitTest(hwndLV, &lvhti);
            // get edit control's text
            wchar_t txt[7] = L"";
            Edit_GetText(hwnd, (LPSTR)txt, vActiveViewtypeCharacterCount);
            // Check User Input
            if (Validade_User_Input((char*)txt)) {

                // User Input is Valid
                // edit cell text
                ListView_SetItemText(hwndLV, lvhti.iItem, lvhti.iSubItem, (LPSTR)txt);
                // restore focus to listview
                // this triggers EN_KILLFOCUS
                // which will hide edit control
                SetFocus(hwndLV);

                PostMessage(nullptr, message, ID_EDITOR_CHANGE, lParam);
            }
            else {
            
                // User Input is Invalid
                PostMessage(nullptr, message, 0, lParam);
            
            };
        };


        return 0L;
        case VK_ESCAPE:
            SetFocus(GetDlgItem(GetParent(hwnd), ID_LISTVIEW));
            return 0L;
        default:
            return ::DefSubclassProc(hwnd, message, wParam, lParam);
        }
        break;
    case WM_NCDESTROY:
        ::RemoveWindowSubclass(hwnd, InPlaceEditControl_SubclassProc, uIdSubclass);
        return DefSubclassProc(hwnd, message, wParam, lParam);

    }
    return ::DefSubclassProc(hwnd, message, wParam, lParam);
};


// Validade User Input depending on the active Viewtype
bool Validade_User_Input(char User_Input[7]) {

    // Check the Users Input bevor writing it back to the Grid View
    // depending on the selected ViewType
    // HEX has to be HEX
    // BIN has to be BIN
    // CHAR has to be CHAR
    // String is allready limitet to the ViewType Specific Lenght

    // Init needed Variables
    int vTemp = 0;
    bool vRetunrValue = true;
    string hexChars = "0123456789ABCDEF";
    string vMessageText;

    switch (vActiveViewtype) {

    // HEX
    case 1:

        // Check if User Input only contains '@0123456789ABCDEF'
        // Check each of the 2 Characters
        for (vTemp = 0; vTemp <= 1; ++vTemp) {

            if (hexChars.find(User_Input[vTemp]) == -1) {

                // Generate Error-Text including Character and Position
                vMessageText = vLanguagepack[31] + '\n' + vLanguagepack[32] + User_Input[vTemp] + " " + vLanguagepack[33] + to_string(vTemp + 1);
                MessageBox(NULL, vMessageText.c_str(), vLanguagepack[17].c_str(), MB_ICONERROR | MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
                vRetunrValue = false;

            };

        };

        return vRetunrValue;
        break;

    // BIN
    case 2:

        // Check if User Input only contains '1' or '0'
        // Check each of the 8 Characters
        for (vTemp = 0; vTemp <= 7; ++vTemp) {

            if (User_Input[vTemp] != '1' and User_Input[vTemp] != '0') {
                
                // Generate Error-Text including Character and Position
                vMessageText = vLanguagepack[31] + '\n' + vLanguagepack[32] + User_Input[vTemp] + " " + vLanguagepack[33] + to_string(vTemp + 1);
                MessageBox(NULL, vMessageText.c_str(), vLanguagepack[17].c_str(), MB_ICONERROR | MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
                vRetunrValue = false; 
            
            };

        };

        return vRetunrValue;
        break;


    // CHAR
    case 3:

        // Allways true
        return vRetunrValue;
        break;


    default:

        break;

    };

    return false;

};
