#include "UI.h"

int UI::Begin(HINSTANCE hInstance, int nCmdShow)
{
    //定义窗口样式
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = c_lpszWndClassName;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    capMenuAppendCy = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYCAPTION); 

    Init(hInstance, nCmdShow, 0, 0, mainWndSize.x, mainWndSize.y + capMenuAppendCy, 
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX, c_lpszWndTitle, wcex);

    MSG msg;

    //加载位图

    LoadGameImg();

    // 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam; 
}

//消息处理函数
bool UI::messageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        CreateBuffer(hWnd);
        SetTimer(hWnd, TIMER_ID_START, 500, NULL);
        SetTimer(hWnd, 500, 20, NULL);
        break; 
    case WM_PAINT:
    {
        Paint(hWnd, TRUE);
    }
    break;
    case WM_LBUTTONUP:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_TIMER:
        if (wParam == TIMER_ID_START)
        {
            InvalidateRect(hWnd, NULL, TRUE);
            KillTimer(hWnd, TIMER_ID_START);
        }
        else
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break; 
    case WM_DESTROY: 
        if (hBmMem)
        {
            DeleteObject(hBmMem); 
            hBmMem = NULL; 
        }
        KillTimer(hWnd, 500); 
        PostQuitMessage(0); 
        break; 
    default: 
        return false; 
    }
    return true; 
}

bool UI::LoadGameImg()
{
    hBmBkgnd = (HBITMAP)LoadImage(m_hInst, TEXT("image\\bkgnd.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmRole = (HBITMAP)LoadImage(m_hInst, TEXT("image\\role.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmTnt = (HBITMAP)LoadImage(m_hInst, TEXT("image\\tnt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmObstacle = (HBITMAP)LoadImage(m_hInst, TEXT("image\\obstacle.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmGlove = (HBITMAP)LoadImage(m_hInst, TEXT("image\\glove.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShield = (HBITMAP)LoadImage(m_hInst, TEXT("image\\shield.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddTnt = (HBITMAP)LoadImage(m_hInst, TEXT("image\\addtnt.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddLife = (HBITMAP)LoadImage(m_hInst, TEXT("image\\addLife.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShoe = (HBITMAP)LoadImage(m_hInst, TEXT("image\\shoe.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmJinKeLa = (HBITMAP)LoadImage(m_hInst, TEXT("image\\jinkela.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLachrymator = (HBITMAP)LoadImage(m_hInst, TEXT("image\\lachrymator.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMine = (HBITMAP)LoadImage(m_hInst, TEXT("image\\mine.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFire = (HBITMAP)LoadImage(m_hInst, TEXT("image\\fire.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmIce = (HBITMAP)LoadImage(m_hInst, TEXT("image\\ice.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmGrenade = (HBITMAP)LoadImage(m_hInst, TEXT("image\\grenade.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMissil = (HBITMAP)LoadImage(m_hInst, TEXT("image\\missil.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmBkgnd || !hBmRole || !hBmTnt || !hBmObstacle || !hBmGlove || !hBmShield | !hBmAddTnt || !hBmAddLife || !hBmShoe 
        || !hBmJinKeLa || !hBmLachrymator || !hBmMine || !hBmFire || !hBmIce || !hBmGrenade || !hBmMissil)
    {
        MessageBox(m_hWnd, TEXT("加载图片失败！"), c_lpszError, MB_OK | MB_ICONERROR);
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
