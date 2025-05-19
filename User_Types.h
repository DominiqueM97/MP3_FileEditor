// User Defined Structs

#ifndef SETTINGSSTRUCT
#define SETTINGSSTRUCT
#include <string>
// Global Struct for Settings
struct Settings {

    int Language = 2;
    int TextColor = 15;
    int TextHighlightColor = 10;
    int DefaultTyp = 1;
    std::string LastFiles[10] = { "- - -", "- - -", "- - -", "- - -", "- - -", "- - -", "- - -", "- - -", "- - -", "- - -" };

};
#endif


#ifndef MP3FRAMEINFOSTRUCT
#define MP3FRAMEINFOSTRUCT
// Struct for MP3 Frame Information
struct MP3Framinfo {

    byte MPEG_VERSION;
    byte LAYER;
    bool CRC_PROTECTION;
    int BITRATE;            // in Kbps
    int SAMPLINGRATE;       // in Hz
    bool PADDING;
    byte CHANNEL;
    byte MODE_EXTENTION;
    bool COPYRIGHT;
    bool ORIGINAL;
    byte EMPHASIS;
    int FRAMELENGHT;

};
#endif


#ifndef MP3BITRATESTRUCT
#define MP3BITRATESTRUCT
// Struct for MP3 Bitrates
// Bitrate in kbps / V1 & L1 / V1 & L2 / V1 & L3 / V2 & L1 / V2 & L2 or L3
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
struct MP3Bitrates {

    int Rate_0[5] = { 0 , 0 , 0 , 0 , 0 };
    int Rate_1[5] = { 32 , 32 , 32 , 32 , 8 };
    int Rate_2[5] = { 64 , 48 , 40 , 48 , 16 };
    int Rate_3[5] = { 96 , 56 , 48 , 56 , 24 };
    int Rate_4[5] = { 128 , 64 , 56 , 64 , 32 };
    int Rate_5[5] = { 160 , 80 , 64 , 80 , 40 };
    int Rate_6[5] = { 192 , 96 , 80 , 96 , 48 };
    int Rate_7[5] = { 224 , 112 , 96 , 112 , 56 };
    int Rate_8[5] = { 256 , 128 , 112 , 128 , 64 };
    int Rate_9[5] = { 288 , 160 , 128 , 144 , 80 };
    int Rate_10[5] = { 320 , 192 , 160 , 160 , 96 };
    int Rate_11[5] = { 352 , 224 , 192 , 176 , 112 };
    int Rate_12[5] = { 384 , 256 , 224 , 192 , 128 };
    int Rate_13[5] = { 416 , 320 , 256 , 224 , 144 };
    int Rate_14[5] = { 448 , 384 , 320 , 256 , 160 };
    int Rate_15[5] = { 0 , 0 , 0 , 0 , 0 };

};
#endif


#ifndef MP3FRAMELENGHTSTRUCT
#define MP3FRAMELENGHTSTRUCT
// Struct to Save
struct MP3Framelenghts {

    INT64 FrameStartByte = 0;
    INT64 FrameNumber = 0;
    INT64 FrameStartRow = 0;
    int FrameByteLenght = 0;

};
#endif




// Global Statics

#ifndef vMaxLanguages
#define vMaxLanguages 2
#endif

#ifndef vMaxTextLines
#define vMaxTextLines 150
#endif

#ifndef vFilePathLenght
#define vFilePathLenght 1024
#endif



// User ID's for Elements

#ifndef ID_BUTTON_START_EDITOR
#define ID_BUTTON_START_EDITOR 100
#endif

#ifndef ID_BUTTON_START_SETTINGS
#define ID_BUTTON_START_SETTINGS 101
#endif

#ifndef ID_BUTTON_CLOSE
#define ID_BUTTON_CLOSE 102
#endif

#ifndef ID_BUTTON_SETTINGS_SET_LANGUAGE
#define ID_BUTTON_SETTINGS_SET_LANGUAGE 150
#endif

#ifndef ID_BUTTON_SETTINGS_SET_TEXTCOLOR
#define ID_BUTTON_SETTINGS_SET_TEXTCOLOR 151
#endif

#ifndef ID_BUTTON_SETTINGS_SAVE
#define ID_BUTTON_SETTINGS_SAVE 154
#endif

#ifndef ID_BUTTON_SETTINGS_CLOSE
#define ID_BUTTON_SETTINGS_CLOSE 155
#endif

#ifndef ID_DROPTOWN_SETTINGS_LANGUAGE
#define ID_DROPTOWN_SETTINGS_LANGUAGE 156
#endif

#ifndef ID_BUTTON_SETTINGS_SET_DEFAULTTYPE
#define ID_BUTTON_SETTINGS_SET_DEFAULTTYPE 157
#endif

#ifndef ID_BUTTON_EDITOR_CLOSE
#define ID_BUTTON_EDITOR_CLOSE 200
#endif

#ifndef ID_BUTTON_EDITOR_OPEN_NEW_FILE
#define ID_BUTTON_EDITOR_OPEN_NEW_FILE 201
#endif

#ifndef ID_BUTTON_EDITOR_OPEN_LAST_FILE
#define ID_BUTTON_EDITOR_OPEN_LAST_FILE 202
#endif

#ifndef ID_LISTVIEW
#define ID_LISTVIEW 250
#endif

#ifndef ID_LISTVIEW_EDITOR
#define ID_LISTVIEW_EDITOR 251
#endif

#ifndef ID_BUTTON_EDITOR_SAVE
#define ID_BUTTON_EDITOR_SAVE 252
#endif

#ifndef ID_BUTTON_EDITOR_SAVE_NEW
#define ID_BUTTON_EDITOR_SAVE_NEW 253
#endif

#ifndef ID_EDITOR_CHANGE
#define ID_EDITOR_CHANGE 300
#endif