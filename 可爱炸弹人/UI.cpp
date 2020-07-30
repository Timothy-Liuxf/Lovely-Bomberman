#include "UI.h"

HINSTANCE hInst; 
HWND hMainWnd; 
CONST LONG objSize = 40; 
CONST LONG propSize = 30;
CONST POINT mainWndPos = { 0, 0 }; 
CONST POINT mainWndSize = { (objSize * 15) + 200, objSize * 13 }; 
LONG capMenuAppendCy; 

bool UI::LoadGameImg()
{
    hBmBkgnd = (HBITMAP)LoadImage(hInst, TEXT("image\\bkgnd.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmRole = (HBITMAP)LoadImage(hInst, TEXT("image\\role.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmTnt = (HBITMAP)LoadImage(hInst, TEXT("image\\tnt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 

    if (!hBmBkgnd || !hBmRole || !hBmTnt)
    {
        MessageBox(hMainWnd, TEXT("º”‘ÿÕº∆¨ ß∞‹£°"), c_lpszError, MB_OK | MB_ICONERROR);
        return false; 
    }

    GetObject(hBmBkgnd, sizeof(BITMAP), &bmBkgnd);
    GetObject(hBmRole, sizeof(BITMAP), &bmRole);
    GetObject(hBmTnt, sizeof(BITMAP), &bmTnt); 
	return true; 
}

void UI::CreateBuffer(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    hBmMem = CreateCompatibleBitmap(hdc, mainWndSize.x, mainWndSize.y); 
    ReleaseDC(hWnd, hdc); 
}

void UI::Paint(HWND hWnd, const BOOL calledByPaintMessage)
{
    HDC hdc = NULL; 
    PAINTSTRUCT ps = { 0 }; 

    if (calledByPaintMessage)
        hdc = BeginPaint(hWnd, &ps);
    else hdc = GetDC(hWnd); 

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBmMemOld = (HBITMAP)SelectObject(hdcMem, hBmMem);

    HDC hdcObj = CreateCompatibleDC(hdc);
    HBITMAP hBmBkgndOld = (HBITMAP)SelectObject(hdcObj, hBmBkgnd);
    BitBlt(hdcMem, 0, 0, objSize * 15, objSize * 13, hdcObj, 0, 0, SRCCOPY);

    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 15; ++j)
        {
            if (i == j) continue;
            int roleNum = (i * 15 + j) % 4;
            SelectObject(hdcObj, hBmRole);
            BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * roleNum, 0, SRCCOPY);
        }
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 15; ++j)
        {
            if (i != j && i != 13 - j) continue;
            if (i % 2)
            {
                SelectObject(hdcObj, hBmTnt); 
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, 0, 0, SRCCOPY); 
            }
            else
            {
                SelectObject(hdcObj, hBmTnt);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * 1, 0, SRCCOPY);
            }
        }

    SelectObject(hdcObj, hBmBkgndOld);
    DeleteDC(hdcObj);

    BitBlt(hdc, 0, 0, mainWndSize.x, mainWndSize.y, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hBmMemOld);
    DeleteDC(hdcMem);

    if (calledByPaintMessage)
        EndPaint(hWnd, &ps);
    else ReleaseDC(hWnd, hdc); 
}

UI::~UI()
{
    if(hBmMem) DeleteObject(hBmMem); 
}

UI mainGameUI; 


