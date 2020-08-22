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
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <type_traits>

#ifdef _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#ifdef _UNICODE
#define _tstring wstring
#define _tistringstream wistringstream
#define _tostringstream wostringstream
#define _tifstream wifstream
#define _tofstream wofstream
#else					//#ifdef _UNICODE
#define _tstring string
#define _tistringstream istringstream
#define _tostringstream ostringstream
#define _tifstream ifstream
#define _tofstream ofstream
#endif					//#ifdef _UNICODE

#endif // #ifndef FRAMEWORK_H

