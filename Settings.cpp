// Include Basic Funktions
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>

// Include additional Modules / Functions
#include "User_Types.h"
#include "MessageHandler.h"
#include "Languagepack.h"

// Activate Namespace
using namespace std;

// Global Variables
extern Settings vSettings;
extern string vLanguagepack[];
extern bool vUpdateLanguage[];

// Later defined Functions
int SaveSettings();
int SettingsMenu(HINSTANCE hInstance, HWND LastWindowHandle);
int LoadLastFiles();
int SaveLastFiles(char vNewFilepath[vFilePathLenght]);

// Load the Settings from the Settings.conf File
int LoadSettings() {

	// Init needed Variables
	int vCurrentLine = 0;
	ifstream vSettingsFile;
	string vSettingLines[10];
	int vSettingLinesLoaded = 0;


	// Open the .conf file
	vSettingsFile.open("Settings.conf");


	// Make sure the file was found
	if (!vSettingsFile.is_open()) {

		// File not found
		return 100;

	}
	else {

		// File is found, then load the lines
		// Stop when the end of the File is reached or Limit of 150 Lines is reached (to avoid Looperror)
		while (!vSettingsFile.eof() and vCurrentLine < 150) {

			// Create the needed Variables
			string vActualLine;

			// Load the current Line from the .conf
			getline(vSettingsFile, vActualLine);

			// Lines starting with # are Comments
			// Lines that or not emtpy and not starting with # ore F are the Settings
			// Only save the Lines with Settings
			// < 10 to avoid Looperror
			if (vActualLine.length() > 0 and vActualLine[0] != '#' and vActualLine[0] != 'F' and vSettingLinesLoaded < 10) {


				// Save the Valid Settings Line
				vSettingLines[vSettingLinesLoaded] = vActualLine;
				// Count up the Pointer
				++vSettingLinesLoaded;

			};

			// Count up the Pointer
			++vCurrentLine;

		};
	};

	// Close the File
	vSettingsFile.close();


	// Extrakt the Settings and Values
	// Create the needed Variables
	string vActuallSetting;
	int vActuallValue = 0;
	int vTempPointer = 0;
	int vTempSplipos = 0;
	int vTempLength = 0;

	// < 10 to avoid Loop
	while (vTempPointer < vSettingLinesLoaded and vTempPointer < 10) {

		// Split current Line into Settings Name and Value
		// Get Lenght and Split Position
		// Then Store the Name and Value to check
		vTempSplipos = vSettingLines[vTempPointer].find('=');
		vTempLength = vSettingLines[vTempPointer].length();
		vActuallSetting = vSettingLines[vTempPointer].substr(0, vTempSplipos);
		vActuallValue = stoi(vSettingLines[vTempPointer].substr(vTempSplipos + 1, vTempLength));

		// Check Typ of Setting 
		// If valid Typ detected, check if Value is valid
		// Set the Value
		if (vActuallSetting == "Language" and vActuallValue > 0 and vActuallValue <= 4) {

			vSettings.Language = vActuallValue;

		}
		else if (vActuallSetting == "TextColor" and vActuallValue > 0 and vActuallValue < 255) {

			vSettings.TextColor = vActuallValue;

		}
		else if (vActuallSetting == "DefaultTyp" and vActuallValue > 0 and vActuallValue <= 3) {

			vSettings.DefaultTyp = vActuallValue;

		}
		else if (vActuallSetting == "TextHighlightColor" and vActuallValue > 0 and vActuallValue < 255) {

			vSettings.TextHighlightColor = vActuallValue;

		}
		else {

			string vErrorString = "Invalid Settings, for Value " + vSettingLines[vTempPointer] + ". Default value got loaded";
			MessageBox(NULL, vErrorString.c_str(), "Error", MB_OK);

		};


		++vTempPointer;

	};


	// Sucessfull load of Settings
	return 0;

};

// Save the Settings to the Settings.conf File if User wants the selection to Stay active with the next Start of the Software
int SaveSettings() {


	// Create needed Variables
	int vTempPointer = 0;
	int vTempPointer_2 = 0;
	ifstream vSettingsFileRead;
	ofstream vSettingsFileWrite;
	string vSettingsLines[150];
	int vCurrentLine = 0;
	int vLineCount = 0;


	// First open the .conf file and Read all Lines
	vSettingsFileRead.open("Settings.conf");

	// Make sure the file was found
	if (!vSettingsFileRead.is_open()) {

		// File not found
		return 100;

	}
	else {

		// File is found
		while (!vSettingsFileRead.eof() and vCurrentLine < 150) {

			// Load all Lines of Settings File
			getline(vSettingsFileRead, vSettingsLines[vCurrentLine]);

			++vLineCount;
			++vCurrentLine;
		};

	};
	// Close the File
	vSettingsFileRead.close();



	// Upadte SettingsLines in Temp Storage
	vTempPointer = 0;
	vTempPointer_2 = 0;

	while (vTempPointer < vLineCount) {

		if (vSettingsLines[vTempPointer].length() > 0 and vSettingsLines[vTempPointer][0] != 'F' and vSettingsLines[vTempPointer][0] != '#') {


			// Split the Settingstype from its Value
			int vTempSplitpos = vSettingsLines[vTempPointer].find('=');
			int vTempLength = vSettingsLines[vTempPointer].length();
			string vActuallSetting = vSettingsLines[vTempPointer].substr(0, vTempSplitpos + 1);


			// Check the Type of Setting and Update Settingsvalue 
			// Check if Settingsvalue is valid, else ignore it
			if (vActuallSetting == "Language=" and vSettings.Language > 0 and vSettings.Language <= 4) {

				vSettingsLines[vTempPointer] = vActuallSetting + to_string(vSettings.Language);

			}
			else if (vActuallSetting == "TextColor=" and vSettings.TextColor > 0 and vSettings.TextColor < 255) {

				vSettingsLines[vTempPointer] = vActuallSetting + to_string(vSettings.TextColor);

			}
			else if (vActuallSetting == "DefaultTyp=" and vSettings.DefaultTyp > 0 and vSettings.DefaultTyp <= 3) {

				vSettingsLines[vTempPointer] = vActuallSetting + to_string(vSettings.DefaultTyp);

			}
			else if (vActuallSetting == "TextHighlightColor=" and vSettings.TextHighlightColor > 0 and vSettings.TextHighlightColor < 255) {

				vSettingsLines[vTempPointer] = vActuallSetting + to_string(vSettings.TextHighlightColor);

			}
			else {};

		};

		++vTempPointer;
	};



	// Write Data back to File
	vSettingsFileWrite.open("Settings.conf");
	vCurrentLine = 0;

	// Make sure the file was found
	if (!vSettingsFileWrite.is_open()) {

		// File not found
		return 100;

	}
	else {

		// File is found
		while (vCurrentLine < vLineCount and vCurrentLine < 150) {

			// Write all Lines of Settings File
			vSettingsFileWrite << vSettingsLines[vCurrentLine];
			vSettingsFileWrite << endl;
			++vCurrentLine;
		};

	};

	// Close the File
	vSettingsFileWrite.close();

	return 0;

};


// Update the Paths of the last 10 opend Files in the Settings.conf File
int SaveLastFiles(char vNewFilepath[vFilePathLenght]) {

	// Create needed Variables
	// Check if Path is allready in List of last opend Files
	int vTempPointer = 0;
	int vTempPointer_2 = 0;
	while (vTempPointer < 10) {


		if (vSettings.LastFiles[vTempPointer] == vNewFilepath) {

			// If File allready exists in List, do nothing and return
			return 0;
		};

		++vTempPointer;

	};


	// If the Filepath is new, push down the last Filepaths once, and save the new one on first position
	// Need to Start from button to top
	vTempPointer = 9;
	while (vTempPointer > 0) {

		vSettings.LastFiles[vTempPointer] = vSettings.LastFiles[vTempPointer - 1];
		--vTempPointer;

	};
	vSettings.LastFiles[0] = vNewFilepath;


	// write the last Filepahts to the .conf File
	// Init needed Variables
	ifstream vSettingsFileRead;
	ofstream vSettingsFileWrite;
	string vSettingsLines[150];
	int vCurrentLine = 0;
	int vLineCount = 0;
	vTempPointer = 0;

	// First open the .conf file and Read all Lines
	vSettingsFileRead.open("Settings.conf");

	// Make sure the file was found
	if (!vSettingsFileRead.is_open()) {

		// File not found
		return 100;

	}
	else {
		
		// File is found
		while (!vSettingsFileRead.eof() and vCurrentLine < 150) {
		
			// Load all Lines of Settings File
			getline(vSettingsFileRead, vSettingsLines[vCurrentLine]);

			++vLineCount;
			++vCurrentLine;
		};
	
	};

	// Close the File
	vSettingsFileRead.close();


	// Upadte Filepaths in Temp Storage
	vTempPointer = 0;
	vTempPointer_2 = 0;

	while (vTempPointer < vLineCount) {

		if (vSettingsLines[vTempPointer][0] == 'F') {

			// Split the non-paht Part from the Line and update it with the new path
			int vTempSplitpos = vSettingsLines[vTempPointer].find('=');
			int vTempLength = vSettingsLines[vTempPointer].length();
			string vFile = vSettingsLines[vTempPointer].substr(0, vTempSplitpos + 1);

			// Update the Line
			vSettingsLines[vTempPointer] = vFile + vSettings.LastFiles[vTempPointer_2];
			++vTempPointer_2;
		};

		++vTempPointer;
	};



	// Write Data back to File
	vSettingsFileWrite.open("Settings.conf");
	vCurrentLine = 0;

	// Make sure the file was found
	if (!vSettingsFileWrite.is_open()) {

		// File not found
		return 100;

	}
	else {

		// File is found
		while (vCurrentLine < vLineCount and vCurrentLine < 150) {

			// Write all Lines of Settings File
			vSettingsFileWrite << vSettingsLines[vCurrentLine];
			vSettingsFileWrite << endl;
			++vCurrentLine;
		};

	};

	// Close the File
	vSettingsFileWrite.close();

	return 0;

};

// Load the Paths of the last 10 opend Files from the Settings.conf File
int LoadLastFiles() {

	// First Load the Paths of the last opend Files
	// Init needed Variables
	ifstream vSettingsFile;
	int vCurrentLine = 0;
	int vFilePathLinesLoaded = 0;
	string vFilePathLines[10];


	// Open the .conf file
	vSettingsFile.open("Settings.conf");


	// Make sure the file was found
	if (!vSettingsFile.is_open()) {

		// File not found
		return 100;

	}
	else {

		// File is found, then load the lines
		// Stop when the end of the File is reached or Limit of 150 Lines is reached (to avoid Looperror)
		while (!vSettingsFile.eof() and vCurrentLine < 150) {

			// Create the needed Variables
			string vActualLine;

			// Load the current Line from the .conf
			getline(vSettingsFile, vActualLine);

			// Lines starting with # are Comments
			// Lines that or not emtpy and not starting with # but starting f´with F are the Filepaths
			// Only save the Lines with Filepaths
			// < 10 to avoid Looperror
			if (vActualLine.length() > 0 and vActualLine[0] == 'F' and vFilePathLinesLoaded <= 10) {


				// Save the Valid Settings Line
				vFilePathLines[vFilePathLinesLoaded] = vActualLine;
				// Count up the Pointer
				++vFilePathLinesLoaded;

			};

			
			// Count up the Pointer
			++vCurrentLine;

		};
	};

	// Close the File
	vSettingsFile.close();


	// Extrakt the Settings and Values
	// Create the needed Variables
	string vActuallSettings;
	string vActuallFilepath;
	int vTempPointer = 0;
	int vTempSplitpos_1 = 0;
	int vTempSplitpos_2 = 0;
	int vTempLength = 0;

	// < 10 to avoid Loop
	while (vTempPointer < vFilePathLinesLoaded and vTempPointer < 10) {

		// Split current Line into Filepathnumber and Filepath
		// Get Lenght and Split Position
		// Then Store Filepath into global Structure
		vTempSplitpos_1 = vFilePathLines[vTempPointer].find('[');
		vTempSplitpos_2 = vFilePathLines[vTempPointer].find('=');
		vTempLength = vFilePathLines[vTempPointer].length();
		vActuallSettings = vFilePathLines[vTempPointer].substr(0, vTempSplitpos_1);
		vActuallFilepath = vFilePathLines[vTempPointer].substr(vTempSplitpos_2 + 1, vTempLength);

		// Number of Lastfilepath
		if (vActuallSettings == "File") {

			vSettings.LastFiles[vTempPointer] = vActuallFilepath;

		};


		++vTempPointer;

	};

	return 0;

};

// Menu Window for Settings
int SettingsMenu(HINSTANCE hInstance, HWND LastWindowHandle) {

	// Dont show the last Window and create new one
	ShowWindow(LastWindowHandle, SW_HIDE);

	// Create Variables
	// Set new Variables
	HWND HandleWinSettings;
	HWND HandleButtonLangauage;
	HWND HandleSelectionLanguage[5];
	HWND HandleButtonDefaultTyp;
	HWND HandleSelectionDefaultTyp[4];
	HWND HandleButtonSaveSettings;
	HWND HandleButtonCancel;

	MSG MessageSettings;
	WNDCLASS WinSettings;

	WinSettings = {};
	WinSettings.style = CS_HREDRAW | CS_VREDRAW;
	WinSettings.lpfnWndProc = MessageHandler;
	WinSettings.hInstance = hInstance;
	WinSettings.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WinSettings.lpszClassName = "WinSettings";


	// Register the WinSettings Parameters
	// Create the Window Handler for Settings Window
	RegisterClass(&WinSettings);
	HandleWinSettings = CreateWindow("WinSettings", vLanguagepack[1].c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, 0);

	// Create Elements for Settings Menu

	// Language Selection
	// GroupBox and Button
	HandleSelectionLanguage[0] = CreateWindow("Button", vLanguagepack[9].c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 340, 220, HandleWinSettings, (HMENU)0, hInstance, NULL);
	HandleButtonLangauage = CreateWindow("button", vLanguagepack[7].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 30, 160, 300, 50, HandleWinSettings, (HMENU)ID_BUTTON_SETTINGS_SET_LANGUAGE, hInstance, 0);
	// Radio Buttons -> Language Selection
	// Languages are a Child of Groupbox, Groupbox is a Child of HandleWinSettings
	HandleSelectionLanguage[1] = CreateWindow("Button", "1 - Deutsch", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 30, 150, 30, HandleSelectionLanguage[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLanguage[2] = CreateWindow("Button", "2 - Englisch", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 55, 150, 30, HandleSelectionLanguage[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLanguage[3] = CreateWindow("Button", "3 - Spansich", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 80, 150, 30, HandleSelectionLanguage[0], (HMENU)0, hInstance, NULL);
	HandleSelectionLanguage[4] = CreateWindow("Button", "4 - Chinesich", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 105, 150, 30, HandleSelectionLanguage[0], (HMENU)0, hInstance, NULL);
	// Preselect the current Language
	SendMessage(HandleSelectionLanguage[vSettings.Language], BM_SETCHECK, BST_CHECKED, 0);


	// Default Type Selection
	// GroupBox and Button
	HandleSelectionDefaultTyp[0] = CreateWindow("Button", vLanguagepack[19].c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 380, 10, 340, 220, HandleWinSettings, (HMENU)0, hInstance, NULL);
	HandleButtonDefaultTyp = CreateWindow("button", vLanguagepack[20].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 400, 160, 300, 50, HandleWinSettings, (HMENU)ID_BUTTON_SETTINGS_SET_DEFAULTTYPE, hInstance, 0);
	// Radio Buttons -> Typ Selection
	// Types are a Child of Groupbox, Groupbox is a Child of HandleWinSettings
	HandleSelectionDefaultTyp[1] = CreateWindow("Button", "1 - HEX", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 30, 150, 30, HandleSelectionDefaultTyp[0], (HMENU)0, hInstance, NULL);
	HandleSelectionDefaultTyp[2] = CreateWindow("Button", "2 - BIN", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 55, 150, 30, HandleSelectionDefaultTyp[0], (HMENU)0, hInstance, NULL);
	HandleSelectionDefaultTyp[3] = CreateWindow("Button", "3 - CHAR", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 20, 80, 150, 30, HandleSelectionDefaultTyp[0], (HMENU)0, hInstance, NULL);
	// Preselect the current Default Type
	SendMessage(HandleSelectionDefaultTyp[vSettings.DefaultTyp], BM_SETCHECK, BST_CHECKED, 0);	
	// Save Settings Button
	HandleButtonSaveSettings = CreateWindow("button", vLanguagepack[18].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 440, 340, 50, HandleWinSettings, (HMENU)ID_BUTTON_SETTINGS_SAVE, hInstance, 0);
	// Close Settings Button
	HandleButtonCancel = CreateWindow("button", vLanguagepack[6].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 500, 340, 50, HandleWinSettings, (HMENU)ID_BUTTON_SETTINGS_CLOSE, hInstance, 0);
	

	//Show the created Window;
	ShowWindow(HandleWinSettings, SW_SHOW);
	SetForegroundWindow(HandleWinSettings);
	SetFocus(HandleWinSettings);
	UpdateWindow(HandleWinSettings);


	// Variables to handle User Input
	int vLanguageSelected = 0;
	int vDefaultypeSelected = 0;


	// User Interaction
	bool running = true;
	while (running) {

		while (PeekMessage(&MessageSettings, 0, 0, 0, PM_REMOVE)) {


			if (MessageSettings.message == WM_QUIT) {

				// Close Settings and Return to past Window
				running = false;

			}
			else if (MessageSettings.message == WM_COMMAND) {

				switch (LOWORD(MessageSettings.wParam)) {

				case ID_BUTTON_SETTINGS_SET_LANGUAGE:


					// Check witch Language is selected
					// Store Result to later Check if new Language is selected
					if (SendMessage(HandleSelectionLanguage[1], BM_GETCHECK, 0, 0) == BST_CHECKED) {
						vLanguageSelected = 1;
					}
					else if (SendMessage(HandleSelectionLanguage[2], BM_GETCHECK, 0, 0) == BST_CHECKED){
						vLanguageSelected = 2;
					}
					else if (SendMessage(HandleSelectionLanguage[3], BM_GETCHECK, 0, 0) == BST_CHECKED) {
						vLanguageSelected = 3;
					}
					else if (SendMessage(HandleSelectionLanguage[4], BM_GETCHECK, 0, 0) == BST_CHECKED) {
						vLanguageSelected = 4;
					};


					// If selcted Language is different from current Settings, load the new Language
					// Check if Selection is valid
					if (vLanguageSelected > 0 and vLanguageSelected <= vMaxLanguages and vLanguageSelected != vSettings.Language) {

						// If new Language is valid, reload the Languagepack, save to vSettings and update Window.
						if (LoadLanguage(vLanguageSelected) == 0) {
							vSettings.Language = vLanguageSelected;
							MessageBox(NULL, vLanguagepack[11].c_str(), "Info", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);

							// Update Element Text after Language cahnge
							SetWindowText(HandleWinSettings, vLanguagepack[1].c_str());
							SetWindowText(HandleSelectionLanguage[0], vLanguagepack[9].c_str());
							SetWindowText(HandleButtonLangauage, vLanguagepack[7].c_str());
							SetWindowText(HandleSelectionDefaultTyp[0], vLanguagepack[19].c_str());
							SetWindowText(HandleButtonDefaultTyp, vLanguagepack[20].c_str());
							SetWindowText(HandleButtonSaveSettings, vLanguagepack[18].c_str());
							SetWindowText(HandleButtonCancel, vLanguagepack[6].c_str());

							UpdateWindow(HandleWinSettings);
							// deactivate Update Flag
							vUpdateLanguage[2] = false;
						};

					}
					else if (vLanguageSelected == vSettings.Language) {

						// If selected Language ist equal to loaded Language, do nothing
						break;

					}
					else {

						// Invalid Setting
						MessageBox(NULL, vLanguagepack[12].c_str(), "Error", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);

					};

					break;


				case ID_BUTTON_SETTINGS_SET_DEFAULTTYPE:

					
					// Check witch Defaulttype is selected
					// Store Result to later Check if new defaulttype is selected
					if (SendMessage(HandleSelectionDefaultTyp[1], BM_GETCHECK, 0, 0) == BST_CHECKED) {
						vDefaultypeSelected = 1;
					}
					else if (SendMessage(HandleSelectionDefaultTyp[2], BM_GETCHECK, 0, 0) == BST_CHECKED) {
						vDefaultypeSelected = 2;
					}
					else if (SendMessage(HandleSelectionDefaultTyp[3], BM_GETCHECK, 0, 0) == BST_CHECKED) {
						vDefaultypeSelected = 3;
					};
	
					// If selcted Defaulttype is different from current Settings, set the new Defaulttype
					// Check if Selection is valid
					if (vDefaultypeSelected > 0 and vDefaultypeSelected <= 3 and vDefaultypeSelected != vSettings.DefaultTyp) {

						MessageBox(NULL, vLanguagepack[21].c_str(), "Info", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
						vSettings.DefaultTyp = vDefaultypeSelected;

					}
					else if (vDefaultypeSelected == vSettings.DefaultTyp) {

						// If selected Language ist equal to loaded Language, do nothing
						break;

					}
					else {

						// Invalid Setting
						MessageBox(NULL, vLanguagepack[12].c_str(), "Error", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);

					};

					break;



				case ID_BUTTON_SETTINGS_SET_TEXTCOLOR:

					break;

				case ID_BUTTON_SETTINGS_SAVE:

					SaveSettings();
					running = false;
					break;

				case ID_BUTTON_SETTINGS_CLOSE:

					// Close Settings and Return to past Window
					running = false;
					break;

				default:
					break;

				};
				break;

			};


			TranslateMessage(&MessageSettings);
			DispatchMessage(&MessageSettings);

		};

	};


	// Close the Settings Window
	ShowWindow(HandleWinSettings, SW_HIDE);
	// Restore the last Window
	ShowWindow(LastWindowHandle, SW_SHOW);
	UpdateWindow(LastWindowHandle);


	return 0;

};
