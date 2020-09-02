#include "Sound.h"

void AllSound(int i)
{
	const TCHAR* num[10] = { TEXT("0"), TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5"), TEXT("6"), TEXT("7"),TEXT("8"), TEXT("9") };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);

#ifdef _WIN64
	TCHAR s[50] = TEXT("\"GameSound(x64).exe\" ");
#else	//ifdef _WIN64
	TCHAR s[50] = TEXT("\"GameSound.exe\" ");
#endif	//ifdef _WIN64
	
	_tcscat_s(s, 50, num[i]); 
	CreateProcess(NULL, s, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void BombSound()
{
	AllSound(1); 
}
void PickPropSound()
{
	AllSound(2); 
}
void SuccessSound()
{
	AllSound(3); 
}
void FailSound()
{
	AllSound(4); 
}

