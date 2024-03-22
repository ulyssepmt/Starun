#include "game.h"

void DrawEnd(struct GAME *pGame, HWND gameWin, HDC hDC, HDC hDCMemoire, int playerScore) {

    //HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    int gameWinW = pGame->ui.w;

    int gameWinH = pGame->ui.h;
    if(pGame->END == 1) {
    alSourceStop(pGame->sound.gameMusicSource);

    CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0), 0,0, 1000, 1000);

    alSourcePlay(pGame->sound.gameVictorySource);

    SelectObject(hDCMemoire, pGame->earthBitmap);

    for (int z = 0; z < gameWinH - 280; z++) {

        BitBlt(hDC, gameWinW / 2 - 70, z, 130, 130, hDCMemoire, 0, 0, SRCCOPY);
        Sleep(1);
    }
    pGame->player.playerBitmap = (HBITMAP)LoadImage(GetModuleHandle(0),
                                          MAKEINTRESOURCE(IDB_IMAGE),
                                          IMAGE_BITMAP,
                                          0, 0,
                                          LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
    SelectObject(hDCMemoire, pGame->player.playerBitmap);

    //Placement
    int schipX = (gameWinW / 2) - (pGame->player.bSize / 2);

    int schipY = (gameWinH - pGame->player.bSize) - (pGame->player.bSize / 2);

    BitBlt(hDC, schipX, schipY, gameWinW, gameWinH, hDCMemoire, 0, 0, SRCCOPY);

    for (int i = 0; i <= gameWinW; i += 8) {

        EntitiesMask(gameWin, hDC, gameWinW - i, gameWinH - i, schipX, schipY);

        StretchBlt(hDC, schipX, schipY, gameWinW - i, gameWinH - i, hDCMemoire, 0, 0, gameWinW, gameWinH, SRCCOPY);

        Sleep(15);

    }

    ShowEndScore(pGame, hDC, playerScore, 1);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

    printf("\nPilot: Done, let's goooo!\n");

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->earthBitmap);

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->player.playerBitmap);

    }
    else {
            alSourceStop(pGame->sound.gameMusicSource);

            CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0), 0, 0, 600, 600);

            ShowEndScore(pGame, hDC, playerScore, 0);

            alSourcePlay(pGame->sound.gameoverSource);

            CleanUPResources(gameWin, hDC, hDCMemoire, pGame->player.playerBitmap);

            printf("\nPilot: AAAAAAAAAARG!\n");
    }

    pGame->END = -1;

    ClearGameDataSound(pGame, 2000);

    printf("cleared");

    PLAY_AGAIN = TRUE;
}

DWORD WINAPI DrawShots(LPVOID lparam) {

    struct GAME* pGame = (struct GAME *)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    LoadTexture(pGame, &pGame->player.explosionBitmap, IDB_IMAGE5, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_entitie.bmp)");

    LoadTexture(pGame, &pGame->player.playerWeaponBitmap, IDB_IMAGE4, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_entitie.bmp)");
    GetObject(pGame->player.playerWeaponBitmap, sizeof(pGame->player.playerWeaponBitmapInfos), &pGame->player.playerWeaponBitmapInfos);

    SelectObject(hDCMemoire, pGame->player.playerWeaponBitmap);

    int xshot = pGame->player.x;
    int top = -50;

    for(int i = pGame->player.y - 25; i > top  /*bar*/ ; i--) {

        BitBlt(hDC, xshot + 42, i, 20, 20, hDCMemoire, 0, 0, SRCCOPY); // bitmap tirs

        if(CheckCollision(xshot, i, pGame->player.playerWeaponBitmapInfos.bmWidth, pGame->player.playerWeaponBitmapInfos.bmHeight,
                pGame->alien[pGame->event].x, pGame->alien[pGame->event].y, 90, 95))  {

                pGame->player.shotCollision = TRUE;

                alSourcePlay(pGame->sound.gameExplosionSource);

                SelectObject(hDCMemoire, pGame->player.explosionBitmap);

                BitBlt(hDC, pGame->alien[pGame->event].x ,pGame->alien[pGame->event].y, 120, 120, hDCMemoire, 0, 0, SRCCOPY);

                //CleanUpGameScreen(gameWin, 0, 0, pGame->ui.w, 490);

                break;


           }
           if (pGame->boss.bossActivation && !pGame->boss.collisionDetected && CheckCollision(xshot, i, pGame->player.playerWeaponBitmapInfos.bmWidth, pGame->player.playerWeaponBitmapInfos.bmHeight,
                pGame->boss.x, pGame->boss.y, pGame->boss.bossBitmapInfos.bmWidth, pGame->boss.bossBitmapInfos.bmHeight))  {

                top = pGame->boss.y + 160;

                alSourcePlay(pGame->sound.gameExplosionSource);

                SelectObject(hDCMemoire, pGame->player.explosionBitmap);

                pGame->boss.heartPoint--;

                BitBlt(hDC, pGame->boss.x , pGame->boss.y, 120, 120, hDCMemoire, 0, 0, SRCCOPY);

                Sleep(400);

                CleanUpGameScreen(gameWin, 0, 0, pGame->ui.w, 490);

                pGame->boss.collisionDetected = TRUE;

           }

        BitBlt(hDC, xshot + 42, i + 20, 5, 10, hDCMemoire, 0, 0, BLACKNESS); //mask shots

        Sleep(1);


    }

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->player.playerWeaponBitmap);

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->player.explosionBitmap);

    return 0;

}

DWORD WINAPI DrawPlayer(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    HANDLE AsyncShots;

    //printf("w=%d, h=%d", playerInfos.bmWidth, playerInfos.bmHeight);

    if (!gameWin) {

        ShowGameError("[FATAL ERROR] Can't find the specified window ('Starun')");
    }

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    HDC hDCheartPoint = CreateCompatibleDC(hDC);

    SelectObject(hDCMemoire, pGame->player.playerBitmap);

    int gameWinW = pGame->ui.w;

    int gameWinH = pGame->ui.h;

    int marge = 50;

    while (ANIMATION) {

            pGame->player.x = (gameWinW / 2) - (pGame->player.bSize / 2) + pGame->player.direction;

            pGame->player.y = (gameWinH - pGame->player.bSize) - (pGame->player.bSize / 2);

            BitBlt(hDC, pGame->player.x, pGame->player.y, pGame->player.playerBitmapInfos.bmWidth, pGame->player.playerBitmapInfos.bmHeight, hDCMemoire, 0, 0, SRCCOPY);

            //printf("spaceshipX = %d, spaceshipY = %d\n", pGame->spaceship.x, pGame->spaceship.y);
            Sleep(5);

            if(GetAsyncKeyState(VK_LEFT) != 0) {

                pGame->player.direction -= pGame->player.speed;

                EntitiesMask(gameWin, hDC, 100, gameWinH,(gameWinW/ 2) - (pGame->player.direction*(-1)), (gameWinH- 100) - (100 / 2));

                if (pGame->player.direction <= -(gameWinW /2) + marge) {

                pGame->player.direction += pGame->player.speed;

                }
            }

            else if(GetAsyncKeyState(VK_RIGHT) != 0) {

                pGame->player.direction += pGame->player.speed;

                EntitiesMask(gameWin, hDC, 200, gameWinH, pGame->player.direction + 115, (gameWinH- 100) - (100 / 2));

                if (pGame->player.direction >= (gameWinW/ 2) - marge) {

                    pGame->player.direction -= pGame->player.speed;

                }
            }
            else if(GetAsyncKeyState(0x58) != 0 && pGame->player.firerate)  { //X

                    pGame->player.firerate = FALSE;

                    AsyncShots = CreateThread(0, 0, DrawShots, (LPVOID)pGame, 0, 0);

                    WaitForSingleObject(AsyncShots, 150);

                    CloseHandle(AsyncShots);

            }

            DrawPlayerLife(pGame, gameWin, hDC, hDCheartPoint);


             //PlayerState(pGame, gameWin, hDC, hDCMemoire, hDCheartPoint);

    }

    return 0;
}


DWORD WINAPI DrawAliens(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    if (!gameWin) {

        ShowGameError("[FATAL ERROR] Can't find the specified window ('Starun')");
    }

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    SelectObject(hDCMemoire, pGame->alien->alienBitmap);

    //BitBlt(hDCMemoire, 0, 0, 0, 0, hDC, 0, 0, SRCCOPY); // capture

    //pGame->alien->speed = 2.0;

    int aliens[8] = {0, 85, 185, 285, 385, 485, 585, 675};

    srand (time(NULL));

    for (int i = 0; i < NBR_ALIEN; i++) {

            //pGame->player.dataAccess = FALSE;

            pGame->alien[i].x = rand() % (pGame->ui.w - 80);

            pGame->alien[i].y = 0;

            pGame->alien[i].type = aliens[pGame->alien->lockType];


            for (int k = 0; k < pGame->ui.h - 10; k += pGame->alien->speed) {

                pGame->alien[i].y = k;

                if(pGame->player.shotCollision) {

                    break;

                }


                BitBlt(hDC, pGame->alien[i].x, pGame->alien[i].y, 90, 95, hDCMemoire, 0, pGame->alien[i].type, SRCCOPY);

                EntitiesMask(gameWin, hDC, 90, 95, pGame->alien[i].x, pGame->alien[i].y - 90);

                Sleep(5);

                //pGame->player.dataAccess = TRUE;

            }

            pGame->player.shotCollision = FALSE;

            pGame->player.firerate = TRUE;

            pGame->collisionDetected = FALSE;

            pGame->event += 1;

            pGame->alien[i].x = 0;

            pGame->alien[i].y = 0;

            if(!ANIMATION_ALIEN) {

                pGame->player.gamescore  = -1;

                break;
            }

            pGame->player.gamescore += 10;


        }

        CleanUPResources(gameWin, hDC, hDCMemoire, pGame->alien->alienBitmap);

        CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0), 0,0, 1000, 1000);


        return 0;
}

DWORD WINAPI DrawBoss(LPVOID lparam) {

    struct GAME *pGame = (struct GAME*)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);


    if (!gameWin) {

        ShowGameError("[FATAL ERROR] Can't find the specified window ('Starun')");
    }

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    SelectObject(hDCMemoire, pGame->boss.bossBitmap);

    int distance = 0;

    pGame->boss.x = 20;

    ANIMATION_ALIEN = FALSE;

    CleanUpGameScreen(gameWin, 0, 0, pGame->ui.w, pGame->ui.h);

    alSourcePlay(pGame->sound.gameBossSource);

    while(pGame->boss.heartPoint > 0) {

        SelectObject(hDCMemoire, pGame->boss.bossBitmap);

        distance = rand() % (400);

        int step = (pGame->boss.x < distance) ? 1 : -1;

        while (pGame->boss.x != distance) {

            DrawBossLife(pGame, hDC);

            BitBlt(hDC, pGame->boss.x, pGame->boss.y /*20*/, 130, 130, hDCMemoire, 0, 0, SRCCOPY);

            pGame->boss.x += step;

            Sleep(1);
        }

        pGame->boss.bossShotX = pGame->boss.x + 55;

        for(pGame->boss.bossShotY = pGame->boss.y + 160; pGame->boss.bossShotY < pGame->ui.h; pGame->boss.bossShotY+=2) {

            SelectObject(hDCMemoire, pGame->boss.bossWeaponBitmap);

            BitBlt(hDC, pGame->boss.bossShotX, pGame->boss.bossShotY, 40, 40, hDCMemoire, 0, 0, SRCCOPY);

            EntitiesMask(gameWin, hDC, 50, 50, pGame->boss.bossShotX, pGame->boss.bossShotY - 30);


            Sleep(2);

        }

        pGame->boss.damage = FALSE;

        pGame->boss.collisionDetected = FALSE;

        pGame->player.firerate = TRUE;

    }

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->boss.bossBitmap);

    DeleteObject(pGame->boss.bossWeaponBitmap);

    ANIMATION = FALSE;


    if(pGame->boss.heartPoint == 0 && pGame->player.heartPoint > 0) {

        pGame->END = 1; //Win
    }

    return 0;
}

void DrawBossLife(struct GAME *pGame, HDC hDC) {

    char buffer[20];

    SetTextColor(hDC, RGB(255, 255, 255));

    SetBkColor(hDC, RGB(127, 0, 255));

    wsprintf(buffer, "[BOSS] Boss HP : %d", pGame->boss.heartPoint);

    TextOut(hDC, 0, 0, buffer, lstrlen(buffer));

}

int PlayerLife(struct GAME *pGame) {


   if(!pGame->collisionDetected && CheckCollision(pGame->player.x,

                               pGame->player.y, pGame->player.playerBitmapInfos.bmWidth, pGame->player.playerBitmapInfos.bmHeight,

                               pGame->alien[pGame->event].x, pGame->alien[pGame->event].y, 90, 95)) {


        pGame->player.heartPoint--;

        pGame->player.gamescore -= 20;

        pGame->collisionDetected = true;

        alSourcePlay(pGame->sound.gameEatingSource);
    }

    if(pGame->boss.bossActivation && !pGame->boss.damage && CheckCollision(pGame->boss.bossShotX, pGame->boss.bossShotY, pGame->boss.bossWeaponBitmapInfos.bmWidth, pGame->boss.bossWeaponBitmapInfos.bmHeight,
                              pGame->player.x, pGame->player.y, pGame->player.playerBitmapInfos.bmWidth, pGame->player.playerBitmapInfos.bmHeight)) {

                                pGame->player.heartPoint--;

                                pGame->boss.damage = TRUE;

            }

    return pGame->player.heartPoint;
}

void DrawPlayerLife(struct GAME *pGame, HWND gameWin, HDC hDC, HDC hDCMemoire) {

        /* Gestion PV Joueur */

    SelectObject(hDCMemoire, pGame->player.fullLifeBitmap);

    if(PlayerLife(pGame) == 2) {

       SelectObject(hDCMemoire, pGame->player.midLifeBitmap);
    }

    else if(PlayerLife(pGame) == 1) {

       SelectObject(hDCMemoire, pGame->player.lowLifeBitmap);
    }

    else if(PlayerLife(pGame) == 0) { // GameOver

            ANIMATION_ALIEN = FALSE;

            pGame->boss.heartPoint = 0;

            pGame->END = -1;

            CleanUPResources(gameWin, 0, hDCMemoire, pGame->player.fullLifeBitmap);

            CleanUPResources(gameWin, 0, hDCMemoire, pGame->player.midLifeBitmap);

            CleanUPResources(gameWin, 0, hDCMemoire, pGame->player.lowLifeBitmap);

    }

    BitBlt(hDC, 390, 0, 100, 100,hDCMemoire, 0, 0, SRCCOPY);
}



void DrawGameBackground(struct GAME* pGame) {

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    //LoadTexture(pGame, &pGame->ui.bgkBitmap, IDB_IMAGE11, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_entitie.bmp)");

    SelectObject(hDCMemoire, pGame->ui.backgroundBitmap);

    BitBlt(hDC, 0, 30, pGame->ui.w, pGame->ui.h, hDCMemoire, 0, 0, SRCCOPY);

    CleanUPResources(gameWin, hDC, hDCMemoire, NULL);

    Sleep(250);
}




/*DWORD WINAPI DrawCoin(LPVOID lparam) { // BONUS

    struct GAME *pGame = (struct GAME *)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    if (!gameWin) {

        ShowGameError("[FATAL ERROR] Can't find the specified window ('Starun')");
    }
    HBITMAP hbm = (HBITMAP)LoadImage(GetModuleHandle(0),
                              MAKEINTRESOURCE(IDB_IMAGE9),
                              IMAGE_BITMAP,
                              0, 0,
                              LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    SelectObject(hDCMemoire, hbm);

    int x = 0;
    while(ANIMATION) {
     x = rand() % pGame->ui.w;
    for(int i = 0; i < pGame->ui.h; i++) {

        BitBlt(hDC, x, i, 90, 95, hDCMemoire, 0, 0, SRCCOPY);
        EntitiesMask(gameWin, hDC, 90, 95, x, i - 90);
        Sleep(1);
        if(CheckCollision(pGame->spaceshipBitmap, pGame->player.x,
                               pGame->player.y, hbm,
                               x, i)) {
                printf("coined\n!");
        }

    }
    Sleep(1000);
}
}*/

/*void CheckCollision(int Ax, int Ay, int Sx, int Sy, int Ssize) {
 //printf("Alien [%d] : x = %d, y = %d, type = %d\n", i, Ax, A, pGame->alien[i].type);
    if ((Ay > Sy + 20) &&
        ((Ax + 100 > (Sx - Ssize)) &&
             (Ax + 100 < (Sx + Ssize)))) {

            ANIMATION = FALSE;
            MessageBox(0, "GAME OVER", "", MB_OK | MB_TOPMOST);

        }
}*/


