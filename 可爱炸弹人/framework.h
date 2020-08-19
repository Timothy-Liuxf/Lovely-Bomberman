////////////////////////////////////////
//
//  windows窗体程序需要的头文件和宏
//


#ifndef FRAMEWORK_H

#define FRAMEWORK_H

// // 包含 SDKDDKVer.h 可定义可用的最高版本的 Windows 平台。
// 如果希望为之前的 Windows 平台构建应用程序，在包含 SDKDDKVer.h 之前请先包含 WinSDKVer.h 并
// 将 _WIN32_WINNT 宏设置为想要支持的平台。
#include <SDKDDKVer.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#endif	// #ifndef WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <tchar.h>

#include <cstdlib>
#include <string>
#include <sstream>

#ifdef _UNICODE
#define _tstring wstring
#define _tistringstream wistringstream
#define _tostringstream wostringstream
#else					//#ifdef _UNICODE
#define _tstring string
#define _tistringstream istringstream
#define _tostringstream ostringstream
#endif					//#ifdef _UNICODE

#endif // #ifndef FRAMEWORK_H

