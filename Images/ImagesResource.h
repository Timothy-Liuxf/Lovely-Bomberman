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

//Í¼Æ¬Â·¾¶

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

#define IDB_BKGND		1000
#define IDB_MAIN		1001
#define IDB_ROLE		1002
#define IDB_TNT			1003
#define IDB_OBSTACLE	1004
#define IDB_GLOVE		1005
#define IDB_SHIELD		1006
#define IDB_ADDTNT		1007
#define IDB_ADDLIFE		1008
#define IDB_SHOE		1009
#define IDB_JINKELA		1010
#define IDB_LACHRYMATOR	1011
#define IDB_MINE		1012
#define IDB_FIRE		1013
#define IDB_ICE			1014
#define IDB_GRENADE		1015
#define IDB_MISSILE		1016

#endif
