#include "globals.h"

HINSTANCE hInst; 
HWND hMainWnd; 
CONST LONG objSize = 40; 
CONST POINT mainWndPos = { 0, 0 }; 
CONST POINT mainWndSize = { (objSize * 15) + 200, objSize * 13 }; 
LONG capMenuAppendCy; 

HBITMAP hBmMem; 
HBITMAP hBmBkgnd; 
HBITMAP hBmRole; 

BITMAP bmBkgnd; 
BITMAP bmRole; 
