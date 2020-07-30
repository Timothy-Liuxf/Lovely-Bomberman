
#include "MsgProc.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        mainGameUI.CreateBuffer(hWnd); 
        SetTimer(hWnd, TIMER_ID_START, 500, NULL); 
    break; 
    case WM_PAINT:
    {
        mainGameUI.Paint(hWnd, TRUE); 
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
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
