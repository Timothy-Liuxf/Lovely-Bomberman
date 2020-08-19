/////////////////////////////////////////
//
// 放置已经删除的代码
//

//
//for (int i = 0; i < 13; ++i)
//    for (int j = 0; j < 15; ++j)
//    {
//        if (i == j) continue;
//        int roleNum = (i * 15 + j) % 4;
//        SelectObject(hdcObj, hBmRole);
//        //BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * roleNum, 0, SRCCOPY);
//    }
//for (int i = 0; i < 13; ++i)
//    for (int j = 0; j < 15; ++j)
//    {
//        if (i != j && i != 13 - j) continue;
//        if (i % 2)
//        {
//            SelectObject(hdcObj, hBmTnt);
//            /*BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, 0, 0, SRCCOPY); */
//        }
//        else
//        {
//            SelectObject(hdcObj, hBmTnt);
//            /*BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * 1, 0, SRCCOPY);*/
//        }
//    }
///*for (int i = 0; i < 13; ++i)
//    for (int j = 0; j < 15; ++j)
//    {
//        if (i % 3) continue;
//        SelectObject(hdcObj, hBmObstacle);
//        if (j % 3 == 0)
//            BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, 0, 0, SRCCOPY);
//        else if (j % 3 == 1) BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize, 0, SRCCOPY);
//        else
//        {
//            SelectObject(hdcObj, hBmLachrymator);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * 1, 0, SRCCOPY);
//        }
//    }*/
//for (int i = 0; i < 13; ++i)
//    for (int j = 0; j < 15; ++j)
//    {
//        int tmp = rand() % 16;
//        switch (tmp)
//        {
//        case 0:
//            SelectObject(hdcObj, hBmRole);
//            BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * (rand() % 4), 0, SRCCOPY);
//            break;
//        case 1:
//            SelectObject(hdcObj, hBmObstacle);
//            BitBlt(hdcMem, j * objSize, i * objSize, objSize, objSize, hdcObj, objSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 2:
//            SelectObject(hdcObj, hBmTnt);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 3:
//            SelectObject(hdcObj, hBmGlove);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
//            break;
//        case 4:
//            SelectObject(hdcObj, hBmShield);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
//            break;
//        case 5:
//            SelectObject(hdcObj, hBmAddTnt);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
//            break;
//        case 6:
//            SelectObject(hdcObj, hBmAddLife);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
//            break;
//        case 7:
//            SelectObject(hdcObj, hBmShoe);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
//            break;
//        case 8:
//            SelectObject(hdcObj, hBmJinKeLa);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 1), 0, SRCCOPY);
//            break;
//        case 9:
//            SelectObject(hdcObj, hBmLachrymator);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 10:
//            SelectObject(hdcObj, hBmMine);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 11:
//            SelectObject(hdcObj, hBmFire);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 12:
//            SelectObject(hdcObj, hBmIce);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 13:
//            SelectObject(hdcObj, hBmGrenade);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 3), 0, SRCCOPY);
//            break;
//        case 14:
//            SelectObject(hdcObj, hBmMissile);
//            BitBlt(hdcMem, j * objSize + (objSize - propSize) / 2, i * objSize + (objSize - propSize) / 2,
//                propSize, propSize, hdcObj, propSize * (rand() % 2), 0, SRCCOPY);
//            break;
//        case 15:
//            break;
//
//        }
//    }



    //[[maybe_unused]] int playerid1 = 3, playerid2 = 0, computerid1 = 1, computerid2 = 2, computerid3 = 4;

    //pGame = new Game(1, playerid1, playerid2, Game::Difficulty::easy);
    //pGame->InitNewLevel(0, true);

    //pScanDataTask = new std::future<void>(std::async(&UI::ScanData, this)); 
    //pRefreshScreenTask = new std::future<void>(std::async(&UI::RefreshScreen, this));

    ////开启两个角色线程
    //pRoleControlTasks[1] = new std::future<void>(std::async(&UI::RoleControl, this, playerid1));    //角色1
    //pRoleControlTasks[2] = new std::future<void>(std::async(&UI::AI, this, computerid3));    //角色2
    //pRoleControlTasks[3] = new std::future<void>(std::async(&UI::AI, this, computerid1));
    //pRoleControlTasks[4] = new std::future<void>(std::async(&UI::AI, this, computerid2));

    /*case WM_TIMER:
        if (wParam == TIMER_ID_START)
        {
            InvalidateRect(hWnd, NULL, TRUE);
            KillTimer(hWnd, TIMER_ID_START);
        }
        else
        {
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break; */
