#include "UI.h"

const int UI::objSize = 40;
const int UI::propSize = 30;
const POINT UI::mainWndPos = { 0, 0 };
const POINT UI::mainWndSize = { (objSize * 15) + 200, objSize * 13 }; 
const int UI::dataFps = 50; 
const int UI::paintFps = 50;

int UI::Begin(HINSTANCE hInstance, int nCmdShow)
{

    //加载位图
    LoadGameImg();

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

    programState = programstate::gaming;
    pGame = new Game(2, 1, 2);
    pGame->InitNewLevel(0, true);
    std::thread thr1(&UI::ScanData, this); 
    std::thread thr2(&UI::RoleControl, this, 1);
    std::thread thr3(&UI::RefreshScreen, this); 
    std::thread thr4(&UI::RoleControl, this, 2);
    std::thread thr5(&UI::AI, this, 3); 
    std::thread thr6(&UI::AI, this, 4);
    thr1.detach(); thr2.detach(); 
    thr3.detach(); thr4.detach(); 
    thr5.detach(); thr6.detach();

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
    case WM_KEYDOWN: 
        switch (wParam)
        {
        case VK_SPACE: if (programState == programstate::gaming) playerLay[0] = true; break; 
        case VK_RETURN: if (programState == programstate::gaming) playerLay[1] = true; break;
        }
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
    hBmMissile = (HBITMAP)LoadImage(m_hInst, TEXT("image\\missil.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (!hBmBkgnd || !hBmRole || !hBmTnt || !hBmObstacle || !hBmGlove || !hBmShield | !hBmAddTnt || !hBmAddLife || !hBmShoe 
        || !hBmJinKeLa || !hBmLachrymator || !hBmMine || !hBmFire || !hBmIce || !hBmGrenade || !hBmMissile)
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
    GetObject(hBmMissile, sizeof(BITMAP), &bmMissile);
	return true; 
}

void UI::CreateBuffer(HWND hWnd)
{
    HDC hdc = GetDC(hWnd);
    hBmMem = CreateCompatibleBitmap(hdc, mainWndSize.x, mainWndSize.y); 
    ReleaseDC(hWnd, hdc); 
}

void UI::ScanData()
{
    while (programState == programstate::gaming)
    {
        pGame->CheckRole(); 
        pGame->CheckBomb(1000 / dataFps); 
        Sleep(1000 / dataFps); 
        if (pGame->CheckGameEnd()) EndGame(); 
    }
}

void UI::RoleControl(int player)
{
    if (player == 1)
    {
        playerLay[0] = false; 
        while (programState == programstate::gaming)
        {
            if (GetKeyState('W') < 0) pGame->WalkUpOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('S') < 0) pGame->WalkDownOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('A') < 0) pGame->WalkLeftOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (GetKeyState('D') < 0) pGame->WalkRightOneCell(pGame->GetID1(), 1000 / dataFps);
            else if (playerLay[0]) pGame->LayTnt(pGame->GetID1()); 
            playerLay[0] = false; 
            Sleep(1000 / dataFps); 
        }
    }
    else if (player == 2)
    {
        playerLay[1] = false;
        while (programState == programstate::gaming)
        {
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
    while (programState == programstate::gaming)
    {
        InvalidateRect(m_hWnd, NULL, FALSE); 
        Sleep(1000 / paintFps); 
    }
}

void UI::EndGame()
{
    programState = programstate::gamePulsing; 
    MessageBox(m_hWnd, TEXT("游戏结束"), NULL, MB_OK); 
    Sleep(800); 
    delete pGame; 
    pGame = nullptr; 
    programState = programstate::starting; 
}

void UI::AI(int roleID)
{
    int rows, cols; 
    if (pGame) rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size(); 
    else return; 

    //判断是否越界
    std::function<bool(int, int)> InRange = [rows, cols](int xc, int yc) { return xc >= 0 && yc >= 0 && xc < rows&& yc < cols; }; 

    while (programState == programstate::gaming || programState == programstate::gamePulsing)               //游戏还没结束
    {
        while (programState == programstate::gamePulsing) Sleep(dataFps);                                   //游戏暂停状态，停止
        if (!(programState == programstate::gaming || programState == programstate::gamePulsing)) break;    //游戏结束了，停止
        
        //获取角色位置
        auto [x, y] = pGame->GetRole(roleID)->GetPos(); 
        int xc = pGame->PosToCell(x), yc = pGame->PosToCell(y); 
        
        //检查周围的危险
        std::vector<std::vector<bool>> dangerous(rows, std::vector<bool>(cols, false));                     //记录是否有危险
        std::vector<std::vector<bool>> obstacle(rows, std::vector<bool>(cols, false));                      //记录是否有障碍
        const std::vector<int> xMove{ -1, 1, 0, 0 }; 
        const std::vector<int> yMove{ 0, 0, -1, 1 }; 
        for (int tmpXc = 0; tmpXc < rows; ++tmpXc)
            for (int tmpYc = 0; tmpYc < cols; ++tmpYc)
            {
                auto mapObjList = pGame->GetMapObj(tmpXc, tmpYc);
                for (auto pObj : mapObjList)
                {
                    switch (pObj->GetObjType())
                    {
                    case obj_base::objType::tnt:                                                            //如果是TNT
                    {
                        dangerous[tmpXc][tmpYc] = true;                                                     //标记为危险区
                        obstacle[tmpXc][tmpYc] = true;                                                      //不能踩上该点
                        int distance = dynamic_cast<TNT*>(pObj)->GetDistance();
                        for (int i = 0; i < 4; ++i)                                                         //向四周窥探
                        {
                            for (int j = 1; j < distance; ++j)
                            {
                                int newXc = tmpXc + j * xMove[i], newYc = tmpYc + j * yMove[i];
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
                        case Prop::propType::mine:                                                          //地雷和催泪瓦斯，如果可见则绝对不能踩中
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
            std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>> nearestUnpickedProp(std::make_pair(0, 0), std::make_pair(obj_base::direction::Null, 0));
            std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>> nearestEnemy(std::make_pair(0, 0), std::make_pair(obj_base::direction::Null, 0));
            std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>> nearestSoftObstacle(std::make_pair(0, 0), std::make_pair(obj_base::direction::Null, 0));
            std::queue<std::pair<std::pair<int, int>, std::pair<obj_base::direction, int>>> q; 
            std::vector<std::vector<bool>> hasVisited(rows, std::vector<bool>(cols, false)); 

            //开始广搜
            if (InRange(xc - 1, yc) && !obstacle[xc - 1][yc] && !dangerous[xc - 1][yc])
            {
                q.emplace(std::make_pair(xc - 1, yc), std::make_pair(obj_base::direction::Up, 0)); hasVisited[xc - 1][yc] = true;
            }
            if (InRange(xc + 1, yc) && !obstacle[xc + 1][yc] && !dangerous[xc + 1][yc])
            {
                q.emplace(std::make_pair(xc + 1, yc), std::make_pair(obj_base::direction::Down, 0)); hasVisited[xc + 1][yc] = true;
            }
            if (InRange(xc, yc - 1) && !obstacle[xc][yc - 1] && !dangerous[xc][yc - 1])
            {
                q.emplace(std::make_pair(xc, yc - 1), std::make_pair(obj_base::direction::Left, 0)); hasVisited[xc][yc - 1] = true;
            }
            if (InRange(xc, yc + 1) && !obstacle[xc][yc + 1] && !dangerous[xc][yc + 1])
            {
                q.emplace(std::make_pair(xc, yc + 1), std::make_pair(obj_base::direction::Right, 0)); hasVisited[xc][yc + 1] = true;
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
                    case obj_base::objType::role:                                                           //如果是敌人，录进去
                    {
                        if (nearestEnemy.second.first != obj_base::direction::Null) break; 
                        if (IsComputer(dynamic_cast<Role*>(pMapObj)->GetID())) break; 
                        nearestEnemy.first = std::make_pair(frontXc, frontYc); 
                        nearestEnemy.second = std::make_pair(direct, depth); 
                        break; 
                    }
                    case obj_base::objType::softObstacle:                                                   //录入可达的软障碍
                    {
                        if (nearestSoftObstacle.second.first != obj_base::direction::Null) break; 
                        bool canArrive = false; 
                        for (int i = 0; i < 4; ++i)
                        {
                            int aroundXc = frontXc + xMove[i], aroundYc = frontYc + yMove[i];
                            if (!InRange(aroundXc, aroundYc) || dangerous[aroundXc][aroundYc] || obstacle[aroundXc][aroundYc]) continue; 
                            canArrive = true; 
                            break; 
                        }
                        if (canArrive) 
                        {
                            nearestSoftObstacle.first = std::make_pair(frontXc, frontYc);
                            nearestEnemy.second = std::make_pair(direct, depth);
                        }
                        break; 
                    }
                    case obj_base::objType::prop:                                                           //可以捡起的道具
                    {
                        if (nearestUnpickedProp.second.first != obj_base::direction::Null) break;
                        if (!dynamic_cast<Prop*>(pMapObj)->IsUnpicked()) break; 
                        bool canArrive = false;
                        for (int i = 0; i < 4; ++i)
                        {
                            int aroundXc = frontXc + xMove[i], aroundYc = frontYc + yMove[i];
                            if (!InRange(aroundXc, aroundYc) || dangerous[aroundXc][aroundYc] || obstacle[aroundXc][aroundYc]) continue;
                            canArrive = true;
                            break;
                        }
                        if (canArrive) 
                        {
                            nearestSoftObstacle.first = std::make_pair(frontXc, frontYc);
                            nearestEnemy.second = std::make_pair(direct, depth);
                        }
                        break; 
                    }
                    }
                }
                for (int i = 0; i < 4; ++i)                                                                 //向四周遍历
                {
                    int nextXc = frontXc + xMove[i], nextYc = frontYc + yMove[i];
                    if (InRange(nextXc, nextYc) && !hasVisited[nextXc][nextYc] && !obstacle[nextXc][nextYc] && !dangerous[nextXc][nextYc])
                    {
                        q.emplace(std::make_pair(nextXc, nextYc), std::make_pair(direct, depth + 1));
                        hasVisited[nextXc][nextYc] = true;
                    }
                }
            }


        }
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
    case programstate::gaming: 
    case programstate::gamePulsing: 
    {
        BitBlt(hdcMem, 0, 0, objSize * 15, objSize * 13, hdcObj, 0, 0, SRCCOPY); 
        if (pGame == nullptr) break; 
        int rows = pGame->GetGameMap(pGame->GetNowLevel()).size(), cols = pGame->GetGameMap(pGame->GetNowLevel())[0].size();
        for (int i = 0; i < rows; ++i)
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
                        SelectObject(hdcObj, hBmRole);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize * (pRole->GetID() - 1), 0, SRCCOPY);
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
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, 0, 0, SRCCOPY);
                        break;
                    }
                    case obj_base::objType::hardObstacle:
                    {
                        SelectObject(hdcObj, hBmObstacle);
                        BitBlt(hdcMem, yp, xp, objSize, objSize, hdcObj, objSize, 0, SRCCOPY);
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
    if (pGame)
    {
        programState = programstate::starting;
        Sleep(800);
        delete pGame;
        pGame = nullptr;
    }
}
