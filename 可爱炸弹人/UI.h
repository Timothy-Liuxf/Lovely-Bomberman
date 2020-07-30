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

	HBITMAP hBmMem = NULL;				//用于缓冲
	HBITMAP hBmBkgnd = NULL;			//背景位图句柄
	HBITMAP hBmRole = NULL;				//角色位图句柄
	HBITMAP hBmTnt = NULL;				//炸弹位图句柄
	HBITMAP hBmObstacle = NULL;			//障碍物位图句柄
	HBITMAP hBmGlove = NULL;			//手套位图句柄
	HBITMAP hBmShield = NULL;			//盾牌位图句柄
	HBITMAP hBmAddTnt = NULL;			//加炮弹位图句柄
	HBITMAP hBmAddLife = NULL;			//加生命位图句柄
	HBITMAP hBmShoe = NULL;				//跑鞋位图句柄
	HBITMAP hBmJinKeLa = NULL;			//金坷垃位图句柄
	HBITMAP hBmLachrymator = NULL;		//催泪瓦斯位图句柄
	HBITMAP hBmMine = NULL;				//地雷位图句柄
	HBITMAP hBmFire = NULL;				//火焰枪位图句柄
	HBITMAP hBmIce = NULL;				//冰枪位图句柄
	HBITMAP hBmGrenade = NULL;			//手榴弹位图句柄
	HBITMAP hBmMissil = NULL;			//导弹位图句柄

	BITMAP bmBkgnd;						//背景位图信息
	BITMAP bmRole;						//角色位图信息
	BITMAP bmTnt;						//炸弹位图信息
	BITMAP bmObstacle;					//障碍物位图信息
	BITMAP bmGlove;						//手套位图信息
	BITMAP bmShield;					//盾牌位图信息
	BITMAP bmAddTnt;					//加炮弹位图信息
	BITMAP bmAddLife;					//加生命位图信息
	BITMAP bmShoe;						//跑鞋位图信息
	BITMAP bmJinKeLa;					//金坷垃位图信息
	BITMAP bmLachrymator;				//催泪瓦斯位图信息
	BITMAP bmMine;						//地雷位图信息
	BITMAP bmFire;						//火焰枪位图句柄信息
	BITMAP bmIce;						//冰枪位图信息
	BITMAP bmGrenade;					//手榴弹位图信息
	BITMAP bmMissil;					//导弹位图信息
};

extern UI mainGameUI; 

#endif	// #ifndef GLOBALS_H
