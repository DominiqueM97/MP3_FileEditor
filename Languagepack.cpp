#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include "User_Types.h"

// Activate Namespace
using namespace std;

// Global Variables
extern string vLanguagepack[];
extern bool vUpdateLanguage[];




int LoadLanguage(int vCurrentLanguage) {

	// Error Code to Handle Faults
	int vErrorCode = 0;


	// 0 to also load the Header
	int vCurrentLine = 0;

	// Create needed variables
	ifstream vLangagepackFile;

	// Check if the selected Language is valid
	if (vCurrentLanguage < 1 or vCurrentLanguage > vMaxLanguages) {
		return 200;
	}


	// Open the .csv file
	vLangagepackFile.open("Languagepack.csv");

	// Make sure the file was found
	if (!vLangagepackFile.is_open()) {
		return 100;
	}
	else {

		// File is found, then load the lines
		// Stop when the max of Lines or the end of the File is reached
		while (!vLangagepackFile.eof() and vCurrentLine <= vMaxTextLines) {

			// Create the needed Variables
			string vActualLine;
			string vActualText;

			// Load the current Line from the .csv
			getline(vLangagepackFile, vActualLine);

			// Store the current Line to split it into the available Languages
			istringstream vActualRow(vActualLine);

			// Create a Temp-Pointer
			// Split the String and only Load the Text in the selected Language
			int vTempPointer = 0;
			while (getline(vActualRow, vActualText, ';')) {
				if (vTempPointer == vCurrentLanguage) {

					// Store the loaded Text to the Array
					vLanguagepack[vCurrentLine] = vActualText;
				}
				++vTempPointer;
			};

			// Count up the Pointer
			++vCurrentLine;

		};
	};

	// Close the File
	vLangagepackFile.close();
	// Set the Update FLag for the other Windows
	vUpdateLanguage[0] = true;
	vUpdateLanguage[1] = true;
	vUpdateLanguage[2] = true;
	vUpdateLanguage[3] = true;
	vUpdateLanguage[4] = true;
	vUpdateLanguage[5] = true;
	vUpdateLanguage[6] = true;
	vUpdateLanguage[7] = true;
	vUpdateLanguage[8] = true;
	vUpdateLanguage[9] = true;


	// Sucessfull load of Languagepack
	return 0;

};

