#pragma once

#ifndef IMAGES_RESOURCE_H

#define IMAGES_RESOURCE_H

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <tchar.h>

#ifdef _WIN64
#define IMG_DLL_NAME TEXT("Images(x64).dll")
#else
#define IMG_DLL_NAME TEXT("Images.dll")
#endif

#endif
