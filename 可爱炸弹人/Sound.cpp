#include "Sound.h"

bool SoundPlay::AllSound(int i) const
{
	const TCHAR* num[10] = { TEXT("0"), TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5"), TEXT("6"), TEXT("7"),TEXT("8"), TEXT("9") };
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);

	TCHAR cmd[128];
	_stprintf(cmd, TEXT("%s %s"), GAME_SOUND_EXE_NAME, num[i]);

	if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		//创建进程失败
		return false;
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return true;
}
