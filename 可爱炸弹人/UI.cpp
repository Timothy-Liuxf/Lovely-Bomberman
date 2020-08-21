#include "UI.h"

const int UI::objSize = 40;
const int UI::propSize = 30;
const POINT UI::mainWndPos = { 0, 0 };
const POINT UI::mainWndSize = { (objSize * 15) + 240, objSize * 13 }; 
const int UI::dataFps = 50; 
const int UI::paintFps = 50;

int UI::Begin(HINSTANCE hInstance, int nCmdShow)
{

    //加载位图
    if (!LoadGameImg()) return 0; 

    //定义窗口样式
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(MAINICON));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = MAKEINTRESOURCE(MAINMENU); 
    wcex.lpszClassName = c_lpszWndClassName;
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));
    
    capMenuAppendCy = GetSystemMetrics(SM_CYMENU) + GetSystemMetrics(SM_CYMIN);

    Init(hInstance, nCmdShow, 0, 0, mainWndSize.x, mainWndSize.y + capMenuAppendCy, 
        WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX, 
        c_lpszWndTitle, wcex, MAKEINTRESOURCE(MAINMENUACCELERATOR));

    MSG msg;

    programState = programstate::starting;
    PlayMainMusic(); 

    // 主消息循环:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(m_hWnd, m_hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam; 
}

//消息处理函数
bool UI::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: 
        CreateBuffer(hWnd);
        break; 
    case WM_PAINT:
    {
        Paint(hWnd, TRUE);
    }
    break;
    case WM_LBUTTONUP:
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_KEYDOWN: 
        switch (wParam)
        {
        case VK_SPACE: if (programState == programstate::gaming) playerLay[0] = true; break; 
        case VK_RETURN: if (programState == programstate::gaming) playerLay[1] = true; break;
        }
        break; 
    case WM_DESTROY: 
        if (hBmMem)
        {
            DeleteObject(hBmMem); 
            hBmMem = NULL; 
        }
        //KillTimer(hWnd, 500); 
        PostQuitMessage(0); 
        break; 
    case WM_COMMAND: 
    {
        switch (LOWORD(wParam))
        {
        case IDM_START: 
        {
            startGameDlg.Begin(m_hInst, hWnd);
            if (!startGameDlg.Choose()) break;
            pGame = new Game(startGameDlg.NumOfPlayer(), startGameDlg.Player1ID(),
                startGameDlg.Player2ID(), startGameDlg.GetDifficulty());
            pGame->InitNewLevel(0, true);
            programState = programstate::gaming;
            pScanDataTask = new std::future<void>(std::async(&UI::ScanData, this));
            pRefreshScreenTask = new std::future<void>(std::async(&UI::RefreshScreen, this));
            pRoleControlTasks[1] = new std::future<void>(std::async(&UI::RoleControl, this, startGameDlg.Player1ID()));    //角色1
            if (startGameDlg.NumOfPlayer() == 2)
            {
                pRoleControlTasks[2] = new std::future<void>(std::async(&UI::RoleControl, this, startGameDlg.Player2ID()));    //角色2
                int i = 3;
                for (int j = 1; j <= 4; ++j)
                {
                    if (j == startGameDlg.Player1ID() || j == startGameDlg.Player2ID()) continue;
                    pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                    ++i;
                }
            }
            else if (startGameDlg.NumOfPlayer() == 1)
            {
                int i = 2;
                for (int j = 1; j <= 4; ++j)
                {
                    if (j == startGameDlg.Player1ID()) continue;
                    pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                    ++i;
                }
            }
            HMENU hMenu = GetMenu(hWnd);
            EnableMenuItem(hMenu, IDM_START, TRUE);
            EnableMenuItem(hMenu, IDM_RESTART, FALSE);
            EnableMenuItem(hMenu, IDM_PAUSE, FALSE);
            EnableMenuItem(hMenu, IDM_END, FALSE);
            break;
        }
        case IDM_END: 
            if (MessageBox(hWnd, TEXT("您真的要结束游戏吗？\nDo you really want to end this game?"),
                TEXT("Confirm"), MB_YESNO | MB_ICONWARNING) == IDYES) EndGame(3); 
            break; 
        case IDM_RESTART: 
            if (MessageBox(hWnd,
                TEXT("您真的要重新开始吗？您的本次游戏进度将不会保留。\nDo you really want to end this game? Your game will NOT be saved."),
                TEXT("Confirm"), MB_YESNO | MB_ICONWARNING) == IDYES)
            {
                EndGame(3); 
                SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDM_START, 0), 0); 
            }
            break; 
        case IDM_PAUSE: 
            programState = programstate::gamePulsing; 
            MessageBox(hWnd, TEXT("暂停中……\nPausing..."), TEXT("游戏暂停"), MB_OK); 
            programState = programstate::gaming;
            break; 
        case IDM_LIST:
        {
            auto scoreList = ReadHighScore(); 
            if (scoreList.size() != 6)
            {
                MessageBox(m_hWnd, TEXT("读取错误"), c_lpszError, MB_OK | MB_ICONERROR); 
                break; 
            }
            std::_tostringstream outStr; 
            if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) outStr.imbue(std::locale("chs")); 
            outStr << TEXT("单人：\n简单：") << scoreList[0].second << TEXT("\t") << scoreList[0].first << TEXT("\n"); 
            outStr << TEXT("中等：") << scoreList[1].second << TEXT("\t") << scoreList[1].first << TEXT("\n");
            outStr << TEXT("困难：") << scoreList[2].second << TEXT("\t") << scoreList[2].first << TEXT("\n");
            outStr << TEXT("\n"); 
            outStr << TEXT("双人：\n简单：") << scoreList[3].second << TEXT("\t") << scoreList[3].first << TEXT("\n");
            outStr << TEXT("中等：") << scoreList[4].second << TEXT("\t") << scoreList[4].first << TEXT("\n");
            outStr << TEXT("困难：") << scoreList[5].second << TEXT("\t") << scoreList[5].first << TEXT("\n");
            MessageBox(hWnd, outStr.str().c_str(), TEXT("高分榜"), MB_OK); 
            break; 
        }
        case IDM_HELP: 
        {
            MessageBox(m_hWnd, c_lpszHelp, TEXT("帮助"), MB_OK | MB_ICONQUESTION); 
            MessageBox(m_hWnd, c_lpszHelpProp, TEXT("道具系统"), MB_OK | MB_ICONQUESTION);
            MessageBox(m_hWnd, c_lpszHelpScore, TEXT("分数系统"), MB_OK | MB_ICONQUESTION);
            MessageBox(m_hWnd, c_lpszHelpMusic, TEXT("温馨提示"), MB_OK | MB_ICONQUESTION); 
            break; 
        }
        case IDM_ABOUT: 
            MessageBox(hWnd, c_lpszAbout, TEXT("About"), MB_OK | MB_ICONINFORMATION); 
            break; 
        case IDM_EXIT:                          //退出
            SendMessage(hWnd, WM_CLOSE, 0, 0); 
            break; 
        }
        break; 
    }
    case WM_CLOSE: 
        if (MessageBox(hWnd, TEXT("您确定要退出吗？\nAre you sure to quit?"), TEXT("Exit"), MB_YESNO | MB_ICONQUESTION) == IDYES)
            SendMessage(hWnd, WM_DESTROY, 0, 0);
        break; 
    default: 
        return false; 
    }
    return true; 
}

bool UI::LoadGameImg()
{
loadImage: 

    hBmBkgnd = (HBITMAP)LoadImage(m_hInst, BKGND_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMain = (HBITMAP)LoadImage(m_hInst, MAIN_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmRole = (HBITMAP)LoadImage(m_hInst, ROLE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmTnt = (HBITMAP)LoadImage(m_hInst, TNT_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmObstacle = (HBITMAP)LoadImage(m_hInst, OBSTACLE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
    hBmGlove = (HBITMAP)LoadImage(m_hInst, GLOVE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShield = (HBITMAP)LoadImage(m_hInst, SHIELD_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddTnt = (HBITMAP)LoadImage(m_hInst, ADDTNT_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAddLife = (HBITMAP)LoadImage(m_hInst, ADDLIFE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmShoe = (HBITMAP)LoadImage(m_hInst, SHOE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmJinKeLa = (HBITMAP)LoadImage(m_hInst, JINKELA_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLachrymator = (HBITMAP)LoadImage(m_hInst, LACHRYMATOR_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMine = (HBITMAP)LoadImage(m_hInst, MINE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmFire = (HBITMAP)LoadImage(m_hInst, FIRE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmIce = (HBITMAP)LoadImage(m_hInst, ICE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmGrenade = (HBITMAP)LoadImage(m_hInst, GRENADE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmMissile = (HBITMAP)LoadImage(m_hInst, MISSILE_PATH, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmBkgnd || !hBmRole || !hBmTnt || !hBmObstacle || !hBmGlove || !hBmShield | !hBmAddTnt || !hBmAddLife || !hBmShoe 
        || !hBmJinKeLa || !hBmLachrymator || !hBmMine || !hBmFire || !hBmIce || !hBmGrenade || !hBmMissile)
    {

        if (!hBmBkgnd) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(BKGND_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmMain) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(MAIN_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmRole) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ROLE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmTnt) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(TNT_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmObstacle) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(OBSTACLE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmGlove) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(GLOVE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmShield) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(SHIELD_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmAddTnt) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ADDTNT_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmAddLife) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ADDLIFE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmShoe) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(SHOE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmJinKeLa) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(JINKELA_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmLachrymator) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(LACHRYMATOR_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmMine) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(MINE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmFire) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(FIRE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmIce) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(ICE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmGrenade) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(GRENADE_PATH), c_lpszError, MB_OK | MB_ICONERROR);
        if (!hBmMissile) MessageBox(m_hWnd, IMAGE_LOAD_FAIL_STR(MISSILE_PATH), c_lpszError, MB_OK | MB_ICONERROR);

    choose: 
        switch (MessageBox(m_hWnd, IMAGE_LOAD_FAIL_RETRY, c_lpszError, MB_ABORTRETRYIGNORE | MB_ICONERROR))
        {
        case IDRETRY: goto loadImage; 
        default: case IDABORT: return false; 
        case IDIGNORE: 
            if (MessageBox(m_hWnd, c_lpszWarning, c_lpszWarningTitle, MB_YESNO | MB_ICONWARNING) != IDYES) goto choose; 
        }
    }

    if (hBmBkgnd) GetObject(hBmBkgnd, sizeof(BITMAP), &bmBkgnd); 
    if (hBmMain) GetObject(hBmMain, sizeof(BITMAP), &bmMain);
    if (hBmRole) GetObject(hBmRole, sizeof(BITMAP), &bmRole); 
    if (hBmTnt) GetObject(hBmTnt, sizeof(BITMAP), &bmTnt);
    if (hBmObstacle) GetObject(hBmObstacle, sizeof(BITMAP), &bmObstacle);
    if (hBmGlove) GetObject(hBmGlove, sizeof(BITMAP), &bmGlove);
    if (hBmShield) GetObject(hBmShield, sizeof(BITMAP), &bmShield);
    if (hBmAddTnt) GetObject(hBmAddTnt, sizeof(BITMAP), &bmAddTnt);
    if (hBmAddLife) GetObject(hBmAddLife, sizeof(BITMAP), &bmAddLife);
    if (hBmShoe) GetObject(hBmShoe, sizeof(BITMAP), &bmShoe);
    if (hBmJinKeLa) GetObject(hBmJinKeLa, sizeof(BITMAP), &bmJinKeLa);
    if (hBmLachrymator) GetObject(hBmLachrymator, sizeof(BITMAP), &bmLachrymator);
    if (hBmMine) GetObject(hBmMine, sizeof(BITMAP), &bmMine);
    if (hBmFire) GetObject(hBmFire, sizeof(BITMAP), &bmFire);
    if (hBmIce) GetObject(hBmIce, sizeof(BITMAP), &bmIce);
    if (hBmGrenade) GetObject(hBmGrenade, sizeof(BITMAP), &bmGrenade);
    if (hBmMissile) GetObject(hBmMissile, sizeof(BITMAP), &bmMissile);
	return true; 
}

void UI::CreateBuffer(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    hBmMem = CreateCompatibleBitmap(hdc, mainWndSize.x, mainWndSize.y); 
    ReleaseDC(hWnd, hdc); 
}

void UI::PlayMainMusic()
{
loadMainMusic: 
    WIN32_FIND_DATA wfd; 
    if (FindFirstFile(MAIN_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE)
    {
        switch (MessageBox(m_hWnd, MAIN_MUSIC_LOAD_FAIL_STR, c_lpszError, MB_RETRYCANCEL | MB_ICONERROR))
        {
        case IDRETRY: goto loadMainMusic; break;
        }
    }
    else PlaySound(MAIN_MUSIC_PATH, NULL, SND_ASYNC | SND_LOOP); 

loadBombMusic: if (FindFirstFile(BOMB_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(BOMB_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadBombMusic; 
loadPropMusic: if (FindFirstFile(PICK_PROP_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(PICK_PROP_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadPropMusic;
loadFailMusic: if (FindFirstFile(FAIL_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(FAIL_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadFailMusic;
loadSuccessMusic: if (FindFirstFile(SUCCESS_MUSIC_PATH, &wfd) == INVALID_HANDLE_VALUE
        && MessageBox(m_hWnd, MUSIC_LOAD_FAIL_STR(SUCCESS_MUSIC_PATH), c_lpszError, MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY) goto loadSuccessMusic;
}

void UI::ScanData()
{
    while (programState == programstate::gaming || programState == programstate::gamePulsing)
    {
        while (programState == programstate::gamePulsing) Sleep(1000 / dataFps); 
        pGame->CheckRole(); 
        pGame->CheckBomb(1000 / dataFps); 
        Sleep(1000 / dataFps); 
        int result = pGame->CheckGameEnd(); 
        if (result)
        {
            std::thread end(&UI::EndGame, this, result); 
            end.detach(); 
            break; 
        }
    }
}

void UI::RoleControl(int roleID)
{
    if (roleID == pGame->GetID1())
    {
        playerLay[0] = false; 
        while (programState == programstate::gaming || programState == programstate::gamePulsing)
        {
            while (programState == programstate::gamePulsing) Sleep(1000 / dataFps); 
            if (!pGame->GetRole(roleID)->IsLiving()) break; 
            if (GetKeyState('W') < 0) pGame->WalkUpOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('S') < 0) pGame->WalkDownOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('A') < 0) pGame->WalkLeftOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('D') < 0) pGame->WalkRightOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (playerLay[0]) pGame->LayTnt(pGame->GetID1()); 
            playerLay[0] = false; 
            Sleep(1000 / dataFps); 
        }
    }
    else if (roleID == pGame->GetID2())
    {
        playerLay[1] = false;
        while (programState == programstate::gaming || programState == programstate::gamePulsing)
        {
            while (programState == programstate::gamePulsing) Sleep(1000 / dataFps);
            if (!pGame->GetRole(roleID)->IsLiving()) break;
            if (GetKeyState(VK_UP) < 0) pGame->WalkUpOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (GetKeyState(VK_DOWN) < 0) pGame->WalkDownOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (GetKeyState(VK_LEFT) < 0) pGame->WalkLeftOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (GetKeyState(VK_RIGHT) < 0) pGame->WalkRightOneCell(pGame->GetID2(), 1000 / dataFps);
            else if (playerLay[1]) pGame->LayTnt(pGame->GetID2()); 
            playerLay[1] = false;
            Sleep(1000 / dataFps);
        }
    }
}

void UI::RefreshScreen()
{
    while (programState == programstate::gaming || programState == programstate::gamePulsing)
    {
        InvalidateRect(m_hWnd, NULL, FALSE); 
        Sleep(1000 / paintFps); 
    }
}

void UI::EndGame(int result)
{
    programState = programstate::changeLevel; 

    std::function<void(void)> waitTask = [this]()
    {
        //等待异步结束
        if (pScanDataTask)
        {
            pScanDataTask->wait(); delete pScanDataTask; pScanDataTask = nullptr;
        }
        if (pRefreshScreenTask)
        {
            pRefreshScreenTask->wait(); delete pRefreshScreenTask; pRefreshScreenTask = nullptr;
        }

        for (auto& pRoleControl : pRoleControlTasks)
        {
            if (pRoleControl)
            {
                pRoleControl->wait(); delete pRoleControl; pRoleControl = nullptr;
            }
        }
        Sleep(1000);
    }; 

    std::function<void(void)> beginGame = [this]()
    {
        programState = programstate::gaming;
        pScanDataTask = new std::future<void>(std::async(&UI::ScanData, this));
        pRefreshScreenTask = new std::future<void>(std::async(&UI::RefreshScreen, this));
        pRoleControlTasks[1] = new std::future<void>(std::async(&UI::RoleControl, this, pGame->GetID1()));    //角色1
        if (startGameDlg.NumOfPlayer() == 2)
        {
            pRoleControlTasks[2] = new std::future<void>(std::async(&UI::RoleControl, this, pGame->GetID2()));    //角色2
            int i = 3;
            for (int j = 1; j <= 4; ++j)
            {
                if (j == pGame->GetID1() || j == pGame->GetID2()) continue;
                pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                ++i;
            }
        }
        else if (startGameDlg.NumOfPlayer() == 1)
        {
            int i = 2;
            for (int j = 1; j <= 4; ++j)
            {
                if (j == pGame->GetID1()) continue;
                pRoleControlTasks[i] = new std::future<void>(std::async(&UI::AI, this, j));
                ++i;
            }
        }
    }; 

    std::function<void(void)> endGame = [this]()
    {
        programState = programstate::starting; 

        delete pGame;
        pGame = nullptr;

        HMENU hMenu = GetMenu(m_hWnd);
        EnableMenuItem(hMenu, IDM_START, FALSE);
        EnableMenuItem(hMenu, IDM_RESTART, TRUE);
        EnableMenuItem(hMenu, IDM_PAUSE, TRUE);
        EnableMenuItem(hMenu, IDM_END, TRUE);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }; 

    int totalScore = 0; 
    std::_tostringstream outStr; 
    if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) outStr.imbue(std::locale("chs"));
    int nowLevel = pGame->GetNowLevel();

    if (result == 1)    //玩家获胜
    {
        SuccessSound(); 
        waitTask(); 
        pGame->InitNewLevel(nowLevel, true);
        totalScore += pGame->GetRole(pGame->GetID1())->GetTotalScore();
        if (pGame->GetNumOfPlayer() == 2) totalScore += pGame->GetRole(pGame->GetID2())->GetTotalScore();
        if (nowLevel == pGame->GetNumOfLevel() - 1)     //通关
        {
            outStr << TEXT("恭喜通关！总分：") << totalScore << TEXT("\nCongratulations! Total score: ") << totalScore; 
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("Congratulations!"), MB_OK);
            newScore(pGame->GetNumOfPlayer(), pGame->GetDifficulty(), totalScore);
            endGame(); 
        }
        else
        {
            //进入下一关，开启新future
            ++nowLevel; 
            outStr << TEXT("你赢了，即将进入下一关。当前总分：") << totalScore << TEXT("\nYou win! Total score: ") << totalScore;
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("You win!"), MB_OK); 
            pGame->InitNewLevel(nowLevel, true); 
            beginGame(); 
        }
    }
    else if (result == 2)  //电脑获胜
    {
        FailSound(); 
        bool end = true; 
        if (pGame->GetRole(pGame->GetID1())->GetLife() > 0
            || pGame->GetNumOfPlayer() == 2 && pGame->GetRole(pGame->GetID2())->GetLife() > 0) end = false; 

        totalScore += pGame->GetRole(pGame->GetID1())->GetTotalScore();
        if (pGame->GetNumOfPlayer() == 2) totalScore += pGame->GetRole(pGame->GetID2())->GetTotalScore();

        if (end)        //如果结束了
        {

            outStr << TEXT("你失败了！最终分数：") << totalScore << TEXT("\nYou fail! Ultimate score: ") << totalScore; 
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("You fail!"), MB_OK); 
            newScore(pGame->GetNumOfPlayer(), pGame->GetDifficulty(), totalScore);
            waitTask(); 
            endGame(); 
        }
        else            //还没结束
        {
            outStr << TEXT("你输了！点击重新开始本关。当前分数：") << totalScore << TEXT("\nYou lose! Click to restart this level. Total score: ") << totalScore;
            MessageBox(m_hWnd, outStr.str().c_str(), TEXT("You lose!"), MB_OK);
            waitTask();
            pGame->InitNewLevel(nowLevel, false); 
            beginGame(); 
        }
    }
    else if (result == 3)   //玩家主动结束游戏
    {
        FailSound(); 
        totalScore += pGame->GetRole(pGame->GetID1())->GetTotalScore();
        if (pGame->GetNumOfPlayer() == 2) totalScore += pGame->GetRole(pGame->GetID2())->GetTotalScore(); 
        outStr << TEXT("游戏结束！总分：") << totalScore << TEXT("\nGame over! Total score: ") << totalScore;
        MessageBox(m_hWnd, outStr.str().c_str(), TEXT("Game over!"), MB_OK);
        newScore(pGame->GetNumOfPlayer(), pGame->GetDifficulty(), totalScore); 
        waitTask();
        endGame(); 
    }
}

void UI::AI(int roleID)
{
    int rows, cols; 
    if (pGame) rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size(); 
    else return; 

    int computerID1 = 0, computerID2 = 0, computerID3 = 0, computerID4 = 0; 
    for (int i = 1; i <= 4; ++i)
    {
        if (IsComputer(i))
        {
            if (computerID1 == 0) computerID1 = i;
            else if (computerID2 == 0) computerID2 = i;
            else if (computerID3 == 0) computerID3 = i; 
            else if (computerID4 == 0) computerID4 = i;
        }
    }
    
    std::vector<std::pair<std::pair<int, int>, bool>> laidMine;                                             //记录自己放下的地雷
    std::mutex laidMineMutex;                                                                               //管理它的互斥锁
    std::vector<std::thread*> laidMineThread; 

    //判断是否越界
    std::function<bool(int, int)> InRange = [rows, cols](int xc, int yc) { return xc >= 0 && yc >= 0 && xc < rows && yc < cols; }; 

    while (programState == programstate::gaming || programState == programstate::gamePulsing)               //游戏还没结束
    {
        while (programState == programstate::gamePulsing) Sleep(1000 / dataFps);                            //游戏暂停状态，停止
        
        if (!pGame->GetRole(roleID)->IsLiving()) break;                                                     //人死了，停止

        //获取角色位置
        auto [x, y] = pGame->GetRole(roleID)->GetPos(); 
        int xc = pGame->PosToCell(x), yc = pGame->PosToCell(y); 
        
        //检查周围的危险
        std::vector<std::vector<bool>> dangerous(rows, std::vector<bool>(cols, false));                     //记录是否有危险
        std::vector<std::vector<bool>> obstacle(rows, std::vector<bool>(cols, false));                      //记录是否有障碍
        std::initializer_list<int> xList{ -1, 1, 0, 0 }; 
        std::initializer_list<int> yList{ 0, 0, -1, 1 }; 
        if (roleID == computerID2)
        {
            xList = { 0, 0, -1, 1 }; 
            yList = { -1, 1, 0, 0 }; 
        }
        else if (roleID == computerID3)
        {
            xList = { 0, 0, 1, -1 };
            yList = { 1, -1, 0, 0 };
        }
        const std::vector<int> xMove(xList); 
        const std::vector<int> yMove(yList); 
        for (int tmpXc = 0; tmpXc < rows; ++tmpXc)
            for (int tmpYc = 0; tmpYc < cols; ++tmpYc)
            {
                auto mapObjList = pGame->GetMapObj(tmpXc, tmpYc);
                for (auto pObj : mapObjList)
                {
                    switch (pObj->GetObjType())
                    {
                    case obj_base::objType::tnt:                                                                //如果是TNT
                    {

                        std::function<void(int, int)> bombTnt = [&dangerous, &obstacle, pObj, &InRange, &xMove, &yMove, this](int xc, int yc)
                        {
                            dangerous[xc][yc] = true;                                                           //标记为危险区
                            obstacle[xc][yc] = true;                                                            //不能踩上该点
                            int distance = dynamic_cast<TNT*>(pObj)->GetDistance();
                            for (int i = 0; i < 4; ++i)                                                         //向四周窥探
                            {
                                for (int j = 1; j < distance; ++j)
                                {
                                    int newXc = xc + j * xMove[i], newYc = yc + j * yMove[i];
                                    if (!InRange(newXc, newYc)) break;                                          //越界
                                    auto tmpMapObjList = pGame->GetMapObj(newXc, newYc);
                                    bool jumpOut = false;
                                    for (auto pTmpMapObj : tmpMapObjList)                                       //有障碍
                                        if (pTmpMapObj->GetObjType() == obj_base::objType::hardObstacle) { jumpOut = true; break; }
                                        else if (pTmpMapObj->GetObjType() == obj_base::objType::softObstacle)
                                        {
                                            dangerous[newXc][newYc] = true; jumpOut = true; break;
                                        }
                                    if (jumpOut) break;
                                    dangerous[newXc][newYc] = true;                                             //此地危险
                                }
                            }
                        }; 
                        if (!dynamic_cast<TNT*>(pObj)->IsMoving())                                              //静止的炸弹
                            bombTnt(tmpXc, tmpYc); 
                        else
                        {
                            int tmpXMove = 0, tmpYMove = 0; 
                            switch (dynamic_cast<TNT*>(pObj)->GetDirect())
                            {
                            case Prop::direction::Up: tmpXMove = -1; tmpYMove = 0; break; 
                            case Prop::direction::Down: tmpXMove = 1; tmpYMove = 0; break;
                            case Prop::direction::Left: tmpXMove = 0; tmpYMove = -1; break;
                            case Prop::direction::Right: tmpXMove = 0; tmpYMove = 1; break;
                            }
                            if (tmpXMove == 0 && tmpYMove == 0) break; 
                            for (int j = 1; ; ++j)
                            {
                                int nextXc = tmpXc + j * tmpXMove, nextYc = tmpYc + j * tmpYMove; 
                                bool hereBomb = false; 
                                if (!InRange(nextXc, nextYc)) hereBomb = true; 
                                if (!hereBomb)
                                {
                                    auto hereMapObjList = pGame->GetMapObj(nextXc, nextYc); 
                                    for (auto pHereMapObj : hereMapObjList)
                                    {
                                        switch (pHereMapObj->GetObjType())
                                        {
                                        case obj_base::objType::softObstacle: 
                                        case obj_base::objType::hardObstacle:
                                        case obj_base::objType::tnt:
                                            hereBomb = true; 
                                            break; 
                                        }
                                        if (hereBomb) break; 
                                    }
                                }
                                if (hereBomb)
                                {
                                    nextXc -= tmpXMove; nextYc -= tmpYMove; 
                                    if (InRange(nextXc, nextYc)) bombTnt(nextXc, nextYc); 
                                    break; 
                                }
                            }
                        }
                        break;
                    }
                    case obj_base::objType::bombArea:                                                       //正在爆炸，危险
                        dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true; 
                        break; 
                    case obj_base::objType::hardObstacle: case obj_base::objType::softObstacle:             //有障碍
                        obstacle[tmpXc][tmpYc] = true; 
                        break; 
                    case obj_base::objType::prop: 
                    {
                        Prop* pProp = dynamic_cast<Prop*>(pObj); 
                        if (!pProp->IsLaid()) break;                                                        //不是放置的道具，无危险
                        switch (pProp->GetPropType())
                        {
                        case Prop::propType::mine:                                                          //地雷和催泪瓦斯，如果可见且不是自己的则绝对不能踩中
                            if (dynamic_cast<Mine*>(pProp)->GetOwnerID() != roleID && dynamic_cast<Mine*>(pProp)->Visible())
                                dangerous[tmpXc][tmpYc] = true, obstacle[tmpXc][tmpYc] = true;
                            break; 
                        case Prop::propType::lachrymator: 
                            if (dynamic_cast<Lachrymator*>(pProp)->GetOwnerID() != roleID && dynamic_cast<Lachrymator*>(pProp)->Visible())
                                dangerous[tmpXc][tmpYc] = true; 
                            break; 
                        case Prop::propType::missile:                                                       //导弹炸整条直线
                        {
                            dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true;
                            int direct = 0; 
                            switch (dynamic_cast<Missile*>(pProp)->GetDirection())
                            {
                            case obj_base::direction::Up: direct = 0; break; 
                            case obj_base::direction::Down: direct = 1; break; 
                            case obj_base::direction::Left: direct = 2; break;
                            case obj_base::direction::Right: direct = 3; break;
                            }
                            for (int i = 1;; ++i)
                            {
                                int dangerXc = tmpXc + i * xMove[direct], dangerYc = tmpYc + i * yMove[direct]; 
                                if (!InRange(dangerXc, dangerYc)) break; 
                                auto dangerMapObjList = pGame->GetMapObj(dangerXc, dangerYc); 
                                bool stop = false; 
                                for (auto pDangerMapObj : dangerMapObjList)
                                    if (pDangerMapObj->GetObjType() == obj_base::objType::hardObstacle) stop = true; 
                                    else if (pDangerMapObj->GetObjType() == obj_base::objType::softObstacle) { stop = true; dangerous[dangerXc][dangerYc] = true; }
                                if (stop) break; 
                                dangerous[dangerXc][dangerYc] = true; 
                            }
                            break; 
                        }
                        case Prop::propType::grenade:                                                       //手榴弹的终点位置周围的十字都是危险区
                        {
                            Grenade* pGrenade = dynamic_cast<Grenade*>(pProp); 
                            auto [tmpX, tmpY] = pGrenade->GetPos(); 
                            auto leftDistance = pGrenade->GetLeftDistance(); 
                            switch (pGrenade->GetDirection())
                            {
                            case Prop::direction::Up: tmpX -= leftDistance; break;  
                            case Prop::direction::Down: tmpX += leftDistance; break; 
                            case Prop::direction::Left: tmpY -= leftDistance; break; 
                            case Prop::direction::Right: tmpY += leftDistance; break; 
                            }
                            int tmpXc = pGame->PosToCell(tmpX), tmpYc = pGame->PosToCell(tmpY); 
                            if (InRange(tmpXc, tmpYc)) dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true; 
                            for (int i = 0; i < 4; ++i)
                            {
                                int nextXc = tmpXc + xMove[i], nextYc = tmpYc + yMove[i]; 
                                if (!InRange(nextXc, nextYc)) continue; 
                                dangerous[tmpXc][tmpYc] = obstacle[tmpXc][tmpYc] = true; 
                            }
                            break; 
                        }
                        }
                        break; 
                    }
                    }
                }
            }
        
        if (dangerous[xc][yc])                                                                              //有危险，赶紧逃
        {
            //优先尝试无危险道路
            if (InRange(xc - 1, yc) && !dangerous[xc - 1][yc] && !obstacle[xc - 1][yc]) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
            else if (InRange(xc + 1, yc) && !dangerous[xc + 1][yc] && !obstacle[xc + 1][yc]) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
            else if (InRange(xc, yc - 1) && !dangerous[xc][yc - 1] && !obstacle[xc][yc - 1]) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
            else if (InRange(xc, yc + 1) && !dangerous[xc][yc + 1] && !obstacle[xc][yc + 1]) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
            else                                                                                            //无法找到无危险道路，尝试最近的暂时无生命危险的道路
            {
                std::queue<std::pair<std::pair<int, int>, obj_base::direction>> q;                          //广搜寻找出路，记录目标点位置和第一步迈出的方向
                std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols, false));            //记录是否被访问过

                //开始广搜
                if (InRange(xc - 1, yc) && !obstacle[xc - 1][yc])
                {
                    q.emplace(std::make_pair(xc - 1, yc), obj_base::direction::Up); hasVisited[xc - 1][yc] = true; 
                }
                if (InRange(xc + 1, yc) && !obstacle[xc + 1][yc])
                {
                    q.emplace(std::make_pair(xc + 1, yc), obj_base::direction::Down); hasVisited[xc + 1][yc] = true; 
                }
                if (InRange(xc, yc - 1) && !obstacle[xc][yc - 1]) 
                {
                    q.emplace(std::make_pair(xc, yc - 1), obj_base::direction::Left); hasVisited[xc][yc - 1] = true; 
                }
                if (InRange(xc, yc + 1) && !obstacle[xc][yc + 1])
                {
                    q.emplace(std::make_pair(xc, yc + 1), obj_base::direction::Right); hasVisited[xc][yc + 1] = true; 
                }
                while (!q.empty())
                {
                    auto [frontXc, frontYc] = q.front().first; 
                    auto direct = q.front().second; 
                    if (!dangerous[frontXc][frontYc]) break;                                                  //找到了安全的地方，跳出
                    q.pop(); 
                    for (int i = 0; i < 4; ++i)                                                               //向四周遍历
                    {
                        int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i]; 
                        if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !obstacle[nextXc][nextYc])
                        {
                            q.emplace(std::make_pair(nextXc, nextYc), direct); 
                            hasVisited[nextXc][nextYc] = true; 
                        }
                    }
                }

                //如果队列不空，说明找到了路线
                if (!q.empty())
                {
                    switch (q.front().second)
                    {
                    case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                    case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                    }
                }
                else switch (pGame->GetRandom() % 4)                                                        //无法躲避危险，随机走动
                {
                case 0: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                case 1: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                case 2: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                case 3: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break; 
                }
            }
        }
        else                                                                                                //没有危险，开始自主行动
        {
            std::vector<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> unpickedProps; 
            std::vector<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> enemies;
            std::vector<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> softObstacles; 
            std::queue<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> q; 
            std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols, false)); 

            //先录入本格的敌人
            auto localMapObjList = pGame->GetMapObj(xc, yc); 
            for (auto pLocalMapObj : localMapObjList)
                if (pLocalMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pLocalMapObj)->GetID()))
                    enemies.emplace_back(std::make_pair(xc, yc), std::make_pair(obj_base::direction::Null, 0));
            
            //开始广搜
            if (InRange(xc - 1, yc) && !dangerous[xc - 1][yc] && !obstacle[xc - 1][yc])
            {
                q.emplace(std::make_pair(xc - 1, yc), std::make_pair(obj_base::direction::Up, 1)); hasVisited[xc - 1][yc] = true;
            }
            if (InRange(xc + 1, yc) && !dangerous[xc + 1][yc] && !obstacle[xc + 1][yc])
            {
                q.emplace(std::make_pair(xc + 1, yc), std::make_pair(obj_base::direction::Down, 1)); hasVisited[xc + 1][yc] = true;
            }
            if (InRange(xc, yc - 1) && !dangerous[xc][yc - 1] && !obstacle[xc][yc - 1])
            {
                q.emplace(std::make_pair(xc, yc - 1), std::make_pair(obj_base::direction::Left, 1)); hasVisited[xc][yc - 1] = true;
            }
            if (InRange(xc, yc + 1) && !dangerous[xc][yc + 1] && !obstacle[xc][yc + 1])
            {
                q.emplace(std::make_pair(xc, yc + 1), std::make_pair(obj_base::direction::Right, 1)); hasVisited[xc][yc + 1] = true;
            }

            while (!q.empty())
            {
                auto [frontXc, frontYc] = q.front().first;
                auto direct = q.front().second.first; 
                int depth = q.front().second.second; 
                q.pop();
                auto mapObjList = pGame->GetMapObj(frontXc, frontYc);                                       //查看这一格的物品
                for (auto pMapObj : mapObjList)
                {
                    switch (pMapObj->GetObjType())
                    {
                    case obj_base::objType::role:                                                           //如果是人物
                        if (IsComputer(dynamic_cast<Role*>(pMapObj)->GetID())) break;                       //如果不是敌人，跳出去不录入
                        enemies.emplace_back(std::make_pair(frontXc, frontYc), std::make_pair(direct, depth)); 
                        break; 
                    case obj_base::objType::prop:                                                           //道具
                        if (!dynamic_cast<Prop*>(pMapObj)->IsUnpicked()) break;                             //不是未捡起的道具，跳出去不录入
                        unpickedProps.emplace_back(std::make_pair(frontXc, frontYc), std::make_pair(direct, depth)); 
                        break; 
                    case obj_base::objType::softObstacle:                                                   //软障碍，直接录入
                        softObstacles.emplace_back(std::make_pair(frontXc, frontYc), std::make_pair(direct, depth)); 
                        break; 
                    }
                }

                bool hasLaidMine = false;                                                                   //检查是否有自己埋藏的地雷
                laidMineMutex.lock(); 
                for (int i = 0; i < (int)laidMine.size(); ++i)
                {
                    if (laidMine[i].second == false) continue;                                              //已经爆炸完毕的导弹
                    if (laidMine[i].first.first == frontXc && laidMine[i].first.second == frontYc)
                    {
                        hasLaidMine = true; break; 
                    }
                }
                laidMineMutex.unlock(); 

                if (!obstacle[frontXc][frontYc]) for (int i = 0; i < 4; ++i)                                //没有障碍，就可以向四周遍历
                {
                    int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i]; 
                    if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !dangerous[nextXc][nextYc])
                    {
                        bool isSoftObstacle = false; 
                        if (hasLaidMine)                                                                    //如果本格有未爆炸的地雷，不录入旁边的软障碍
                        {
                            for (auto pNextMapObj : pGame->GetMapObj(nextXc, nextYc))
                            {
                                if (pNextMapObj->GetObjType() == obj_base::objType::softObstacle) isSoftObstacle = true; 
                            }
                        }
                        if (!isSoftObstacle)
                        {
                            q.emplace(std::make_pair(nextXc, nextYc), std::make_pair(direct, depth + 1));
                            hasVisited[nextXc][nextYc] = true;
                        }
                    }
                }
            }

            //临时函数，用于判断如果放置了TNT会不会成功逃生，后面用到
            std::function<obj_base::direction(int, int, int, int)> canRunAway = [this, &dangerous, &obstacle, &xMove, &yMove, &InRange, rows, cols, roleID](int roleXc, int roleYc, int tntXc, int tntYc)
            {
                std::vector<std::vector<bool>> avoid(rows, std::vector<bool>(cols, false));     //记录若本次放置炸弹则炸到的区域
                avoid[tntXc][tntYc] = true;
                int distance = pGame->GetRole(roleID)->GetDistance();
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = 1; j < distance; ++j)
                    {
                        int tmpXc = tntXc + xMove[i] * j, tmpYc = tntYc + yMove[i] * j;
                        if (!InRange(tmpXc, tmpYc)) break;
                        auto mapObjList = pGame->GetMapObj(tmpXc, tmpYc);
                        bool stop = false;
                        for (auto pMapObjList : mapObjList)
                        {
                            switch (pMapObjList->GetObjType())
                            {
                            case obj_base::objType::softObstacle: case obj_base::objType::hardObstacle:
                                stop = true; break;
                            }
                            if (stop) break;
                        }
                        if (!stop) avoid[tmpXc][tmpYc] = true;
                    }
                }

                std::queue<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> q;              //搜索一旦放完炸弹后是否有出路
                std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols));
                if (InRange(roleXc - 1, roleYc) && !obstacle[roleXc - 1][roleYc] && !dangerous[roleXc - 1][roleYc])
                {
                    q.emplace(std::make_pair(roleXc - 1, roleYc), std::make_pair(obj_base::direction::Up, 1)); hasVisited[roleXc - 1][roleYc] = true;
                }
                if (InRange(roleXc + 1, roleYc) && !obstacle[roleXc + 1][roleYc] && !dangerous[roleXc + 1][roleYc])
                {
                    q.emplace(std::make_pair(roleXc + 1, roleYc), std::make_pair(obj_base::direction::Down, 1)); hasVisited[roleXc + 1][roleYc] = true;
                }
                if (InRange(roleXc, roleYc - 1) && !obstacle[roleXc][roleYc - 1] && !dangerous[roleXc][roleYc - 1])
                {
                    q.emplace(std::make_pair(roleXc, roleYc - 1), std::make_pair(obj_base::direction::Left, 1)); hasVisited[roleXc][roleYc - 1] = true;
                }
                if (InRange(roleXc, roleYc + 1) && !obstacle[roleXc][roleYc + 1] && !dangerous[roleXc][roleYc + 1])
                {
                    q.emplace(std::make_pair(roleXc, roleYc + 1), std::make_pair(obj_base::direction::Right, 1)); hasVisited[roleXc][roleYc + 1] = true;
                }

                while (!q.empty())
                {
                    auto [frontXc, frontYc] = q.front().first;
                    auto [direct, depth] = q.front().second;
                    q.pop();
                    if (!avoid[frontXc][frontYc])                                   //找到了逃生路线，查看自己是否有时间安全逃离
                    {
                        auto speed = pGame->GetRole(roleID)->GetMoveSpeed(); 
                        if (depth * Game::defPosUnitPerCell <= Game::tntInitialTime * speed * dataFps / 1000) return direct;   //能逃离
                        else return obj_base::direction::Null;                                                                 //安全地带太远了，不能逃离
                    }
                    for (int i = 0; i < 4; ++i)
                    {
                        int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i];
                        if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !dangerous[nextXc][nextYc] && !obstacle[nextXc][nextYc])
                        {
                            q.emplace(std::make_pair(nextXc, nextYc), std::make_pair(direct, depth + 1)); hasVisited[nextXc][nextYc] = true;
                        }
                    }
                }

                return obj_base::direction::Null;                                   //没有找到逃生路线
            };
            
            //查看当前武器
            SpecialBomb* pWeapon = pGame->GetRole(roleID)->GetWeapon(); 
            bool attack = false;
            if (pWeapon)
            {
                //优先攻击敌人
                switch (pWeapon->GetPropType())
                {
                case Prop::propType::mine:                                  //手中有地雷或催泪瓦斯并且敌人不与自己同一格或者自己有护盾，且敌人在不远处就安放
                case Prop::propType::lachrymator:
                {
                    auto thisMapObjList = pGame->GetMapObj(xc, yc);
                    attack = true;
                    for (auto pThisMapObj : thisMapObjList)
                        if (pThisMapObj->GetObjType() == obj_base::objType::role && dynamic_cast<Role*>(pThisMapObj)->GetID() != roleID
                            && (!pGame->GetRole(roleID)->HaveShield() || IsComputer(dynamic_cast<Role*>(pThisMapObj)->GetID())))
                        {
                            attack = false; break;
                        }
                    if (attack)                                             //如果攻击没有危险
                    {
                        if (!enemies.empty())                               //如果没有连通敌人，用了地雷；有连通敌人进入考虑
                        {
                            if (enemies[0].second.second > 4 && enemies[0].second.second <= 7)   //敌人不远不近，先接近一步再说；过远或过近都放
                            {
                                switch (enemies[0].second.first)
                                {
                                case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Null: attack = false; break;
                                }
                                if (attack) break;                          //已经行走了，跳出来，防止触发下面的LayTnt
                            }
                        }
                    }

                    if (attack)                                             //最终决定放置地雷
                    {
                        pGame->LayTnt(roleID); 
                        if (pGame->GetRole(roleID)->GetWeapon() == nullptr)
                        {
                            int now = laidMine.size(); 
                            laidMineMutex.lock(); 
                            laidMine.emplace_back(std::make_pair(xc, yc), true); 
                            laidMineMutex.unlock(); 
                            laidMineThread.emplace_back(new std::thread
                            ([&laidMineMutex, &laidMine](int i)
                                {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(Game::mineInitialTime));
                                    laidMineMutex.lock(); laidMine[i].second = false; laidMineMutex.unlock();
                                }, now
                            )); 
                        }
                    }
                    break;
                }
                case Prop::propType::fire:                                  //火焰枪，判断敌人是否在路上
                {
                    int xMove = 0, yMove = 0;
                    switch (pGame->GetRole(roleID)->GetDirect())            //获取角色面对的方向
                    {
                    case obj_base::direction::Up: xMove = -1; break;
                    case obj_base::direction::Down: xMove = 1; break;
                    case obj_base::direction::Left: yMove = -1; break;
                    case obj_base::direction::Right: yMove = 1; break;
                    }
                    for (int i = 1; i <= Game::fireMaxDistance; ++i)        //在火焰枪爆炸范围内能找到敌人
                    {
                        int nowX = xc + xMove * i, nowY = yc + yMove * i;
                        if (!InRange(nowX, nowY)) break;
                        auto nextMapObjList = pGame->GetMapObj(nowX, nowY);
                        for (auto pNextMapObj : nextMapObjList)
                            if (pNextMapObj->GetObjType() == obj_base::objType::role && IsPlayer(dynamic_cast<Role*>(pNextMapObj)->GetID()))
                            {
                                attack = true; break;
                            }
                    }
                    if (attack) pGame->LayTnt(roleID);                      //开炸
                    else if (!enemies.empty())
                    {
                        for (int i = 0; i < (int)enemies.size(); ++i)       //敌人不空，炸敌人
                        {
                            if (enemies[i].second.second > 2 * Game::fireMaxDistance) break;       //敌人太远
                            if (enemies[i].first.first == xc)               //有同排敌人，检查是否追击，如果中间没有障碍就追击
                            {
                                attack = true;
                                if (enemies[i].first.second > yc)
                                {
                                    for (int nextYc = yc + 1; nextYc <= enemies[i].first.second - Game::fireMaxDistance; ++nextYc)
                                        if (dangerous[xc][nextYc] || obstacle[xc][nextYc]) { attack = false; break; }
                                }
                                else
                                {
                                    for (int nextYc = yc - 1; nextYc >= enemies[i].first.second + Game::fireMaxDistance; --nextYc)
                                        if (dangerous[xc][nextYc] || obstacle[xc][nextYc]) { attack = false; break; }
                                }
                            }
                            else if (enemies[i].first.second == yc)
                            {
                                attack = true;
                                if (enemies[i].first.first > xc)
                                {
                                    for (int nextXc = xc + 1; nextXc <= enemies[i].first.first - Game::fireMaxDistance; ++nextXc)
                                        if (dangerous[nextXc][yc] || obstacle[nextXc][yc]) { attack = false; break; }
                                }
                                else
                                {
                                    for (int nextXc = xc - 1; nextXc >= enemies[i].first.first + Game::fireMaxDistance; --nextXc)
                                        if (dangerous[nextXc][yc] || obstacle[nextXc][yc]) { attack = false; break; }
                                }
                            }
                            if (attack) switch (enemies[i].second.first)            //有同排敌人，追击
                            {
                            case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Null: attack = false; break;
                            }
                        }
                    }
                    break;
                }
                case Prop::propType::grenade:
                {
                    int destXc = xc, destYc = yc;
                    switch (pGame->GetRole(roleID)->GetDirect())                    //获取角色面对的方向
                    {
                    case obj_base::direction::Up: destXc -= 2; break;
                    case obj_base::direction::Down: destXc += 2; break;
                    case obj_base::direction::Left: destYc -= 2; break;
                    case obj_base::direction::Right: destYc += 2; break;
                    }
                    if (InRange(destXc, destYc))
                    {
                        auto destMapObjList = pGame->GetMapObj(destXc, destYc);     //搜索手榴弹爆炸位置有无敌人
                        for (auto pDestMapObj : destMapObjList)
                            if (pDestMapObj->GetObjType() == obj_base::objType::role && IsPlayer(dynamic_cast<Role*>(pDestMapObj)->GetID()))
                            {
                                attack = true; break;
                            }
                        if (!attack) for (int i = 0; i < 4; ++i)                    //搜索手榴弹爆炸的范围有没有敌人
                        {
                            int aroundXc = destXc + xMove[i], aroundYc = destYc + yMove[i];
                            if (!InRange(aroundXc, aroundYc)) continue;
                            auto aroundMapObjList = pGame->GetMapObj(aroundXc, aroundYc);
                            for (auto pAroundMapObj : aroundMapObjList)
                                if (pAroundMapObj->GetObjType() == obj_base::objType::role && IsPlayer(dynamic_cast<Role*>(pAroundMapObj)->GetID()))
                                {
                                    attack = true; break;
                                }
                            if (attack) break;
                        }
                        if (attack) { pGame->LayTnt(roleID); Sleep(1000 / dataFps); } //如果有敌人，炸他
                        else
                        {
                            for (int i = 0; i < 4; ++i)                             //没有敌人，搜寻四周
                            {
                                int destXc = xc + xMove[i], destYc = yc + yMove[i];
                                if (!InRange(destXc, destYc)) continue; 
                                if (dangerous[destXc][destYc]) continue;            //往那边走会有危险
                                if (obstacle[destXc][destYc]) { destXc += xMove[i], destYc += yMove[i]; }
                                else { destXc += 2 * xMove[i], destYc += 2 * yMove[i]; }
                                if (!InRange(destXc, destYc)) continue; 
                                bool stop = false; 
                                for (auto pMapObj : pGame->GetMapObj(destXc, destYc))
                                {
                                    if (pMapObj->GetObjType() == obj_base::objType::role
                                        && IsPlayer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                    {
                                        attack = true; break; 
                                    }
                                    else if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)
                                    {
                                        stop = true; break; 
                                    }
                                }
                                if (stop) continue; 
                                if (!attack)                                        //向落点四周查找
                                {
                                    for (int j = 0; j < 4; ++j)
                                    {
                                        int aroundXc = destXc + xMove[j], aroundYc = destYc + yMove[j]; 
                                        if (!InRange(aroundXc, aroundYc)) continue; 
                                        for (auto pMapObj : pGame->GetMapObj(aroundXc, aroundYc))
                                        {
                                            if (pMapObj->GetObjType() == obj_base::objType::role
                                                && IsPlayer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                            {
                                                attack = true; break; 
                                            }
                                        }
                                        if (attack) break; 
                                    }
                                }
                                if (attack)                                         //如果可以炸到人
                                {
                                    if (xMove[i] == -1 && yMove[i] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 1 && yMove[i] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps); 
                                    pGame->LayTnt(roleID); Sleep(1000 / dataFps); 
                                }
                            }
                        }
                    }
                    break;
                }
                case Prop::propType::missile:                                       //是导弹
                {
                    int tmpXMove = 0, tmpYMove = 0;
                    switch (pGame->GetRole(roleID)->GetDirect())                    //检查这一排有没有敌人
                    {
                    case obj_base::direction::Up: tmpXMove = -1; break;
                    case obj_base::direction::Down: tmpXMove = 1; break;
                    case obj_base::direction::Left: tmpYMove = -1; break;
                    case obj_base::direction::Right: tmpYMove = 1; break;
                    }
                    int nextXc = xc + tmpXMove, nextYc = yc + tmpYMove;
                    if (InRange(nextXc, nextYc))
                    {
                        auto nextMapObjList = pGame->GetMapObj(nextXc, nextYc);
                        for (auto pNextMapObj : nextMapObjList)
                        {
                            bool stop = false;
                            switch (pNextMapObj->GetObjType())
                            {
                            case obj_base::objType::hardObstacle:                   //碰到障碍都没有遇到角色，不攻击
                            case obj_base::objType::softObstacle:
                                stop = true; break;
                            case obj_base::objType::role:                           //遇到角色，攻击
                                attack = true; stop = true; break;
                            }
                            if (stop) break;
                        }
                        if (attack) pGame->LayTnt(roleID);
                    }
                    break;
                }
                }
            }
            else if (pGame->GetRole(roleID)->GetLeftTntNum() > 0)                   //手中没有道具，只有普通炸弹
            {
                bool canLayTnt = true; 
                auto mapObjList = pGame->GetMapObj(xc, yc); 
                for (auto pMapObj : mapObjList)
                {
                    switch (pMapObj->GetObjType())
                    {
                    case obj_base::objType::role: 
                        if (IsPlayer(dynamic_cast<Role*>(pMapObj)->GetID())) attack = true; 
                        break; 
                    case obj_base::objType::prop: 
                        switch (dynamic_cast<Prop*>(pMapObj)->GetPropType())
                        {
                        case Prop::propType::mine: case Prop::propType::lachrymator: 
                            if (dynamic_cast<Prop*>(pMapObj)->IsLaid()) canLayTnt = false; 
                            break; 
                        }
                        break; 
                    }
                    if (!canLayTnt) break; 
                }
                if (canLayTnt && !attack)
                {
                    int distance = pGame->GetRole(roleID)->GetDistance(); 
                    for (int i = 0; i < 4; ++i)
                    {
                        for (int j = 1; j < distance; ++j)
                        {
                            int tmpXc = xc + xMove[i] * j, tmpYc = yc + yMove[i] * j; 
                            if (!InRange(tmpXc, tmpYc)) break; 
                            auto tmpMapObjList = pGame->GetMapObj(tmpXc, tmpYc); 
                            bool stop = false; 
                            for (auto pTmpMapObj : tmpMapObjList)
                            {
                                switch (pTmpMapObj->GetObjType())
                                {
                                case obj_base::objType::softObstacle: case obj_base::objType::hardObstacle: 
                                    stop = true; 
                                    break; 
                                case obj_base::objType::role: 
                                    if (IsPlayer(dynamic_cast<Role*>(pTmpMapObj)->GetID())) attack = true; 
                                    break; 
                                }
                            }
                            if (stop || attack) break; 
                        }
                        if (attack) break; 
                    }
                }
                else if (!canLayTnt) attack = false; 

                

                if (attack)                                                             //如果发现敌人
                {
                    obj_base::direction runAwayDirect = canRunAway(xc, yc, xc, yc); 
                    switch (runAwayDirect)
                    {
                    case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                    case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                    case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break; 
                    default: attack = false; break; 
                    }
                }
            }

            if (!attack)                                                            //如果没有攻击敌人
            {
                if (pWeapon)                                                        //如果有道具
                {
                    switch (pWeapon->GetPropType())
                    {
                    case Prop::propType::mine: 
                    case Prop::propType::lachrymator: 
                        break; 
                    case Prop::propType::missile: 
                    case Prop::propType::fire:                                      //先看旁边有没有软障碍
                    {
                        for (int i = 0; i < 4; ++i)                                 //搜查四周
                        {
                            int nearXc = xc + xMove[i], nearYc = yc + yMove[i];
                            if (!InRange(nearXc, nearYc)) continue;
                            bool stop = false; 
                            for (auto pMapObj : pGame->GetMapObj(nearXc, nearYc))
                            {
                                if (pMapObj->GetObjType() == obj_base::objType::softObstacle)
                                {
                                    attack = true; break;
                                }
                                else if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)
                                {
                                    stop = true; break; 
                                }
                                else if (pMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                {
                                    stop = true; break; 
                                }
                            }
                            if (stop) { attack = false; continue; }

                            //搜查沿线几格的
                            if (dangerous[nearXc][nearYc]) { attack = false; continue; }
                            for (int j = 2; j <= Game::fireMaxDistance; ++j)
                            {
                                int nextXc = xc + xMove[i] * j, nextYc = yc + yMove[i] * j;
                                if (!InRange(nextXc, nextYc)) break;
                                bool computerExists = false;
                                for (auto pMapObj : pGame->GetMapObj(nextXc, nextYc))
                                {
                                    if (pMapObj->GetObjType() == obj_base::objType::softObstacle)
                                    {
                                        attack = true; break;
                                    }
                                    else if (pMapObj->GetObjType() == obj_base::objType::hardObstacle)
                                    {
                                        stop = true; break;
                                    }
                                    else if (pMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pMapObj)->GetID()))
                                    {
                                        computerExists = true; break;
                                    }
                                }
                                if (computerExists) { attack = false; break; }
                                if (stop || attack) break;
                            }
                            
                            if (attack)
                            {
                                if (xMove[i] == -1 && yMove[i] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps); 
                                else if (xMove[i] == 1 && yMove[i] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                                else if (xMove[i] == 0 && yMove[i] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                                else if (xMove[i] == 0 && yMove[i] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
                                pGame->LayTnt(roleID); 
                                break; 
                            }
                        }
                        break;
                    }
                    case Prop::propType::grenade: 
                    {

                        //判断是否能炸到软障碍
                        std::function<bool(int, int)> canLayGrenadeToBombSoftArea = [this, &dangerous, &obstacle, &hasVisited, &InRange, &xMove, &yMove](int destXc, int destYc)
                        {
                            bool attack = false; 
                            bool stop = false; 
                            if (!InRange(destXc, destYc)) return attack; 
                            for (auto pMapObj : pGame->GetMapObj(destXc, destYc))   //检查手榴弹爆炸位置有没有软障碍
                            {
                                switch (pMapObj->GetObjType())
                                {
                                case obj_base::objType::hardObstacle:
                                    stop = true; break;
                                case obj_base::objType::role:                       //队友在这
                                    if (IsComputer(dynamic_cast<Role*>(pMapObj)->GetID())) stop = true; break;
                                case obj_base::objType::softObstacle:
                                    if (hasVisited[destXc][destYc]) attack = true;
                                    break;
                                }
                                if (stop) { attack = false; break; }
                            }
                            if (!stop)
                            {
                                for (int i = 0; i < 4; ++i)                         //搜索手榴弹爆炸的范围有没有软障碍
                                {
                                    int aroundXc = destXc + xMove[i], aroundYc = destYc + yMove[i];
                                    if (!InRange(aroundXc, aroundYc)) continue;
                                    auto aroundMapObjList = pGame->GetMapObj(aroundXc, aroundYc);
                                    for (auto pAroundMapObj : aroundMapObjList)
                                        if (pAroundMapObj->GetObjType() == obj_base::objType::role && IsComputer(dynamic_cast<Role*>(pAroundMapObj)->GetID()))
                                        {
                                            stop = true; break;
                                        }
                                        else if (pAroundMapObj->GetObjType() == obj_base::objType::softObstacle) { if (hasVisited[aroundXc][aroundYc]) attack = true; }
                                    if (stop) { attack = false; break; }
                                }
                            }
                            return attack; 
                        }; 

                        int destXc = xc, destYc = yc; 
                        switch (pGame->GetRole(roleID)->GetDirect())                //先查看正对的位置有没有软障碍
                        {
                        case obj_base::direction::Up: destXc -= 2; break; 
                        case obj_base::direction::Down: destXc += 2; break; 
                        case obj_base::direction::Left: destYc -= 2; break; 
                        case obj_base::direction::Right: destYc += 2; break; 
                        }
                        if (InRange(destXc, destYc))
                        {
                            attack = canLayGrenadeToBombSoftArea(destXc, destYc); 
                            if (attack) { pGame->LayTnt(roleID); Sleep(1000 / dataFps); }
                        }
                        if (!attack)                                       //没执行攻击，查看四周有没有软障碍
                        {
                            for (int i = 0; i < 4; ++i)
                            {
                                int destXc = xc + xMove[i], destYc = yc + yMove[i];
                                if (!InRange(destXc, destYc) || dangerous[destXc][destYc]) continue;
                                if (obstacle[destXc][destYc]) { destXc += xMove[i], destYc += yMove[i]; }
                                else { destXc += 2 * xMove[i], destYc += 2 * yMove[i]; }
                                if (!InRange(destXc, destYc)) continue;
                                attack = canLayGrenadeToBombSoftArea(destXc, destYc); 
                                if (attack)                                         //如果可以炸到障碍
                                {
                                    if (xMove[i] == -1 && yMove[i] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 1 && yMove[i] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                                    else if (xMove[i] == 0 && yMove[i] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
                                    pGame->LayTnt(roleID); Sleep(1000 / dataFps);
                                    break; 
                                }
                            }
                        }
                        break; 
                    }
                    }
                }
            }
            
            if (!attack && pGame->GetDifficulty() == Game::Difficulty::difficult)
            {
                for (int i = 0; i < (int)unpickedProps.size(); ++i)          //捡道具
                {
                    auto [propXc, propYc] = unpickedProps.front().first; 
                    auto [direct, depth] = unpickedProps.front().second; 
                    if (depth >= 7) break;                              //太远的道具不捡

                    bool find = false; 

                    for (auto pMapObj : pGame->GetMapObj(propXc, propYc))
                    {
                        if (pMapObj->GetObjType() == obj_base::objType::prop)
                        {
                            Prop* pProp = dynamic_cast<Prop*>(pMapObj); 
                            if (pProp->IsUnpicked())
                            {
                                if (pWeapon && pProp->GetPropType() >= Prop::propType::specialbombbegin
                                    && pProp->GetPropType() <= Prop::propType::specialbombend) break;       //已经有武器了，不再捡武器了
                                find = true; 
                            }
                        }
                    }

                    if (!find) continue; 

                    switch (direct)
                    {
                    case Prop::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break; 
                    case Prop::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                    case Prop::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                    case Prop::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                    }
                    attack = true; 
                    break; 
                }
            }

            if (!attack)                                                //找附近的软障碍炸
            {
                if (!pWeapon && pGame->GetRole(roleID)->GetLeftTntNum() > 0) //没有特殊道具，有TNT，看看有没有爆炸范围内的软障碍
                {

                    bool haveLaidMine = false;                          //本格是否有未爆炸的地雷
                    laidMineMutex.lock(); 
                    for (int i = 0; i < (int)laidMine.size(); ++i)
                    {
                        if (laidMine[i].second == false) continue; 
                        if (laidMine[i].first.first == xc && laidMine[i].first.second == yc)
                        {
                            haveLaidMine = true; break; 
                        }
                    }
                    laidMineMutex.unlock(); 

                    if (!haveLaidMine)                                      //无地雷和催泪瓦斯，可以放置
                    {
                        int distance = pGame->GetRole(roleID)->GetDistance();     //获取自己放出炸弹的范围
                        for (int i = 0; i < 4; ++i)                         //查看四周
                        {
                            for (int j = 1; j < distance; ++j)
                            {
                                int bombXc = xc + xMove[i] * j, bombYc = yc + yMove[i] * j;
                                int stop = false;
                                if (!InRange(bombXc, bombYc)) break;
                                for (auto pMapObj : pGame->GetMapObj(bombXc, bombYc))
                                {
                                    switch (pMapObj->GetObjType())
                                    {
                                    case obj_base::objType::softObstacle: attack = true; break;
                                    case obj_base::objType::hardObstacle: stop = true; break;
                                    }
                                    if (attack || stop) break;
                                }
                                if (attack || stop) break;
                            }
                        }

                        bool prevDangerous = dangerous[xc][yc], prevObstacle = obstacle[xc][yc];
                        dangerous[xc][yc] = obstacle[xc][yc] = true;

                        for (int i = 1; i <= 4; ++i)            //找队友，会不会困死队友
                        {
                            if (IsPlayer(i) || i == roleID) continue;
                            auto [teamMateX, teamMateY] = pGame->GetRole(i)->GetPos();
                            int teamMateXc = pGame->PosToCell(teamMateX), teamMateYc = pGame->PosToCell(teamMateY);
                            if (canRunAway(teamMateXc, teamMateYc, xc, yc) == obj_base::direction::Null)  //会困死队友，不能安放
                            {
                                attack = false; break;
                            }
                        }

                        dangerous[xc][yc] = prevDangerous; obstacle[xc][yc] = prevObstacle;

                        if (attack)
                        {
                            switch (canRunAway(xc, yc, xc, yc))
                            {
                            case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                            case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                            default: attack = false; break;
                            }
                        }
                    }
                }

                if (!attack)
                {
                    //如果手中拿的不是地雷或催泪瓦斯或手榴弹
                    if (!(pWeapon && (pWeapon->GetPropType() == Prop::propType::mine 
                        || pWeapon->GetPropType() == Prop::propType::lachrymator 
                        || pWeapon->GetPropType() == Prop::propType::grenade)))
                    {
                        if (!softObstacles.empty())             //去找软障碍
                        {
                            attack = true;
                            for (int i = 0; i < (int)softObstacles.size(); ++i)
                            {
                                auto [obXc, obYc] = softObstacles[i].first; 
                                bool isBeside = false; 
                                for (int j = 0; j < 4; ++j)
                                {
                                    if (xc + xMove[j] == obXc && yc + yMove[j] == obYc)
                                    {
                                        isBeside = true; 
                                        break; 
                                    }
                                }

                                if (isBeside) continue; 

                                switch (softObstacles[i].second.first)
                                {
                                case obj_base::direction::Up: pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Down: pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Left: pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                                case obj_base::direction::Right: pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                                default: attack = false; break;
                                }

                                if (attack) break; 
                            }
                        }
                    }
                }

                if (!attack)                                    //如果无事可做
                {
                    if (!pWeapon && pGame->GetRole(roleID)->GetLeftTntNum() > 0)
                    {
                        attack = true; 

                        bool prevDangerous = dangerous[xc][yc], prevObstacle = obstacle[xc][yc]; 
                        dangerous[xc][yc] = obstacle[xc][yc] = true; 
                        
                        for (int i = 1; i <= 4; ++i)            //找队友，会不会困死队友
                        {
                            if (IsPlayer(i) || i == roleID) continue;
                            auto [teamMateX, teamMateY] = pGame->GetRole(i)->GetPos();
                            int teamMateXc = pGame->PosToCell(teamMateX), teamMateYc = pGame->PosToCell(teamMateY);
                            if (canRunAway(teamMateXc, teamMateYc, xc, yc) == obj_base::direction::Null)  //会困死队友，不能安放
                            {
                                attack = false; break;
                            }
                        }

                        dangerous[xc][yc] = prevDangerous; obstacle[xc][yc] = prevObstacle; 

                        if (attack) switch (canRunAway(xc, yc, xc, yc))
                        {
                        case obj_base::direction::Up: pGame->LayTnt(roleID); pGame->WalkUpOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Down: pGame->LayTnt(roleID); pGame->WalkDownOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Left: pGame->LayTnt(roleID); pGame->WalkLeftOneCell(roleID, 1000 / dataFps); break;
                        case obj_base::direction::Right: pGame->LayTnt(roleID); pGame->WalkRightOneCell(roleID, 1000 / dataFps); break;
                        default: attack = false; break;
                        }
                    }

                    if (!attack)                            //无法放置TNT，随便走一个方向
                    {
                        int directInt = pGame->GetRandom() % 4; 
                        int destXc = xc + xMove[directInt], destYc = yc + yMove[directInt]; 
                        if (InRange(destXc, destYc) && !dangerous[destXc][destYc] && !obstacle[destXc][destYc])
                        {
                            if (xMove[directInt] == -1 && yMove[directInt] == 0) pGame->WalkUpOneCell(roleID, 1000 / dataFps); 
                            else if (xMove[directInt] == 1 && yMove[directInt] == 0) pGame->WalkDownOneCell(roleID, 1000 / dataFps);
                            else if (xMove[directInt] == 0 && yMove[directInt] == -1) pGame->WalkLeftOneCell(roleID, 1000 / dataFps);
                            else if (xMove[directInt] == 0 && yMove[directInt] == 1) pGame->WalkRightOneCell(roleID, 1000 / dataFps);
                            attack = true; 
                        }
                    }
                }
            }
        }
    }

    for (auto& pLaidMineThread : laidMineThread)
    {
        if (pLaidMineThread)
        {
            if (pLaidMineThread->joinable()) pLaidMineThread->join(); 
            delete pLaidMineThread; pLaidMineThread = nullptr; 
        }
    }

}

std::vector<std::pair<std::_tstring, int>> UI::ReadHighScore() const
{
    WIN32_FIND_DATA wfd; 
    HANDLE hFile; 
    std::vector<std::pair<std::_tstring, int>> res;
    std::_tstring name;
    int score;
    std::_tifstream fin; 
    if constexpr (std::is_same<std::_tifstream, std::wifstream>::value) fin.imbue(std::locale("chs"));
    if ((hFile = FindFirstFile(HIGH_SCORE_DIRECTORY, &wfd)) == INVALID_HANDLE_VALUE)
    {
        CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);    //文件夹不存在
        goto endRead; 
    }

    //是文件而不是文件夹
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        if (MessageBox(m_hWnd, TEXT("文件“data”不正确，是否将其删除？\nIt is a wrong that the file \"") HIGH_SCORE_DIRECTORY TEXT("\" exists. Delete it?"),
            c_lpszError, MB_YESNO | MB_ICONERROR) == IDYES)
        {
            DeleteFile(HIGH_SCORE_DIRECTORY); 
            CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);
        }
        goto endRead;
    }
    
    //文件不存在
    if (FindFirstFile(HIGH_SCORE_PATH, &wfd) == INVALID_HANDLE_VALUE) goto endRead;
    fin.open(HIGH_SCORE_PATH, std::ios::in); 
    if (!fin) goto endRead;     //读取失败

    //读取
    for (int i = 0; i < 6; ++i)
    {
        std::getline(fin, name); 
        fin >> score; 
        while (fin.get() != TEXT('\n') && fin); 
        if (!fin) { fin.close(); goto endRead; }   //读取失败
        res.emplace_back(name, score); 
    }
    fin.close(); 

    endRead: 

    if (res.size() != 6 )          //没有存档
    {
        res.clear(); 
        for (int i = 0; i < 6; ++i)
            res.emplace_back(TEXT("未命名"), 0);
    }

    return res; 
}

void UI::SaveHighScore(const std::vector<std::pair<std::_tstring, int>>& scoreList) const
{
    WIN32_FIND_DATA wfd;
    HANDLE hFile;

    std::function<void(void)> saveError = [this]()
    {
        MessageBox(m_hWnd, TEXT("写入存档错误"), c_lpszError, MB_ICONERROR | MB_OK);
    }; 

    if (scoreList.size() != 6)
    {
        saveError(); return; 
    }
    if ((hFile = FindFirstFile(HIGH_SCORE_DIRECTORY, &wfd)) == INVALID_HANDLE_VALUE)
    {
        CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);    //文件夹不存在
    }

    //是文件而不是文件夹
    if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        if (MessageBox(m_hWnd, TEXT("文件“data”不正确，是否将其删除？\nIt is a wrong that the file \"") HIGH_SCORE_DIRECTORY TEXT("\" exists. Delete it?"),
            c_lpszError, MB_YESNO | MB_ICONERROR) == IDYES)
        {
            DeleteFile(HIGH_SCORE_DIRECTORY);
            CreateDirectory(HIGH_SCORE_DIRECTORY, NULL);
        }
        else 
        {
            saveError(); return;
        }
    }

    std::_tofstream fout(HIGH_SCORE_PATH, std::ios::out); 
    if constexpr (std::is_same<std::wofstream, std::_tofstream>::value) fout.imbue(std::locale("chs")); 
    if (!fout)
    {
        saveError(); return;
    }
    for (int i = 0; i < 6; ++i)
        fout << scoreList[i].first << std::endl << scoreList[i].second << std::endl; 
    fout.close(); 
}

void UI::newScore(int numOfPlayer, Game::Difficulty difficulty, int score)
{
    auto highScoreInfo = ReadHighScore(); 
    int pos = 0; 
    switch (difficulty)
    {
    case Game::Difficulty::easy: pos = 0 + (numOfPlayer - 1) * 3; break;
    case Game::Difficulty::medium: pos = 1 + (numOfPlayer - 1) * 3; break;
    case Game::Difficulty::difficult: pos = 2 + (numOfPlayer - 1) * 3; break;
    }
    if (highScoreInfo[pos].second < score)
    {
        inputName.Begin(m_hInst, m_hWnd); 
        highScoreInfo[pos] = std::make_pair(inputName.GetName(), score); 
        SaveHighScore(highScoreInfo); 
    }
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

    switch (programState)
    {
    case programstate::starting: 
    {
        SelectObject(hdcMem, hBmMain); 
        break; 
    }
    case programstate::gaming: 
    case programstate::gamePulsing: 
    case programstate::changeLevel: 
    {
        BitBlt(hdcMem, 0, 0, objSize * 15, objSize * 13, hdcObj, 0, 0, SRCCOPY); 
        if (pGame == nullptr) break; 
        int rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size();
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                auto mapObjList = pGame->GetMapObj(i, j);
                for (auto pMapObj : mapObjList)
                {
                    auto [x, y] = pMapObj->GetPos();
                    int xp = PosToPaint(x), yp = PosToPaint(y);
                    switch (pMapObj->GetObjType())
                    {
                    case obj_base::objType::role:
                    {
                        Role* pRole = dynamic_cast<Role*>(pMapObj);
                        int yGrid = 0;
                        switch (pRole->GetDirect())
                        {
                        case obj_base::direction::Down: yGrid = 0; break;
                        case obj_base::direction::Up: yGrid = 1; break;
                        case obj_base::direction::Left: yGrid = 2; break;
                        case obj_base::direction::Right: yGrid = 3; break;
                        }
                        SelectObject(hdcObj, hBmRole);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize * (pRole->GetID() - 1), yGrid * objSize, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::tnt:
                    {
                        SelectObject(hdcObj, hBmTnt);
                        BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::softObstacle:
                    {
                        int xGrid = 0;
                        if (dynamic_cast<SoftObstacle*>(pMapObj)->GetStyle()) xGrid = 1;
                        else xGrid = 0;
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, 0, xGrid * objSize, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::hardObstacle:
                    {
                        int xGrid = 0;
                        if (dynamic_cast<HardObstacle*>(pMapObj)->GetStyle()) xGrid = 1;
                        else xGrid = 0;
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize, xGrid * objSize, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::bombArea:
                    {
                        BombArea* pBombArea = dynamic_cast<BombArea*>(pMapObj);
                        switch (pBombArea->GetBomb())
                        {
                        case Prop::propType::null:
                            SelectObject(hdcObj, hBmTnt);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        case Prop::propType::fire:
                            SelectObject(hdcObj, hBmFire);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        case Prop::propType::ice:
                            SelectObject(hdcObj, hBmIce);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        case Prop::propType::grenade:
                            SelectObject(hdcObj, hBmGrenade);
                            BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize * 2, 0, SRCCOPY);
                            break;
                        }
                        break;
                    }
                    case obj_base::objType::prop:
                    {
                        Prop* pProp = dynamic_cast<Prop*>(pMapObj);
                        switch (pProp->GetPropType())
                        {
                        case Prop::propType::glove: SelectObject(hdcObj, hBmGlove); goto paintUnpickedProp;
                        case Prop::propType::shield: SelectObject(hdcObj, hBmShield); goto paintUnpickedProp;
                        case Prop::propType::addtnt: SelectObject(hdcObj, hBmAddTnt); goto paintUnpickedProp;
                        case Prop::propType::addlife: SelectObject(hdcObj, hBmAddLife); goto paintUnpickedProp;
                        case Prop::propType::shoe: SelectObject(hdcObj, hBmShoe); goto paintUnpickedProp;
                        case Prop::propType::jinKeLa: SelectObject(hdcObj, hBmJinKeLa); goto paintUnpickedProp;
                        case Prop::propType::fire: SelectObject(hdcObj, hBmFire); goto paintUnpickedProp;
                        case Prop::propType::ice: SelectObject(hdcObj, hBmIce); goto paintUnpickedProp;
                        paintUnpickedProp:
                            if (pProp->IsUnpicked())
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            break;
                        case Prop::propType::mine:
                            if (pProp->IsUnpicked())
                            {
                                SelectObject(hdcObj, hBmMine);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            }
                            else if (pProp->IsLaid() && dynamic_cast<Mine*>(pMapObj)->Visible())
                            {
                                SelectObject(hdcObj, hBmMine);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            }
                            break;
                        case Prop::propType::lachrymator:
                            if (pProp->IsUnpicked())
                            {
                                SelectObject(hdcObj, hBmLachrymator);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            }
                            else if (pProp->IsLaid() && dynamic_cast<Lachrymator*>(pMapObj)->Visible())
                            {
                                SelectObject(hdcObj, hBmLachrymator);
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            }
                            break;
                        case Prop::propType::grenade: SelectObject(hdcObj, hBmGrenade); goto printFlyingProp;
                        case Prop::propType::missile: SelectObject(hdcObj, hBmMissile); goto printFlyingProp;
                        printFlyingProp:
                            if (pProp->IsUnpicked())
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                            else if (pProp->IsLaid())
                                BitBlt(hdcMem, yp + (objSize - propSize) / 2, xp + (objSize - propSize) / 2, propSize, propSize, hdcObj, propSize, 0, SRCCOPY);
                            break;
                        }
                        break;
                    }
                    }
                }
            }
        }

        SelectObject(hdcObj, hBmObstacle); 
        for (int i = cols; i * objSize < mainWndSize.x; ++i)
            for (int j = 0; j < rows; ++j)
                BitBlt(hdcMem, i * objSize, j * objSize, objSize, objSize, hdcObj, objSize, 0, SRCCOPY); 
        
        int fontHeight = 40; 

        //打印关数
        HFONT hF = CreateFont
        (
            fontHeight,
            0,
            0,
            0,
            FW_HEAVY,
            0,
            0,
            0,
            GB2312_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH,
            TEXT("隶书")
        ); 
        HFONT hFOld = (HFONT)SelectObject(hdcMem, hF); 
        SetTextColor(hdcMem, RGB(255, 255, 255)); 
        SetBkColor(hdcMem, RGB(16, 11, 26)); 
        int nowLevel = pGame->GetNowLevel() + 1; 
        std::_tostringstream levelStr; 
        if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) levelStr.imbue(std::locale("chs"));
        levelStr << TEXT("第 ") << nowLevel << TEXT(" 关"); 
        RECT rc = { 15 * objSize, 0, mainWndSize.x, mainWndSize.y };
        DrawText(hdcMem, levelStr.str().c_str(), levelStr.str().length(), &rc, DT_CENTER);

        //打印角色信息
        for (int i = 0; i < 4; ++i)
        {
            const Role* pRole = pGame->GetRole(i + 1); 
            POINT rolePaintPos = { 15 * objSize, fontHeight + i * (mainWndSize.y - fontHeight) / 4 };
            SelectObject(hdcObj, hBmRole); 
            BitBlt(hdcMem, rolePaintPos.x, rolePaintPos.y,
                objSize, objSize, hdcObj, i * objSize, 4 * objSize, SRCCOPY); 
            //拥有武器
            SpecialBomb* pWeapon = pRole->GetWeapon();
            if (pWeapon)
            {
                switch (pWeapon->GetPropType())
                {
                case Prop::propType::lachrymator: SelectObject(hdcObj, hBmLachrymator); goto paintWeapon;
                case Prop::propType::mine: SelectObject(hdcObj, hBmMine); goto paintWeapon;
                case Prop::propType::fire: SelectObject(hdcObj, hBmFire); goto paintWeapon;
                case Prop::propType::ice: SelectObject(hdcObj, hBmIce); goto paintWeapon;
                case Prop::propType::missile: SelectObject(hdcObj, hBmMissile); goto paintWeapon;
                case Prop::propType::grenade: SelectObject(hdcObj, hBmGrenade); goto paintWeapon;
                paintWeapon: BitBlt(hdcMem, rolePaintPos.x + 1 * objSize + (objSize - propSize) / 2, rolePaintPos.y + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, 0, 0, SRCCOPY);
                }
            }
            //手套
            if (pRole->CanPushTnt())
            {
                SelectObject(hdcObj, hBmGlove); 
                BitBlt(hdcMem, rolePaintPos.x + 2 * objSize + (objSize - propSize) / 2, rolePaintPos.y + (objSize - propSize) / 2, 
                propSize, propSize, hdcObj, 0, 0, SRCCOPY); 
            }
            //盾牌
            if (pRole->HaveShield())
            {
                SelectObject(hdcObj, hBmShield);
                BitBlt(hdcMem, rolePaintPos.x + 3 * objSize + (objSize - propSize) / 2, rolePaintPos.y + (objSize - propSize) / 2,
                    propSize, propSize, hdcObj, 0, 0, SRCCOPY);
            }
            
            hF = CreateFont
            (
                fontHeight / 2,
                0,
                0,
                0,
                FW_HEAVY,
                0,
                0,
                0,
                GB2312_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY,
                DEFAULT_PITCH,
                TEXT("隶书")
            );

            DeleteObject(SelectObject(hdcMem, hF)); 

            //生命数
            std::_tostringstream lifeStr;

            if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) lifeStr.imbue(std::locale("chs"));
            
            lifeStr << TEXT("生命：") << pRole->GetLife(); 
            rc.left = rolePaintPos.x + objSize; rc.right = mainWndSize.x; rc.top = rolePaintPos.y + objSize; rc.bottom = rolePaintPos.y + objSize * 2; 
            DrawText(hdcMem, lifeStr.str().c_str(), lifeStr.str().length(), &rc, DT_LEFT); 
            //分数
            std::_tostringstream scoreStr;
            if constexpr (std::is_same<std::_tostringstream, std::wostringstream>::value) scoreStr.imbue(std::locale("chs"));
            scoreStr << TEXT("分数：") << pRole->GetNowScore(); 
            rc.left = rolePaintPos.x + objSize; rc.right = mainWndSize.x; rc.top = rolePaintPos.y + 2 * objSize; rc.bottom = rolePaintPos.y + objSize * 3;
            DrawText(hdcMem, scoreStr.str().c_str(), scoreStr.str().length(), &rc, DT_LEFT);
        }
        SelectObject(hdcMem, hFOld);
        DeleteObject(hF);
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
    
    programState = programstate::starting;

    //等待异步结束
    if (pScanDataTask)
    {
        delete pScanDataTask; pScanDataTask = nullptr;
    }
    if (pRefreshScreenTask)
    {
        delete pRefreshScreenTask; pRefreshScreenTask = nullptr;
    }

    for (auto& pRoleControl : pRoleControlTasks)
    {
        if (pRoleControl)
        {
            delete pRoleControl; pRoleControl = nullptr;
        }
    }

    if (pGame)
    {
        Sleep(800);
        delete pGame;
        pGame = nullptr;
    }
}

bool UI::StartGameDlg::Begin(HINSTANCE hInstance, HWND hWndParent)
{
    Init(hInstance, MAKEINTRESOURCE(STARTGAME_DIALOG), hWndParent); 
    return choose; 
}

void UI::StartGameDlg::MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG: 
        DisableButton(IDC_PLAYER2_RED); 
        DisableButton(IDC_PLAYER2_YELLOW);
        DisableButton(IDC_PLAYER2_GREEN);
        DisableButton(IDC_PLAYER2_BLUE);
        choose = false; 
        break; 
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_SINGLE:
            if (IsButtonCheck(IDC_SINGLE))
            {
                DisableButton(IDC_PLAYER2_RED); DisableButton(IDC_PLAYER2_YELLOW);
                DisableButton(IDC_PLAYER2_GREEN); DisableButton(IDC_PLAYER2_BLUE);
            }
            break;
        case IDC_DOUBLE:
            if (IsButtonCheck(IDC_DOUBLE))
            {
                EnableButton(IDC_PLAYER2_RED); EnableButton(IDC_PLAYER2_YELLOW);
                EnableButton(IDC_PLAYER2_GREEN); EnableButton(IDC_PLAYER2_BLUE);
            }
            break;
        case IDC_PLAYER1_RED: SetUnChecked(IDC_PLAYER2_RED); break; 
        case IDC_PLAYER1_YELLOW: SetUnChecked(IDC_PLAYER2_YELLOW); break;
        case IDC_PLAYER1_GREEN: SetUnChecked(IDC_PLAYER2_GREEN); break;
        case IDC_PLAYER1_BLUE: SetUnChecked(IDC_PLAYER2_BLUE); break;
        case IDC_PLAYER2_RED: SetUnChecked(IDC_PLAYER1_RED); break;
        case IDC_PLAYER2_YELLOW: SetUnChecked(IDC_PLAYER1_YELLOW); break;
        case IDC_PLAYER2_GREEN: SetUnChecked(IDC_PLAYER1_GREEN); break;
        case IDC_PLAYER2_BLUE: SetUnChecked(IDC_PLAYER1_BLUE); break;
        case IDCANCEL: choose = false; EndDialog(hDlg, 0); break; 
        case IDOK: 
            if (IsButtonCheck(IDC_EASY)) difficulty = Difficulty::easy;
            else if (IsButtonCheck(IDC_MEDIUM)) difficulty = Difficulty::medium;
            else if (IsButtonCheck(IDC_DIFFICULT)) difficulty = Difficulty::difficult;
            else { MessageBox(hDlg, TEXT("请选择难度！\nPlease choose difficulty!"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }
            if (IsButtonCheck(IDC_SINGLE)) numOfPlayer = 1;
            else if (IsButtonCheck(IDC_DOUBLE)) numOfPlayer = 2; 
            else { MessageBox(hDlg, TEXT("请选择游戏人数！\nPlease choose the number of players"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }

            if (IsButtonCheck(IDC_PLAYER1_RED)) player1ID = 1;
            else if (IsButtonCheck(IDC_PLAYER1_YELLOW)) player1ID = 2;
            else if (IsButtonCheck(IDC_PLAYER1_GREEN)) player1ID = 3;
            else if (IsButtonCheck(IDC_PLAYER1_BLUE)) player1ID = 4;
            else { MessageBox(hDlg, TEXT("请选择Player1！\nPlease choose Player1"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }

            if (numOfPlayer == 2)
            {
                if (IsButtonCheck(IDC_PLAYER2_RED)) player2ID = 1;
                else if (IsButtonCheck(IDC_PLAYER2_YELLOW)) player2ID = 2;
                else if (IsButtonCheck(IDC_PLAYER2_GREEN)) player2ID = 3;
                else if (IsButtonCheck(IDC_PLAYER2_BLUE)) player2ID = 4;
                else { MessageBox(hDlg, TEXT("请选择Player2！\nPlease choose Player2"), TEXT("Warning"), MB_OK | MB_ICONWARNING); break; }
            }
             
            choose = true; 
            EndDialog(hDlg, 0); 
            break;
        }
        break; 
    }
}

void UI::InputNameDlg::Begin(HINSTANCE hInstance, HWND hWndParent)
{
    Init(hInstance, MAKEINTRESOURCE(INPUTNAME_DIALOG), hWndParent); 
}

void UI::InputNameDlg::MessageProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND: 
        switch (LOWORD(wParam))
        {
        case IDOK: 
        {
            TCHAR nameBuf[51]; 
            GetDlgItemText(hDlg, IDC_NAMEEDIT, nameBuf, 50); 
            name = nameBuf; 
            if (name == TEXT(""))
            {
                MessageBox(hDlg, TEXT("名字不能为空！"), TEXT("Null name"), MB_OK | MB_ICONWARNING); 
                break; 
            }
            EndDialog(hDlg, 0);
            break;
        }
        }
        break; 
    }
}
