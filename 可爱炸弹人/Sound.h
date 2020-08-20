#ifndef SOUND_H

#define SOUND_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif	//#ifndef WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>

void AllSound(int i); 
void BombSound(); 
void PickPropSound(); 
void FailSound(); 
void SuccessSound(); 

#endif	//#ifndef SOUND_H
