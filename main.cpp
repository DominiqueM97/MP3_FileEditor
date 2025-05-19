// To Compile with MS Visual Studio: Set Projekt Propertys -> Linker -> System -> Sub System -> Set to not "not set"
// To Compile with MS Visual Studio: Set Projekt Propertys -> Extendet -> Encoding -> Multibyte Character Encoding
// Include Basic Funktions
#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>

// Include additional Modules / Functions
#include "Languagepack.h"
#include "Settings.h"
#include "User_Types.h"
#include "MessageHandler.h"
#include "Fileeditor.h"

// Activate Namespace
using namespace std;

// Startup Bit
bool vStartup = true;

// Global Useable Settings
Settings vSettings{};
// Global Useable Languagpack
string vLanguagepack[vMaxTextLines];
// Global Bit's for Updating text after Language-change
// 1 = MainMenu
// 2 = Settings
// 3 = Fileeditor Menu
// 4 = Fileeditor Viewer
bool vUpdateLanguage[10];

// Later defined Functions
int Programm_Startup();

// Main Funktion/Window - Start of the Prorgamm
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Error Code to handle Faults
    int vErrorCode = 0;
    if (vStartup == true) {

        // Inital Load of Settings an Languagepack
        Programm_Startup();
        // Finished inital Load of Settings
        vStartup = false;

    };
 

    // Create Variables
    // Set new Variables
    HWND HandleWinMain;
    HWND HandleButtonEditor;
    HWND HandleButtonSettings;
    HWND HandleButtonEnd;
    MSG MessageMainWindow;
    WNDCLASS WinMain;

    WinMain = {};
    WinMain.style = CS_HREDRAW | CS_VREDRAW;
    WinMain.lpfnWndProc = MessageHandler;
    WinMain.hInstance = hInstance;
    WinMain.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WinMain.lpszClassName = "WinMain";


    // Register the WinMain Parameters
    // Create the Window Handler for Main Window
    // Create Buttons for Main Menu
    RegisterClass(&WinMain);
    HandleWinMain = CreateWindow("WinMain", vLanguagepack[2].c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, 0);
    HandleButtonEditor = CreateWindow("button", vLanguagepack[4].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 10, 300, 50, HandleWinMain, (HMENU)ID_BUTTON_START_EDITOR, hInstance, 0);
    HandleButtonSettings = CreateWindow("button", vLanguagepack[1].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 70, 300, 50, HandleWinMain, (HMENU)ID_BUTTON_START_SETTINGS, hInstance, 0);
    HandleButtonEnd = CreateWindow("button", vLanguagepack[5].c_str(), WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 130, 300, 50, HandleWinMain, (HMENU)ID_BUTTON_CLOSE, hInstance, 0);


    //Show the created Window;
    ShowWindow(HandleWinMain, SW_SHOW);
    SetForegroundWindow(HandleWinMain);
    SetFocus(HandleWinMain);
    UpdateWindow(HandleWinMain);



    // User Interaction
    bool running = true;
    while (running) {

        while (PeekMessage(&MessageMainWindow, 0, 0, 0, PM_REMOVE)) {


            // Update Element Text (to get new text in case of new Language)
            if (vUpdateLanguage[1]) {

                SetWindowText(HandleWinMain, vLanguagepack[2].c_str());
                SetWindowText(HandleButtonEditor, vLanguagepack[4].c_str());
                SetWindowText(HandleButtonSettings, vLanguagepack[1].c_str());
                SetWindowText(HandleButtonEnd, vLanguagepack[5].c_str());

                UpdateWindow(HandleWinMain);
                // deactivate Update Flag
                vUpdateLanguage[1] = false;

            }

            if (MessageMainWindow.message == WM_QUIT) {

                // Close Programm
                running = false;

            }
            else if (MessageMainWindow.message == WM_COMMAND) {

                switch (LOWORD(MessageMainWindow.wParam)) {

                case ID_BUTTON_START_EDITOR:

                    // Editor Menu
                    // Hand Over Current Window Informations
                    FileeditorMenu(hInstance, HandleWinMain);
                    break;

                case ID_BUTTON_START_SETTINGS:

                    // Open Settings
                    // Hand Over Current Window Informations
                    SettingsMenu(hInstance, HandleWinMain);
                    break;

                default:
                    break;

                };
                break;

            };


            TranslateMessage(&MessageMainWindow);
            DispatchMessage(&MessageMainWindow);

        };

    };

    return 0;
};


// Prorgamm Startup Function - Call some Functions needed bevor the Main-Window is created, in Sub-Function for better Strukture
int Programm_Startup() {


    // Error Code to Handle Faults
    int vErrorCode = 0;

    // #######################
    // # Set Global Settings #
    // #######################


    // Inital Load of the Settings from the .conf File
    vErrorCode = LoadSettings();
    switch (vErrorCode) {

    case 100:

        // File Not Found
        MessageBox(NULL, "Invalid Settings, default values got loaded", "Error", MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);


    case 0:

        // Settings Loaded
        break;

    };


    // Load Last Opend Files
    // No Error Code needed, if no Filepaths are detected, just let them empty
    LoadLastFiles();


    // Load Language Settings
    // Inital Load of the Language Pack
    vErrorCode = LoadLanguage(vSettings.Language);
    switch (vErrorCode) {

    case 100:

        // File Not Found
        MessageBox(NULL, "File not found", "Error", MB_ICONERROR | MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
        return 100;

    case 200:

        // No Valid Language
        MessageBox(NULL, "No Valid Language", "Error", MB_ICONERROR | MB_OK | MB_APPLMODAL | MB_SETFOREGROUND | MB_TOPMOST);
        return 200;

    case 0:

        // Language loaded sucessfully
        break;

    };

    return 0;

};