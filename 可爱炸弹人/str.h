////////////////////////////////////
//
//  程序需要用到的字符串
//

#ifndef STR_H

#define STR_H

#include "framework.h"


//窗口定义

static LPCTSTR c_lpszWndClassName = TEXT("LovelyBombermanMainWindow");	//窗口类名
static LPCTSTR c_lpszWndTitle = TEXT("可爱炸弹人");						//主窗口标题

//警告
static LPCTSTR c_lpszWarning = TEXT("这样做可能会产生致命错误，是否继续？\nThis may cause fatal errors, continue?");
static LPCTSTR c_lpszWarningTitle = TEXT("warning");

//关于
static LPCTSTR c_lpszAbout = TEXT("名称：可爱炸弹人\n\n版本：v1.0.0.0\n\nCopyRight (C) 2020\n\n计算机程序设计基础(2) All Rights Reverved."); 

//图片路径
#define BKGND_PATH TEXT("image\\bkgnd.bmp")
#define MAIN_PATH TEXT("image\\main.bmp")
#define ROLE_PATH TEXT("image\\role.bmp")
#define TNT_PATH TEXT("image\\tnt.bmp")
#define OBSTACLE_PATH TEXT("image\\obstacle.bmp")
#define GLOVE_PATH TEXT("image\\glove.bmp")
#define SHIELD_PATH TEXT("image\\shield.bmp")
#define ADDTNT_PATH TEXT("image\\addtnt.bmp")
#define ADDLIFE_PATH TEXT("image\\addLife.bmp")
#define SHOE_PATH TEXT("image\\shoe.bmp")
#define JINKELA_PATH TEXT("image\\jinkela.bmp")
#define LACHRYMATOR_PATH TEXT("image\\lachrymator.bmp")
#define MINE_PATH TEXT("image\\mine.bmp")
#define FIRE_PATH TEXT("image\\fire.bmp")
#define ICE_PATH TEXT("image\\ice.bmp")
#define GRENADE_PATH TEXT("image\\grenade.bmp")
#define MISSILE_PATH TEXT("image\\missile.bmp")

#define IMAGE_LOAD_FAIL_STR(path) TEXT("找不到图片：") path TEXT("\nCannot find image: ") path
#define IMAGE_LOAD_FAIL_RETRY TEXT("图片加载失败！\nFailing to load images!")

#endif	// #ifndef STR_H
