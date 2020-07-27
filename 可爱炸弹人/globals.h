//////////////////////////////
//
//  存放全局变量
//

#ifndef GLOBALS_H

#define GLOBALS_H

#include "framework.h"

//定时器ID

#define TIMER_ID_START 9999					//开始刷新窗口计时器

//关于窗口

extern HINSTANCE hInst;						//当前实例
extern HWND hMainWnd;						//主窗口句柄
extern CONST LONG objSize;					//每格的边长
extern CONST POINT mainWndPos;				//主窗口左上角坐标
extern CONST POINT mainWndSize;				//主窗口大小
extern LONG capMenuAppendCy;				//标题栏和菜单栏占用的高度

//位图

extern HBITMAP hBmMem;						//用于缓冲
extern HBITMAP hBmBkgnd;					//背景位图句柄
extern HBITMAP hBmRole;						//角色位图句柄

extern BITMAP bmBkgnd;						//背景位图
extern BITMAP bmRole;						//角色位图

#endif	// #ifndef GLOBALS_H
