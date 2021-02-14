////////////////////////////////////
//
//  str.h : 程序需要用到的字符串
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

//帮助
static LPCTSTR c_lpszHelp = TEXT("本游戏有两种模式：单人模式和双人模式；三种难度：简单、一般和困难。\n\
1.玩家操作：1P：WASD行走，空格放置炸弹；2P：上下左右行走，回车放置炸弹\n\
2.地图：地图包括硬障碍和软障碍，硬障碍不能够被炸毁，而软障碍可以被炸毁\n\
3.道具：炸毁软障碍时有几率并获得道具，不同的道具有不同的效果\n\
4.分数：本游戏有5关，每一关会得到一定的分数\n\
5.生命值：角色的生命值是玩家可以复活的次数，当生命值用完之后便无法复活，游戏结束\n\
6.关于难度：\n\
  简单难度，玩家初始带有护盾，电脑不能捡起道具；\n\
  中等难度，玩家初始不带有护盾，电脑不能捡起道具；\n\
  困难难度：玩家初始不带有护盾，电脑也可以捡道具\n\
7.提示：一个角色放置的炸弹可以同时在地图上存在的数量是有限的，当达到上限时便不能放置炸弹，需要等待以前放置的炸弹爆炸后才能继续放置"); 
static LPCTSTR c_lpszHelpProp = TEXT("道具系统：\n\
1.手套：使人物获得推动炸弹的能力\n\
2.盾牌：能够抵挡一次攻击\n\
3.增加炸弹数：可以增加在地图上放置炸弹的数目\n\
4.增加生命：生命值+1\n\
5.跑鞋：增加移动速度\n\
6.金坷垃：增加炸弹爆炸范围\n\
7.催泪瓦斯：放置后一段时间之内可见，之后变为不可见，会使踩到上面的角色减速，但会被炸弹炸毁，并且变为不可见之后一段时间也会消失\n\
8.地雷：放置后一段时间之后变为不可见，会使才到上面的角色受到攻击，但会被炸弹炸毁，并且变为不可见之后一段时间也会消失\n\
9.火焰枪：使用后会立即攻击面前三格的敌人\n\
10.手榴弹：能够越过障碍攻击敌人\n\
11.导弹：攻击沿线的敌人"); 
static LPCTSTR c_lpszHelpScore = TEXT("分数系统：游戏中特定事件会使得角色获得分数\n\
1.每炸毁一个软障碍得1分\n\
2.每捡到一个道具得2分\n\
3.每攻击到一个角色（不必杀死）得20分\n\
值得注意的是，如果玩家在这一关中获胜，则会获得这一关得到的分数；如果玩家失败，则不会获得这一关得到的分数"); 
static LPCTSTR c_lpszHelpMusic = TEXT("打开声音游戏效果更佳"); 

//高分存档路径
#define HIGH_SCORE_DIRECTORY TEXT("data")
#define HIGH_SCORE_PATH HIGH_SCORE_DIRECTORY TEXT("\\HighScore.sav")

#define IMAGE_LOAD_FAIL_STR(path) TEXT("找不到图片：") path TEXT("\nCannot find image: ") path
#define IMAGE_LOAD_FAIL_RETRY TEXT("图片加载失败！\nFailing to load images!")

#ifdef _WIN64
#define SOUND_EXE_PATH TEXT("GameSound(x64).exe")
#else	//ifdef _WIN64
#define SOUND_EXE_PATH TEXT("GameSound.exe")
#endif	//ifdef _WIN64
#define SOUND_EXE_NOT_FOUND TEXT("找不到音效播放程序：") SOUND_EXE_PATH TEXT("\nCannot find the file for playing sound: ") SOUND_EXE_PATH

#define MUSIC_LOAD_FAIL_STR(path) TEXT("找不到声音：") path TEXT("\nCannot find sound: ") path

#endif	// #ifndef STR_H
