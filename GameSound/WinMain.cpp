#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#pragma comment(lib, "winmm.lib")

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPTSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    int sz = _tcslen(lpCmdLine);
    switch (lpCmdLine[sz - 1])
    {
    case TEXT('1'):
        PlaySound(TEXT("sound\\bomb.wav"), NULL, SND_SYNC);
        break;
    case TEXT('2'):
        PlaySound(TEXT("sound\\pickProp.wav"), NULL, SND_SYNC);
        break;
    case TEXT('3'):
        PlaySound(TEXT("sound\\success.wav"), NULL, SND_SYNC);
        break;
    case TEXT('4'):
        PlaySound(TEXT("sound\\fail.wav"), NULL, SND_SYNC);
        break;
    }
    return 0; 
}
