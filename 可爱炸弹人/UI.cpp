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
    hBmObstacle = (HBITMAP)LoadImage(hInst, TEXT("image\\obstacle.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmGlove = (HBITMAP)LoadImage(hInst, TEXT("image\\glove.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShield = (HBITMAP)LoadImage(hInst, TEXT("image\\shield.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddTnt = (HBITMAP)LoadImage(hInst, TEXT("image\\addtnt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddLife = (HBITMAP)LoadImage(hInst, TEXT("image\\addLife.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShoe = (HBITMAP)LoadImage(hInst, TEXT("image\\shoe.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmJinKeLa = (HBITMAP)LoadImage(hInst, TEXT("image\\jinkela.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLachrymator = (HBITMAP)LoadImage(hInst, TEXT("image\\lachrymator.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMine = (HBITMAP)LoadImage(hInst, TEXT("image\\mine.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFire = (HBITMAP)LoadImage(hInst, TEXT("image\\fire.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmIce = (HBITMAP)LoadImage(hInst, TEXT("image\\ice.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmGrenade = (HBITMAP)LoadImage(hInst, TEXT("image\\grenade.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMissil = (HBITMAP)LoadImage(hInst, TEXT("image\\missil.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmBkgnd || !hBmRole || !hBmTnt || !hBmObstacle || !hBmGlove || !hBmShield | !hBmAddTnt || !hBmAddLife || !hBmShoe 
        || !hBmJinKeLa || !hBmLachrymator || !hBmMine || !hBmFire || !hBmIce || !hBmGrenade || !hBmMissil)
    {
        MessageBox(hMainWnd, TEXT("º”‘ÿÕº∆¨ ß∞‹£°"), c_lpszError, MB_OK | MB_ICONERROR);
        return false; 
    }

    GetObject(hBmBkgnd, sizeof(BITMAP), &bmBkgnd); 
    GetObject(hBmRole, sizeof(BITMAP), &bmRole); 
    GetObject(hBmTnt, sizeof(BITMAP), &bmTnt); 
    GetObject(hBmObstacle, sizeof(BITMAP), &bmObstacle); 
    GetObject(hBmGlove, sizeof(BITMAP), &bmGlove);
    GetObject(hBmShield, sizeof(BITMAP), &bmShield);
    GetObject(hBmAddTnt, sizeof(BITMAP), &bmAddTnt);
    GetObject(hBmAddLife, sizeof(BITMAP), &bmAddLife);
    GetObject(hBmShoe, sizeof(BITMAP), &bmShoe);
    GetObject(hBmJinKeLa, sizeof(BITMAP), &bmJinKeLa);
    GetObject(hBmLachrymator, sizeof(BITMAP), &bmLachrymator);
    GetObject(hBmMine, sizeof(BITMAP), &bmMine);
    GetObject(hBmFire, sizeof(BITMAP), &bmFire);
    GetObject(hBmIce, sizeof(BITMAP), &bmIce);
    GetObject(hBmGrenade, sizeof(BITMAP), &bmGrenade);
    GetObject(hBmMissil, sizeof(BITMAP), &bmMissil);
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
            //BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * roleNum, 0, SRCCOPY);
        }
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 15; ++j)
        {
            if (i != j && i != 13 - j) continue;
            if (i % 2)
            {
                SelectObject(hdcObj, hBmTnt); 
                /*BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, 0, 0, SRCCOPY); */
            }
            else
            {
                SelectObject(hdcObj, hBmTnt);
                /*BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * 1, 0, SRCCOPY);*/
            }
        }
    /*for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 15; ++j)
        {
            if (i % 3) continue;
            SelectObject(hdcObj, hBmObstacle);
            if (j % 3 == 0)
                BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, 0, 0, SRCCOPY); 
            else if (j % 3 == 1) BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize, 0, SRCCOPY);
            else
            {
                SelectObject(hdcObj, hBmLachrymator); 
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * 1, 0, SRCCOPY); 
            }
        }*/
    for (int i = 0; i < 13; ++i)
        for (int j = 0; j < 15; ++j)
        {
            int tmp = rand() % 16; 
            switch (tmp)
            {
            case 0: 
                SelectObject(hdcObj, hBmRole);
                BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * (rand() % 4), 0, SRCCOPY);
                break; 
            case 1: 
                SelectObject(hdcObj, hBmObstacle);
                BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * (rand() % 2), 0, SRCCOPY);
                break; 
            case 2:
                SelectObject(hdcObj, hBmTnt);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
                break; 
            case 3: 
                SelectObject(hdcObj, hBmGlove);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
                break; 
            case 4:
                SelectObject(hdcObj, hBmShield);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
                break; 
            case 5: 
                SelectObject(hdcObj, hBmAddTnt);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
                break; 
            case 6:
                SelectObject(hdcObj, hBmAddLife);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
                break;
            case 7:
                SelectObject(hdcObj, hBmShoe);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
                break;
            case 8:
                SelectObject(hdcObj, hBmJinKeLa);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
                break;
            case 9:
                SelectObject(hdcObj, hBmLachrymator);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
                break;
            case 10:
                SelectObject(hdcObj, hBmMine);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
                break;
            case 11:
                SelectObject(hdcObj, hBmFire);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
                break;
            case 12:
                SelectObject(hdcObj, hBmIce);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
                break;
            case 13:
                SelectObject(hdcObj, hBmGrenade);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 3), 0, SRCCOPY);
                break;
            case 14:
                SelectObject(hdcObj, hBmMissil);
                BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
                break;
            case 15:
                break;

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
