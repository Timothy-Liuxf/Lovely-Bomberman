#define _CRT_SECURE_NO_WARNINGS

#ifndef SOUND_H

#define SOUND_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif	//#ifndef WIN32_LEAN_AND_MEAN

#include "str.h"
#include <windows.h>
#include <tchar.h>
#include <string>

#ifdef _WIN64
#define GAME_SOUND_EXE_NAME TEXT("\"GameSound(x64).exe\"")
#else	//ifdef _WIN64
#define GAME_SOUND_EXE_NAME TEXT("\"GameSound.exe\"")
#endif	//ifdef _WIN64

class SoundPlay
{
public:

	bool NullSound() const { return AllSound(0); }
	void BombSound() const { AllSound(1); }
	void PickPropSound() const { AllSound(2); }
	void FailSound() const { AllSound(4); }
	void SuccessSound() const { AllSound(3); }

private:

	bool AllSound(int i) const;
};

#endif	//#ifndef SOUND_H
