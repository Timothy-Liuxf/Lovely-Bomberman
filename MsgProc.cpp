
#include "MsgProc.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
    {
        HDC hdc = GetDC(hWnd); 
        hBmMem = CreateCompatibleBitmap(hdc, mainWndSize.x, mainWndSize.y);
        SetTimer(hWnd, TIMER_ID_START, 500, NULL); 
        ReleaseDC(hWnd, hdc); 
    }
    break; 
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps); 

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

        SelectObject(hdcObj, hBmBkgndOld); 
        DeleteDC(hdcObj); 

        BitBlt(hdc, 0, 0, mainWndSize.x, mainWndSize.y, hdcMem, 0, 0, SRCCOPY); 
        SelectObject(hdcMem, hBmMemOld); 
        DeleteDC(hdcMem); 

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_TIMER: 
        if (wParam == TIMER_ID_START)
        {
            InvalidateRect(hWnd, NULL, TRUE); 
            KillTimer(hWnd, TIMER_ID_START); 
        }
        break; 
    case WM_DESTROY:
        DeleteObject(hBmMem); 
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
