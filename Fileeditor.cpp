// Include Basic Funktions
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <format>
#include <windowsx.h>
#include <stdio.h>
#include <vector>

// Include additional Modules / Functions
#include "commctrl.h"
#include "User_Types.h"
#include "MessageHandler.h"
#include "Settings.h"

// link with Common Controls library
#pragma comment( lib, "comctl32.lib") 

// Activate Namespace
using namespace std;

// Global Variables
extern Settings vSettings;
extern string vLanguagepack[];
extern bool vUpdateLanguage[];
int vActiveViewtypeCharacterCount = 1;
int vActiveViewtype = 1;

// Later defined Functions
int FileEditorViewer(HINSTANCE hInstance, HWND LastWindowHandle, char vFilePath[vFilePathLenght], int vViewtype);
string ByteToHexString(byte inbyte);
int SaveFile(HINSTANCE hInstance, HWND HandleGridView, HWND HandleWinEditorViewer, char vFilePath[vFilePathLenght], INT64 vFileLenght, int vViewtype, vector <byte> vFileData, vector <MP3Framelenghts> vFrameLenghts, INT64 vFrameCount);
MP3Framinfo MP3_Frame_Decoder(byte Headerbytes[4]);


// Fileeditor Menu
int FileeditorMenu(HINSTANCE hInstance, HWND LastWindowHandle) {

	// Create Basic Variabls
	bool vFileOpend = FALSE;

	// Dont show the last Window and create new one
	ShowWindow(LastWindowHandle, SW_HIDE);

	// Create Variables
	// Set new Variables
	HWND HandleWinEditor;
	HWND HandleButtonCancel;
	HWND HandleButtonOpenNewFile;
	HWND HandleSelectionLastFiles[11];
	HWND HandleSelectionViewTyp[4];
	HWND HandleButtonOpenLastFile;
	HWND HandleButtonSettings;

	MSG MessageEditor;
	WNDCLASS WinEditor;

	WinEditor = {};
	WinEditor.style = CS_HREDRAW | CS_VREDRAW;
	WinEditor.lpfnWndProc = MessageHandler;
	WinEditor.hInstance = hInstance;
	WinEditor.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WinEditor.lpszClassName = "WinEditor";

	// Inital Buttons when opening the Fileeditor
	// Register the WinEditor Parameters
	// Create the Window Handler for Editor Window
	RegisterClass(&WinEditor);

	HandleWinEditor = CreateWindow("WinEditor", vLanguagepack[4].c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, NULL, NULL, hInstance, 0);
	// Open New File -> Open Dialog Window
	HandleButtonOpenNewFile = CreateWindow("button", vLanguagepack[13].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 30, 300, 50, HandleWinEditor, (HMENU)ID_BUTTON_EDITOR_OPEN_NEW_FILE, hInstance, 0);
	// Last File Selection
	// GroupBox and Open Button
	HandleSelectionLastFiles[0] = CreateWindow("Button", vLanguagepack[15].c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 110, 640, 385, HandleWinEditor, (HMENU)0, hInstance, NULL);
	HandleButtonOpenLastFile = CreateWindow("button", vLanguagepack[14].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 420, 600, 50, HandleWinEditor, (HMENU)ID_BUTTON_EDITOR_OPEN_LAST_FILE, hInstance, 0);
	// Radio Buttons -> Last Files Selection
	// Fileselction are a Child of Groupbox, Groupbox is a Child of HandleWinEditor
	HandleSelectionLastFiles[1] = CreateWindow("Button", vSettings.LastFiles[0].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 30, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[2] = CreateWindow("Button", vSettings.LastFiles[1].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 55, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[3] = CreateWindow("Button", vSettings.LastFiles[2].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 80, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[4] = CreateWindow("Button", vSettings.LastFiles[3].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 105, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[5] = CreateWindow("Button", vSettings.LastFiles[4].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 130, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[6] = CreateWindow("Button", vSettings.LastFiles[5].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 155, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[7] = CreateWindow("Button", vSettings.LastFiles[6].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 180, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[8] = CreateWindow("Button", vSettings.LastFiles[7].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 205, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[9] = CreateWindow("Button", vSettings.LastFiles[8].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 230, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLastFiles[10] = CreateWindow("Button", vSettings.LastFiles[9].c_str(), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 255, 600, 30, HandleSelectionLastFiles[0], (HMENU)0, hInstance, NULL);	
	// View-Type Selection
	// GroupBox
	HandleSelectionViewTyp[0] = CreateWindow("Button", vLanguagepack[19].c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 680, 110, 190, 140, HandleWinEditor, (HMENU)0, hInstance, NULL);
	// Radio Buttons -> Typ Selection
	// Types are a Child of Groupbox, Groupbox is a Child of HandleWinEditor
	HandleSelectionViewTyp[1] = CreateWindow("Button", "1 - HEX", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 30, 150, 30, HandleSelectionViewTyp[0], (HMENU)0, hInstance, NULL);
	HandleSelectionViewTyp[2] = CreateWindow("Button", "2 - BIN", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 55, 150, 30, HandleSelectionViewTyp[0], (HMENU)0, hInstance, NULL);
	HandleSelectionViewTyp[3] = CreateWindow("Button", "3 - CHAR", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 80, 150, 30, HandleSelectionViewTyp[0], (HMENU)0, hInstance, NULL);
	// Preselect the current View Type
	SendMessage(HandleSelectionViewTyp[vSettings.DefaultTyp], BM_SETCHECK, BST_CHECKED, 0);
	// Switch to Settings
	HandleButtonSettings = CreateWindow("button", vLanguagepack[1].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 525, 300, 50, HandleWinEditor, (HMENU)ID_BUTTON_START_SETTINGS, hInstance, 0);
	// Close Editor Button
	HandleButtonCancel = CreateWindow("button", vLanguagepack[6].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 700, 300, 50, HandleWinEditor, (HMENU)ID_BUTTON_EDITOR_CLOSE, hInstance, 0);

	//Show the created Window;
	ShowWindow(HandleWinEditor, SW_SHOW);
	SetForegroundWindow(HandleWinEditor);
	SetFocus(HandleWinEditor);
	UpdateWindow(HandleWinEditor);


	// Create Variables for Filehandle - Dialog Window to Open / Save File
	char vFilePath[vFilePathLenght];
	vFilePath[0] = 0;
	OPENFILENAME vOpenFilehandle = {};
	vOpenFilehandle.lStructSize = sizeof(OPENFILENAME);
	vOpenFilehandle.hInstance = 0;
	vOpenFilehandle.lpstrFile = vFilePath;
	vOpenFilehandle.nMaxFile = vFilePathLenght;
	vOpenFilehandle.lpstrFilter = ".mp3\0*.mp3";
	vOpenFilehandle.lpstrTitle = vLanguagepack[13].c_str();
	vOpenFilehandle.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
	
	// User Interaction
	bool running = true;
	int vTempPointer = 0;
	int vLastFilePathNumber = 0;
	int vViewtype = 0;

	while (running) {

		while (PeekMessage(&MessageEditor, 0, 0, 0, PM_REMOVE)) {


			// Update Element Text (to get new text in case of new Language)
			if (vUpdateLanguage[3]) {

				SetWindowText(HandleWinEditor, vLanguagepack[4].c_str());
				SetWindowText(HandleButtonOpenNewFile, vLanguagepack[13].c_str());
				SetWindowText(HandleButtonOpenLastFile, vLanguagepack[14].c_str());
				SetWindowText(HandleButtonSettings, vLanguagepack[1].c_str());
				SetWindowText(HandleSelectionLastFiles[0], vLanguagepack[15].c_str());
				SetWindowText(HandleSelectionViewTyp[0], vLanguagepack[19].c_str());
				SetWindowText(HandleButtonCancel, vLanguagepack[6].c_str());

				UpdateWindow(HandleWinEditor);
				// deactivate Update Flag
				vUpdateLanguage[3] = false;

			};

			if (MessageEditor.message == WM_QUIT) {

				// Close Editor and Return to past Window
				running = false;

			}
			else if (MessageEditor.message == WM_COMMAND) {


				// Check witch Viewtype is selected
				// Store Result to handover it to Fileeditorviewer
				// Set the Global Active Character Count for the selected Viewtype
				if (SendMessage(HandleSelectionViewTyp[1], BM_GETCHECK, 0, 0) == BST_CHECKED) {

					// HEX
					vViewtype = 1;
					vActiveViewtype = 1;
					vActiveViewtypeCharacterCount = 2;

				}
				else if (SendMessage(HandleSelectionViewTyp[2], BM_GETCHECK, 0, 0) == BST_CHECKED) {

					// BIN
					vViewtype = 2;
					vActiveViewtype = 2;
					vActiveViewtypeCharacterCount = 8;

				}
				else if (SendMessage(HandleSelectionViewTyp[3], BM_GETCHECK, 0, 0) == BST_CHECKED) {

					//CHAR
					vViewtype = 3;
					vActiveViewtype = 3;
					vActiveViewtypeCharacterCount = 1;

				};


				switch (LOWORD(MessageEditor.wParam)) {


				case ID_BUTTON_EDITOR_OPEN_NEW_FILE:

					// Dialog-Window to select a new File
					if (GetOpenFileName(&vOpenFilehandle) != 0) {

						// Safe the Path of Current File to the Settings.conf
						SaveLastFiles(vFilePath);
						// Start Editor and Wait for finish Editing
						if (FileEditorViewer(hInstance, HandleWinEditor, vFilePath, vViewtype) == 0) {
						
							running = false;
						
						};

					};

					break;

				case ID_BUTTON_EDITOR_OPEN_LAST_FILE:

					// Open File from the 10 Last opend Files
					// Chek witch File is selectet
					// Switch through the 10 Selectionwindows an get the one selected
					vTempPointer = 1;
					vLastFilePathNumber = 0;

					while (vTempPointer <= 10) {


						if (SendMessage(HandleSelectionLastFiles[vTempPointer], BM_GETCHECK, 0, 0) == BST_CHECKED) {

							vLastFilePathNumber = vTempPointer;
							break;

						};

						++vTempPointer;
					};

					// Check if one Path is selected
					if (vLastFilePathNumber != 0) {

						// Check if Path is Valid
						if (vSettings.LastFiles[vLastFilePathNumber - 1].length() > 0) {

							// Convert the Stringpath to char
							strcpy_s(vFilePath, vSettings.LastFiles[vLastFilePathNumber - 1].data());
							// Start Editor and Wait for finish Editing
							if (FileEditorViewer(hInstance, HandleWinEditor, vFilePath, vViewtype) == 0) {

								running = false;

							};

						};

					};

					break;


				case ID_BUTTON_START_SETTINGS:

					// Open Settings
					// Hand Over Current Window Informations
					SettingsMenu(hInstance, HandleWinEditor);
					break;

				case ID_BUTTON_EDITOR_CLOSE:

					// Close Editor and Return to past Window
					running = false;
					break;

				default:
					break;

				};
				break;

			};


			TranslateMessage(&MessageEditor);
			DispatchMessage(&MessageEditor);

		};

	};

	// Close the Settings Window
	ShowWindow(HandleWinEditor, SW_HIDE);

	// Restore the last Window
	ShowWindow(LastWindowHandle, SW_SHOW);
	UpdateWindow(LastWindowHandle);


	return 0;
};


// File Editor-Window
int FileEditorViewer(HINSTANCE hInstance, HWND LastWindowHandle, char vFilePath[vFilePathLenght], int vViewtype) {

	// Create Basic Variabls
	bool vFileNotSaved = FALSE;
	int vChangeCount = 0;

	// Dont show the last Window and create new one
	// In this case destroy, going back is to Main Menu
	DestroyWindow(LastWindowHandle);


	// Create Variables
	// Set new Variables
	HWND HandleWinEditorViewer;
	HWND HandleButtonCancel;
	HWND HandleButtonSettings;
	HWND HandleButtonSave;
	HWND HandleButtonSaveNew;
	HWND HandleGridView;
	HWND HandleGridViewEditor;
	HWND HandleFilePathText;
	HWND HandleChangeCount;
	HWND HandleLoadingbar;

	MSG MessageEditorViewer;
	WNDCLASS WinEditorViewer;

	WinEditorViewer = {};
	WinEditorViewer.style = CS_HREDRAW | CS_VREDRAW;
	WinEditorViewer.lpfnWndProc = MessageHandler;
	WinEditorViewer.hInstance = hInstance;
	WinEditorViewer.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WinEditorViewer.lpszClassName = "WinEditorViewer";

	// Inital Buttons when opening the Fileeditor
	// Register the WinEditorViewer Parameters
	// Create the Window Handler for Editor Window
	RegisterClass(&WinEditorViewer);
	HandleWinEditorViewer = CreateWindow("WinEditorViewer", vLanguagepack[4].c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1400, 800, NULL, NULL, hInstance, 0);

	// Loading Progress
	string vLoadprogress = vLanguagepack[23] + "0%";
	HandleLoadingbar = CreateWindow("STATIC", vLoadprogress.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT, 600, 385, 200, 30, HandleWinEditorViewer, NULL, hInstance, NULL);

	// Show Window and LoadingProgress
	ShowWindow(HandleWinEditorViewer, SW_SHOW);
	SetForegroundWindow(HandleWinEditorViewer);
	SetFocus(HandleWinEditorViewer);
	UpdateWindow(HandleWinEditorViewer);


	// Path of opend File
	HandleFilePathText = CreateWindow("STATIC", vFilePath, WS_VISIBLE | WS_CHILD | SS_LEFT, 30, 30, 600, 30, HandleWinEditorViewer, NULL, hInstance, NULL);
	string vChangeCountText = vLanguagepack[30] + to_string(vChangeCount);
	HandleChangeCount = CreateWindow("STATIC", vChangeCountText.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT, 1070, 30, 300, 30, HandleWinEditorViewer, NULL, hInstance, NULL);

	// Grid with Filedata
	HandleGridView = CreateWindowEx(0, WC_LISTVIEW, "Überschrift", WS_CHILD | WS_BORDER | WS_CLIPSIBLINGS | LVS_REPORT, 30, 90, 1340, 500, HandleWinEditorViewer, (HMENU)ID_LISTVIEW, hInstance, 0);
	ListView_SetExtendedListViewStyle(HandleGridView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	HandleGridViewEditor = CreateWindowEx(0, WC_EDIT, "", ES_AUTOHSCROLL | WS_CHILD | WS_BORDER, 200, 265, 100, 25, HandleWinEditorViewer, (HMENU)ID_LISTVIEW_EDITOR, hInstance, 0);
	// Set the edit Window to the same font
	HFONT HandleGridViewFont = (HFONT)SendMessage(HandleGridView, WM_GETFONT, 0, 0);
	if (HandleGridViewFont) { SendMessage(HandleGridViewEditor, WM_SETFONT, (WPARAM)HandleGridViewFont, (LPARAM)TRUE); };

	// Switch to Settings
	HandleButtonSettings = CreateWindow("button", vLanguagepack[1].c_str(), WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON, 30, 680, 300, 50, HandleWinEditorViewer, (HMENU)ID_BUTTON_START_SETTINGS, hInstance, 0);
	// Save Button
	HandleButtonSave = CreateWindow("button", vLanguagepack[24].c_str(), WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON, 360, 680, 300, 50, HandleWinEditorViewer, (HMENU)ID_BUTTON_EDITOR_SAVE, hInstance, 0);
	// Save as New File Button
	HandleButtonSaveNew = CreateWindow("button", vLanguagepack[25].c_str(), WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON, 690, 680, 300, 50, HandleWinEditorViewer, (HMENU)ID_BUTTON_EDITOR_SAVE_NEW, hInstance, 0);
	// Close Settings Button
	HandleButtonCancel = CreateWindow("button", vLanguagepack[6].c_str(), WS_TABSTOP | WS_CHILD | BS_DEFPUSHBUTTON, 1020, 680, 300, 50, HandleWinEditorViewer, (HMENU)ID_BUTTON_EDITOR_CLOSE, hInstance, 0);



	// Open the selected File in Binary-Mode
	ifstream vOpenfileBinary;
	vOpenfileBinary.open(vFilePath, ios::binary);


	// Open the selected File as Binary
	// Chek if File was opend !
	if (!vOpenfileBinary.is_open()) {

		// File not found
		MessageBox(NULL, vLanguagepack[22].c_str(), "Error", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
		return 0;

	};


	// Get the Filelength to later Upadte the Loadprogress
	// Store Filelenght and create Variables to show the load progress
	INT64 vFileLenght = 0;
	INT64 vCurrentProgress = 0;
	int vProgressProcent = 0;
	int vProgress = 0;
	vOpenfileBinary.seekg(0, ios::end);		// Go to End of File
	vFileLenght = vOpenfileBinary.tellg();	// Load the Filelength
	vOpenfileBinary.seekg(0, ios::beg);		// Go Back to Begin of File


	// Create Variables to handle the File Data and write it to the Tabel (Gridview)
	// INT64 to handle large Files
	// Vetcor to store the hole file
	LVITEM LvItem;
	memset(&LvItem, 0, sizeof(LvItem));
	LVCOLUMN LvCol;
	memset(&LvCol, 0, sizeof(LvCol));
	INT64 vRowNum = 0;
	INT64 vColNum = 0;
	INT64 vFrameLength = 0;
	INT64 vFrameByteswriten = 0;
	INT64 vFrameCount = 0;
	string vTempText;
	byte vActualByte;
	string vConvertedText;
	vector <byte> vFileData;
	vector <MP3Framelenghts> vFrameLenghts;
	MP3Framelenghts vTempFrameLenght;
	MP3Framinfo vFrameinfo{};
	INT64 vPointer = 0;



	// First load the hole File Data to the Vector-Buffer
	for (vPointer = 0; !vOpenfileBinary.eof(); ++vPointer) {

		vFileData.push_back(vOpenfileBinary.get());

		// Update Current Progress
		++vCurrentProgress;
		// After each Byte
		// Calculate Procentege of Progress
		// Update HandleLoadingbar Text
		if (vProgressProcent < floor(vCurrentProgress * 100 / vFileLenght)) {

			vProgressProcent = floor(vCurrentProgress * 100 / vFileLenght);
			vLoadprogress = vLanguagepack[29] + to_string(vProgressProcent) + "%";
			SetWindowText(HandleLoadingbar, vLoadprogress.c_str());
			UpdateWindow(HandleWinEditorViewer);

		};
	};

	// Close the loaded File
	vOpenfileBinary.close();

	// Create the Colums of the GridView and set the Header-Text for each Colum
	LvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	LvCol.cx = 100;

	// First Colum is for showing the Order of the Bytes in a Step of 20 Bytes
	vTempText = "Bytes";
	LvCol.pszText = (LPSTR)vTempText.c_str();
	SendMessage(HandleGridView, LVM_INSERTCOLUMN, vColNum, (LPARAM)&LvCol);

	// Reset the Column with back to 60 (for the File-Data)
	LvCol.cx = 60;
	for (vColNum = 0; vColNum < 20; ++vColNum) {

		vTempText = "Byte " + to_string(vColNum);
		LvCol.pszText = (LPSTR)vTempText.c_str();
		SendMessage(HandleGridView, LVM_INSERTCOLUMN, vColNum + 1, (LPARAM)&LvCol);

	};

	// Write the Filedata into the Tabel
	// Stop if the last Byte is reached
	// 2 Loops, the outer one to switch trough the Rows, and the inner one to switch trough the Colums
	// Stop Adding new Elements if the last Byte is reached
	// Decode the Frame-Lenght to only print out the Frame-Data
	// Show and update Loading Progress
	vCurrentProgress = 0;
	vProgressProcent = 0;
	LvItem.mask = LVIF_TEXT;
	LvItem.cchTextMax = vActiveViewtypeCharacterCount;

	// Switch Rows
	for (vRowNum = 0; vCurrentProgress < vFileLenght; ++vRowNum) {

		// Switch Colums
		for (vColNum = 0; vColNum <= 20 and vCurrentProgress < vFileLenght; ++vColNum) {

			// Check for Frame-Header / New Frame / First Frame
			// Increment the Byte-Pointer until a Frame-Header is found
			if (vFrameLength == 0 or vFrameByteswriten == vFrameLength) {

				while (vCurrentProgress < vFileLenght and vFileData[vCurrentProgress] != 255 and vFileData[vCurrentProgress + 1] < 224) {

					++vCurrentProgress;

				};

				// Reset the Framebytes writen and
				// Reset Framelenght because of New Frame
				vFrameByteswriten = 0;
				vFrameLength = 0;
			};


			// New Frame detected, get Frame-Information
			if (vFrameByteswriten == 0 and vFrameLength == 0) {

				// Decode the Frameheader to get the Informations
				vFrameinfo = MP3_Frame_Decoder(&vFileData[vCurrentProgress]);
				// Save Framelenght to loop for the Framebytes
				vFrameLength = vFrameinfo.FRAMELENGHT;


				// If New Frame detected, goto next Row and reset the Colums if needed
				// Create an empty Row to different the Frames
				// If vColNum is not allready 0, goto Next Row and set vColnum to 0
				// Cout up vFrameCount
				// If Framelenght is less than 4, it couldt not be a valid Frame (Header is allready 4Bytes long and included in the Framelenght)
				if (vFrameLength > 4) {

					++vFrameCount;

					if (vColNum != 0) {

						vColNum = 0;
						++vRowNum;

					};

					// Set New Text for the Row
					LvItem.iItem = vRowNum;
					LvItem.iSubItem = vColNum;
					vTempText = "Frame " + to_string(vFrameCount) + " L: " + to_string(vFrameLength);
					LvItem.pszText = (LPSTR)vTempText.c_str();
					ListView_InsertItem(HandleGridView, &LvItem);

					// Update vColNum and vRowNum to the next Line
					vColNum = 0;
					++vRowNum;
					// Push the vCurrentProgress-Pointer to the first Framebyte
					// Reduce the Framelenght by 4, because the 4 Header Bytes are included in the Lenght
					vCurrentProgress = vCurrentProgress + 4;
					vFrameLength = vFrameLength - 4;

					// Save Frame-Data-Startbyte, Frame-Data-Lenght, Frame-Number and Frame-Start-Row in the GridView to the Structure
					// This makes it easyer to save the Data back to a File later
					vTempFrameLenght.FrameByteLenght = vFrameLength;
					vTempFrameLenght.FrameStartByte = vCurrentProgress;
					vTempFrameLenght.FrameNumber = vFrameCount;
					vTempFrameLenght.FrameStartRow = vRowNum;
					vFrameLenghts.push_back(vTempFrameLenght);

				}
				// Framelenght less than 4 -> Invalid Frame
				else {

					// Cout up vFrameCount
					++vFrameCount;

					// Create an empty Row to different the Frames
					// If vColNum is not allready 0, goto Next Row and set vColnum to 0
					if (vColNum != 0) {

						vColNum = 0;
						++vRowNum;

					};

					// Set New Text for the Row
					LvItem.iItem = vRowNum;
					LvItem.iSubItem = vColNum;
					vTempText = "Frame " + to_string(vFrameCount) + " is Invalid";
					LvItem.pszText = (LPSTR)vTempText.c_str();
					ListView_InsertItem(HandleGridView, &LvItem);

					// Save Frame-Data-Startbyte, Frame-Data-Lenght, Frame-Number and Frame-Start-Row in the GridView to the Structure
					// This makes it easyer to save the Data back to a File later
					// Set Frame-Lenght to 0 to Skip it while Saving -> Get this Data just from the File-Data-Buffer
					vTempFrameLenght.FrameByteLenght = 0;
					vTempFrameLenght.FrameStartByte = vCurrentProgress;
					vTempFrameLenght.FrameNumber = vFrameCount;
					vTempFrameLenght.FrameStartRow = vRowNum;
					vFrameLenghts.push_back(vTempFrameLenght);

					// goto next byte and restart the inner Loop to search for the next valid Frame
					vFrameLength = 0;
					++vCurrentProgress;
					continue;

				};
					
			};

			// Valid-Frame - Write the Text and File-Data to the GridView
			// Set the first Text of the New Row in the Frame
			if (vColNum == 0) {

				LvItem.iItem = vRowNum;
				LvItem.iSubItem = vColNum;
				vTempText = "Framebyte " + to_string(vFrameByteswriten);
				LvItem.pszText = (LPSTR)vTempText.c_str();
				ListView_InsertItem(HandleGridView, &LvItem);
				++vColNum;

			};


			// Load the Next Byte and store it into the temp Value
			vActualByte = vFileData[vCurrentProgress];
			
			// Set the Values of the GridView for this cycle
			LvItem.iItem = vRowNum;
			LvItem.iSubItem = vColNum;

			// Convert the Data-Byte to a readeble String depending on the selectet View-Type
			switch (vViewtype) {

			case 1:

				// HEX
				vConvertedText = ByteToHexString(vActualByte);
				LvItem.pszText = (LPSTR)vConvertedText.c_str();
				break;

			case 2:

				// BIN
				vConvertedText = bitset<8>(vActualByte).to_string();
				LvItem.pszText = (LPSTR)vConvertedText.c_str();
				break;

			case 3:

				// CHAR
				vConvertedText = char(vActualByte);
				LvItem.pszText = (LPSTR)vConvertedText.c_str();
				break;

			};

			// Update Current Progress and Pointers
			++vCurrentProgress;
			++vFrameByteswriten;
			// Set the Text
			ListView_SetItem(HandleGridView, &LvItem);
		};

		// After each Row
		// Calculate Procentege of Progress
		// Update HandleLoadingbar Text
		if (vProgressProcent < floor(vCurrentProgress * 100 / vFileLenght)) {

			vProgressProcent = floor(vCurrentProgress * 100 / vFileLenght);
			vLoadprogress = vLanguagepack[23] + to_string(vProgressProcent) + "%";
			SetWindowText(HandleLoadingbar, vLoadprogress.c_str());
			UpdateWindow(HandleWinEditorViewer);

		};


	};

	// Dont Show Loadingbar anymore
	ShowWindow(HandleLoadingbar, SW_HIDE);

	// Set the CallBackHandler for the GriedView and the GriedViewEditor Windows
	SetWindowSubclass(HandleGridViewEditor, InPlaceEditControl_SubclassProc, 0, 0);
	SetWindowSubclass(HandleGridView, ListViewSubclassProc, 0, 0);

	//Show the created Window;
	ShowWindow(HandleWinEditorViewer, SW_SHOW);
	ShowWindow(HandleGridView, SW_SHOW);
	ShowWindow(HandleButtonSettings, SW_SHOW);
	ShowWindow(HandleButtonSave, SW_SHOW);
	ShowWindow(HandleButtonSaveNew, SW_SHOW);
	ShowWindow(HandleButtonCancel, SW_SHOW);
	SetForegroundWindow(HandleWinEditorViewer);
	SetFocus(HandleWinEditorViewer);
	UpdateWindow(HandleWinEditorViewer);




	// User Interaction
	bool running = true;
	while (running) {

		while (PeekMessage(&MessageEditorViewer, 0, 0, 0, PM_REMOVE)) {


			// Update Element Text (to get new text in case of new Language)
			if (vUpdateLanguage[4]) {

				SetWindowText(HandleWinEditorViewer, vLanguagepack[4].c_str());
				SetWindowText(HandleButtonSettings, vLanguagepack[1].c_str());
				SetWindowText(HandleButtonCancel, vLanguagepack[6].c_str());
				SetWindowText(HandleButtonSave, vLanguagepack[24].c_str());
				SetWindowText(HandleButtonSaveNew, vLanguagepack[25].c_str());
				vChangeCountText = vLanguagepack[30] + to_string(vChangeCount);
				SetWindowText(HandleChangeCount, vChangeCountText.c_str());
				UpdateWindow(HandleWinEditorViewer);
				// deactivate Update Flag
				vUpdateLanguage[4] = false;

			};

			// Close Editor
			if (MessageEditorViewer.message == WM_QUIT or LOWORD(MessageEditorViewer.wParam) == ID_BUTTON_EDITOR_CLOSE) {

				// Only Close Window if no Changes has been done or if File is saved
				if (vFileNotSaved == true) {

					if (MessageBox(NULL, vLanguagepack[16].c_str(), vLanguagepack[17].c_str(), MB_ICONERROR | MB_OKCANCEL | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST) == IDOK) {

						// Close Editor and Return to past Window only if Okay is presseds
						running = false;

					};

				}
				else {

					// Close Editor and Return to past Window if no Changes has been done or if File is saved
					running = false;

				};

			}
			else if (MessageEditorViewer.message == WM_COMMAND) {

				
				switch (LOWORD(MessageEditorViewer.wParam)) {


				case ID_BUTTON_START_SETTINGS:

					// Open Settings
					// Hand Over Current Window Informations
					SettingsMenu(hInstance, HandleWinEditorViewer);
					break;

				case ID_BUTTON_EDITOR_SAVE:

					// Save to File
					// Check if User really want to overwrite File
					if (MessageBox(NULL, vLanguagepack[26].c_str(), vLanguagepack[17].c_str(), MB_ICONERROR | MB_OKCANCEL | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST) == IDOK) {

						// Call the SaveFile Function
						if (SaveFile(hInstance, HandleGridView, HandleWinEditorViewer, vFilePath, vFileLenght, vViewtype, vFileData, vFrameLenghts, vFrameCount) == 0) {
							// Close Editor and Return to past Window only if File is Saved
							running = false;
						};

					};
					break;

				case ID_BUTTON_EDITOR_SAVE_NEW: {

					// Save to New File
					// Create Variables for Filehandle - Dialog Winodow to Open / Save File
					vFilePath[0] = 0;
					OPENFILENAME vSaveFilehandle = {};
					vSaveFilehandle.lStructSize = sizeof(OPENFILENAME);
					vSaveFilehandle.hInstance = 0;
					vSaveFilehandle.lpstrFile = vFilePath;
					vSaveFilehandle.nMaxFile = vFilePathLenght;
					vSaveFilehandle.lpstrFilter = ".mp3\0*.mp3";
					vSaveFilehandle.lpstrTitle = vLanguagepack[25].c_str();
					vSaveFilehandle.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
					vSaveFilehandle.lpstrDefExt = "mp3";

					// Dialog-Window to select the Path and new Filename, it is possible to overwrite a existing File
					if (GetSaveFileName(&vSaveFilehandle) != 0) {

						// Call the SaveFile Function
						if (SaveFile(hInstance, HandleGridView, HandleWinEditorViewer, vFilePath, vFileLenght, vViewtype, vFileData, vFrameLenghts, vFrameCount) == 0) {
							// Close Editor and Return to past Window only if File is Saved
							running = false;
						};
					};
					};
					break;

				default:
					break;

				};

			}
			else if (LOWORD(MessageEditorViewer.wParam == ID_EDITOR_CHANGE)) {

				// In case of a Change, Count Up and Update the Changecount Window
				// And set the FileNotSaved Bit to True, so you cant close the Programm without an extra Dialog
				++vChangeCount;
				vFileNotSaved = true;
				vChangeCountText = vLanguagepack[30] + to_string(vChangeCount);
				SetWindowText(HandleChangeCount, vChangeCountText.c_str());
				UpdateWindow(HandleWinEditorViewer);

			};

			TranslateMessage(&MessageEditorViewer);
			DispatchMessage(&MessageEditorViewer);

		};

	};


	// Close the Editor Viewer and return to Main Menu
	DestroyWindow(HandleWinEditorViewer);
	DestroyWindow(HandleGridView);
	return 0;
};



// Convert a Byte to a HEX String, Only needed if HEX is the active Viewtype
string ByteToHexString(byte inbyte) {

	// Idea from Stackoverflow - Edited to fit to this Programm
	// If Input is allready 0 then just return 0
	if (inbyte == 0) {

		return "00";

	};

	// define needed Variables
	string hexChars = "0123456789ABCDEF";
	string result;

	//Convert the Byte-Value to a HEX-String
	while (inbyte > 0) {

		result = hexChars[inbyte % 16] + result;
		inbyte /= 16;

	};

	// If Lenght is only 1, add a leading 0 to the string
	if (result.length() == 1) {

		result = "0" + result;

	};

	// Return the HEX-String
	return result;

};


// Decoder for the MP3 Frame-Header
MP3Framinfo MP3_Frame_Decoder(byte Headerbytes[4]) {

	// Struct for Frame Informations and needed Variables
	MP3Framinfo vFrameinfo{};
	MP3Bitrates vBitrates;
	byte vTempByte = 0;
	int vBitrateType = 0;
	int vPadding = 0;


	// Headerbyte 0 should be '11111111' or 255
	// Headerbyte 1 should be >= 224
	if (Headerbytes[0] == 255 and Headerbytes[1] >= 224) {


		// Get MPEG-Version
		// 0 = V2.5
		// 1 = Reserved
		// 2 = V2
		// 3 = V1
		vFrameinfo.MPEG_VERSION = (Headerbytes[1] & 0b00011000) >> 3;

		// Get Layer
		// 0 = Reserved
		// 1 = Layer 3
		// 2 = Layer 2
		// 3 = Layer 1
		vFrameinfo.LAYER = (Headerbytes[1] & 0b00000110) >> 1;

		// CRC Protection
		vFrameinfo.CRC_PROTECTION = (Headerbytes[1] & 0b00000001);

		// Bitrate in kbps / V1 & L1 / V1 & L2 / V1 & L3 / V2 & L1 / V2 & L2 or L3
		// 
		// 0000 = free / free / free / free / free
		// 0001 = 32 / 32 / 32 / 32 / 8
		// 0010 = 64 / 48 / 40 / 48 / 16
		// 0011 = 96 / 56 / 48 / 56 / 24
		// 0100 = 128 / 64 / 56 / 64 / 32
		// 0101 = 160 / 80 / 64 / 80 / 40
		// 0110 = 192 / 96 / 80 / 96 / 48
		// 0111 = 224 / 112 / 96 / 112 / 56
		// 1000 = 256 / 128 / 112 / 128 / 64
		// 1001 = 288 / 160 / 128 / 144 / 80
		// 1010 = 320 / 192 / 160 / 160 / 96
		// 1011 = 352 / 224 / 192 / 176 / 112
		// 1100 = 384 / 256 / 224 / 192 / 128
		// 1101 = 416 / 320 / 256 / 224 / 144
		// 1110 = 448 / 384 / 320 / 256 / 160
		// 1111 = bad / bad / bad / bad / bad
		// 
		// Get the correct Colum for the Structure
		if (vFrameinfo.MPEG_VERSION == 3 and vFrameinfo.LAYER == 3) { vBitrateType = 0; };
		if (vFrameinfo.MPEG_VERSION == 3 and vFrameinfo.LAYER == 2) { vBitrateType = 1; };
		if (vFrameinfo.MPEG_VERSION == 3 and vFrameinfo.LAYER == 1) { vBitrateType = 2; };
		if (vFrameinfo.MPEG_VERSION == 2 and vFrameinfo.LAYER == 3) { vBitrateType = 3; };
		if (vFrameinfo.MPEG_VERSION == 2 and vFrameinfo.LAYER <= 2) { vBitrateType = 4; };

		// Load the correct Bitrate
		vTempByte = (Headerbytes[2] & 0b11110000) >> 4;
		switch (vTempByte) {
		case 0:		vFrameinfo.BITRATE = vBitrates.Rate_0[vBitrateType];	break; // Free
		case 1:		vFrameinfo.BITRATE = vBitrates.Rate_1[vBitrateType];	break;
		case 2:		vFrameinfo.BITRATE = vBitrates.Rate_2[vBitrateType];	break;
		case 3:		vFrameinfo.BITRATE = vBitrates.Rate_3[vBitrateType];	break;
		case 4:		vFrameinfo.BITRATE = vBitrates.Rate_4[vBitrateType];	break;
		case 5:		vFrameinfo.BITRATE = vBitrates.Rate_5[vBitrateType];	break;
		case 6:		vFrameinfo.BITRATE = vBitrates.Rate_6[vBitrateType];	break;
		case 7:		vFrameinfo.BITRATE = vBitrates.Rate_7[vBitrateType];	break;
		case 8:		vFrameinfo.BITRATE = vBitrates.Rate_8[vBitrateType];	break;
		case 9:		vFrameinfo.BITRATE = vBitrates.Rate_9[vBitrateType];	break;
		case 10:	vFrameinfo.BITRATE = vBitrates.Rate_10[vBitrateType];	break;
		case 11:	vFrameinfo.BITRATE = vBitrates.Rate_11[vBitrateType];	break;
		case 12:	vFrameinfo.BITRATE = vBitrates.Rate_12[vBitrateType];	break;
		case 13:	vFrameinfo.BITRATE = vBitrates.Rate_13[vBitrateType];	break;
		case 14:	vFrameinfo.BITRATE = vBitrates.Rate_14[vBitrateType];	break;
		case 15:	vFrameinfo.BITRATE = vBitrates.Rate_15[vBitrateType];	break; // Bad
		};

		// Sampling Rate in Hz
		// 00 = 44100
		// 01 = 48000
		// 10 = 32000
		// 11 = Reserved
		vTempByte = (Headerbytes[2] & 0b00001100) >> 2;
		switch (vTempByte) {
		case 0:		vFrameinfo.SAMPLINGRATE = 44100;	break;
		case 1:		vFrameinfo.SAMPLINGRATE = 48000;	break;
		case 2:		vFrameinfo.SAMPLINGRATE = 32000;	break;
		case 3:		vFrameinfo.SAMPLINGRATE = 0;		break; // Reserved
		};

		// Padding
		vFrameinfo.PADDING = (Headerbytes[2] & 0b00000010) >> 1;
		if (vFrameinfo.PADDING) { vPadding = 1; };

		// Privacy Bit is Optinal

		// Chanel
		// 00 = Stereo
		// 01 = Joint Stereo
		// 10 = Dual
		// 11 = Mono
		vFrameinfo.CHANNEL = (Headerbytes[3] & 0b11000000) >> 6;

		// Mode Extention
		vFrameinfo.MODE_EXTENTION = (Headerbytes[3] & 0b00110000) >> 4;

		// Copyright
		vFrameinfo.COPYRIGHT = (Headerbytes[3] & 0b00001000) >> 3;

		// Original
		vFrameinfo.ORIGINAL = (Headerbytes[3] & 0b00000100) >> 2;

		// Emphasis
		vFrameinfo.EMPHASIS = (Headerbytes[3] & 0b00000011);

		// Calculate Framelenght
		vFrameinfo.FRAMELENGHT = int((144 * vFrameinfo.BITRATE * 1000 / vFrameinfo.SAMPLINGRATE) + vPadding);

		return vFrameinfo;

	}
	else {

		return {};

	};

};


// Save File Function, Used for "Save File" and "Save as new File"
int SaveFile(HINSTANCE hInstance, HWND HandleGridView, HWND HandleWinEditorViewer, char vFilePath[vFilePathLenght], INT64 vFileLenght, int vViewtype, vector <byte> vFileData, vector <MP3Framelenghts> vFrameLenghts, INT64 vFrameCount) {

	// Create Variables to handle the Table Data and write it to the File
	// INT64 to handle large Files
	ofstream vOutPutFile;
	INT64 vRowNum = 0;
	INT64 vColNum = 0;
	INT64 vCurrentByteCount = 0;
	INT64 vCurrentFrameByteCount = 0;
	string vSubItemText;
	byte vCurrentByte = 0;
	int vProgressProcent = 0;


	// Open the File
	vOutPutFile.open(vFilePath, ios::binary);
	// Check if Outputfile is opend / created
	if (!vOutPutFile.is_open()) {

		// File Not Found
		MessageBox(NULL, vLanguagepack[27].c_str(), "Error", MB_ICONERROR | MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
		return 100;

	};


	// If File is found / created then hide the GridView and Show SaveProgressBar
	ShowWindow(HandleGridView, SW_HIDE);
	UpdateWindow(HandleWinEditorViewer);
	// Create Saving Progress Window
	HWND HandleSavebar;
	string vSaveprogress = vLanguagepack[23] + "0%";
	HandleSavebar = CreateWindow("STATIC", vSaveprogress.c_str(), WS_VISIBLE | WS_CHILD | SS_LEFT, 600, 385, 200, 30, HandleWinEditorViewer, NULL, hInstance, NULL);


	// Loop trough the FileData, GridView and Framlenghts
	// Get the Frame-Data from the Gridview, and Frame-Header and other Bytes from the puffered vFiledata
	// Use the Data from the vFrameLenghts to Get the Informations about the Frame in the GridView
	// Depending on the Viewmethod convert the Data back to Bytes

	INT64 vFrame = 0;

	for (vCurrentByteCount = 0; vCurrentByteCount < vFileLenght;) {


		// Bytes bevor or between the Frames. (ID3 Data, Frame-Header and so on)
		// Check if Last Frame is allready processd, to avoid an Accesserror
		if (vFrame < vFrameCount) {

			// Wirte to File from Buffer until Next Frame is Starting
			while (vCurrentByteCount < vFrameLenghts[vFrame].FrameStartByte) {

				// Write the Non-Frame Data to the File
				// No need to convert it back to Byte
				vOutPutFile << vFileData[vCurrentByteCount];
				++vCurrentByteCount;

			};

		}
		// Last Frame is allraedy written, just write the last Bytes to the Files
		else {

			// Wirte to File from Buffer until Next Frame is Starting
			while (vCurrentByteCount < vFileLenght) {

				// Write the Non-Frame Data to the File
				// No need to convert it back to Byte
				vOutPutFile << vFileData[vCurrentByteCount];
				++vCurrentByteCount;
				
			};
			// Continue to Exit the Loop
			continue;
		};



		// Check if Frame is Valid
		// If Valid, the read the hole Framedata from the Gridview
		// Else, just go to next Frame
		if (vFrameLenghts[vFrame].FrameByteLenght > 0) {

			// Outer Loop to switch the GridView Rows
			// Start at the Rownumber stored in vFrameLenghts of the Frame
			// Stop when all Bytes of the Frame are processed
			// Reset the vCurrentFrameByteCount
			vCurrentFrameByteCount = 0;
			for (vRowNum = vFrameLenghts[vFrame].FrameStartRow; vCurrentFrameByteCount < vFrameLenghts[vFrame].FrameByteLenght; ++vRowNum) {

				// Inner Loop to switch the GridView Colums
				// Allways Start with index 1 - 0 is the Frambyte Colum
				// Stop when all Bytes of the Frame are processed, Next Row after each 20 Colums
				for (vColNum = 1; vColNum <= 20 and vCurrentFrameByteCount < vFrameLenghts[vFrame].FrameByteLenght; ++vColNum) {

					// Get the Item Text of the Sub-Element from the GridView
					ListView_GetItemText(HandleGridView, vRowNum, vColNum, (LPSTR)vSubItemText.c_str(), 9);
					// Convert the String back into a Byte
					switch (vViewtype) {

					case 1:

						// HEX
						vCurrentByte = stoi(vSubItemText, nullptr, 16);
						break;

					case 2:

						// BIN
						vCurrentByte = stoi(vSubItemText, nullptr, 2);
						break;

					case 3:

						// CHAR
						vCurrentByte = vSubItemText[0];
						break;
					};

					// Write the Converted Framebyte back to the File
					vOutPutFile << vCurrentByte;
					++vCurrentByteCount;
					++vCurrentFrameByteCount;

				};

			};

		};

		// Frame is Proccesed, go to next Frame
		++vFrame;

		// After each Frame
		// Calculate Procentege of Progress
		// Update HandleSaveBar Text
		if (vProgressProcent < floor(vCurrentByteCount * 100 / vFileLenght)) {

			vProgressProcent = floor(vCurrentByteCount * 100 / vFileLenght);
			vSaveprogress = vLanguagepack[28] + to_string(vProgressProcent) + "%";
			SetWindowText(HandleSavebar, vSaveprogress.c_str());
			UpdateWindow(HandleWinEditorViewer);

		};

	};


	// Close the Output File
	vOutPutFile.close();
	// Return 0, The File got saved sucessfuly
	return 0;

};