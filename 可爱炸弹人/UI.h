//////////////////////////////
//
//  存放全局变量
//

#ifndef UI_H

#define UI_H

#include "framework.h"
#include "str.h"

//定时器ID

#define TIMER_ID_START 9999					//开始刷新窗口计时器

//关于窗口

extern HINSTANCE hInst;						//当前实例
extern HWND hMainWnd;						//主窗口句柄
extern CONST LONG objSize;					//每格的边长
extern CONST LONG propSize;					//道具的边长
extern CONST POINT mainWndPos;				//主窗口左上角坐标
extern CONST POINT mainWndSize;				//主窗口大小
extern LONG capMenuAppendCy;				//标题栏和菜单栏占用的高度

class UI
{
public: 

	bool LoadGameImg();						//加载位图
	void CreateBuffer(HWND hWnd);			//创建缓冲位图

	//开始画图
	void Paint(HWND hWnd, BOOL calledByPaintMessage); 	

	~UI(); 

private: 

	HBITMAP hBmMem = NULL;					//用于缓冲
	HBITMAP hBmBkgnd = NULL;				//背景位图句柄
	HBITMAP hBmRole = NULL;					//角色位图句柄
	HBITMAP hBmTnt = NULL;					//炸弹位图句柄

	BITMAP bmBkgnd;						//背景位图
	BITMAP bmRole;						//角色位图
	BITMAP bmTnt;						//炸弹位图
};

extern UI mainGameUI; 

#endif	// #ifndef GLOBALS_H
