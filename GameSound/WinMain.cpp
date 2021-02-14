#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#include "GameSoundResource.h"
#pragma comment(lib, "winmm.lib")

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    TCHAR* cmdPtr = lpCmdLine;

    //lpCmdLine: Excluding the program name!!!!!!!!!!

    /*while (*cmdPtr != TEXT(' ') && *cmdPtr != TEXT('\0')) ++cmdPtr;
    if (*cmdPtr == TEXT('\0')) return 0;*/

    while (*cmdPtr == TEXT(' ')) ++cmdPtr;

    INT soundID = 0;

    switch (*cmdPtr)
    {
    case TEXT('1'):
        soundID = IDS_BOMB;
        break;
    case TEXT('2'):
        soundID = IDS_PICK_PROP;
        break;
    case TEXT('3'):
        soundID = IDS_SUCCESS;
        break;
    case TEXT('4'):
        soundID = IDS_FAIL;
        break;
    default:
        return 0;
    }


    PlaySound(MAKEINTRESOURCE(soundID), GetModuleHandle(NULL), SND_SYNC | SND_RESOURCE);     //ERROR: 1812

    return 0; 
}
