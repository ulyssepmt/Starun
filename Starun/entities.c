#include "game.h"

BOOL test = NULL;

void DrawEnd(struct GAME *pGame, HBITMAP spaceship, HWND gameWindow, HDC hDC, HDC hDCMemoire, int playerScore) {

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    int gameWinW = pGame->ui.w;

    int gameWinH = pGame->ui.h;

    if(pGame->END) {

    alSourceStop(pGame->sound.gameMusicSource);

    CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0), 0,0, 1000, 1000);

    alSourcePlay(pGame->sound.gameVictorySource);

    SelectObject(hDCMemoire, pGame->earthBitmap);

    for (int z = 0; z < gameWinH - 280; z++) {

        BitBlt(hDC, gameWinW / 2 - 70, z, 130, 130, hDCMemoire, 0, 0, SRCCOPY);

        Sleep(1);
    }
    pGame->spaceshipBitmap = (HBITMAP)LoadImage(GetModuleHandle(0),
                                          MAKEINTRESOURCE(IDB_IMAGE),
                                          IMAGE_BITMAP,
                                          0, 0,
                                          LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
    SelectObject(hDCMemoire, pGame->spaceshipBitmap);

    //Placement
    int schipX = (gameWinW / 2) - (pGame->player.bSize / 2);

    int schipY = (gameWinH - pGame->player.bSize) - (pGame->player.bSize / 2);

    BitBlt(hDC, schipX, schipY, gameWinW, gameWinH, hDCMemoire, 0, 0, SRCCOPY);

    for (int i = 0; i <= gameWinW; i += 8) {

        EntitiesMask(gameWindow, hDC, gameWinW - i, gameWinH - i, schipX, schipY);

        StretchBlt(hDC, schipX, schipY, gameWinW - i, gameWinH - i, hDCMemoire, 0, 0, gameWinW, gameWinH, SRCCOPY);

        Sleep(15);

    }

    ShowEndScore(pGame, hDC, playerScore, 1);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

    printf("\nPilot: Done, let's goooo!\n");

    CleanUPResources(gameWindow, hDC, hDCMemoire, pGame->earthBitmap);

    CleanUPResources(gameWindow, hDC, hDCMemoire, pGame->spaceshipBitmap);

    }
    else {
           alSourceStop(pGame->sound.gameMusicSource);

            CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0), 0, 0, 600, 600);

            ShowEndScore(pGame, hDC, playerScore, 0);

            alSourcePlay(pGame->sound.gameoverSource);

            CleanUPResources(gameWin, hDC, hDCMemoire, pGame->spaceshipBitmap);

            printf("\nPilot: AAAAAAAAAARG!\n");
    }

    ClearGameDataSound(pGame, 2000);

    PLAY_AGAIN = TRUE;
}

DWORD WINAPI DrawShots(LPVOID lparam) {

    struct GAME* pGame = (struct GAME *)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    LoadTexture(pGame, &pGame->explosionBitmap, IDB_IMAGE5, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_entitie.bmp)");

    LoadTexture(pGame, &pGame->weaponBitmap, IDB_IMAGE4, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_entitie.bmp)");

    SelectObject(hDCMemoire, pGame->weaponBitmap);

    int xshot = pGame->player.x;

    for(int i = pGame->player.y - 25; i > -50  /*bar*/ ; i--) {

        BitBlt(hDC, xshot + 42, i, 5, 10, hDCMemoire, 0, 0, SRCCOPY); // bitmap tirs

        if(CheckCollision(pGame->weaponBitmap, xshot, i, pGame->alienBitmap,
                pGame->alien[pGame->event].x, pGame->alien[pGame->event].y))  {

                pGame->player.shotCollision = TRUE;

                alSourcePlay(pGame->sound.gameExplosionSource);

                SelectObject(hDCMemoire, pGame->explosionBitmap);

                BitBlt(hDC, pGame->alien[pGame->event].x ,pGame->alien[pGame->event].y, 120, 120, hDCMemoire, 0, 0, SRCCOPY);

                break;


           }

        BitBlt(hDC, xshot + 42, i + 20, 5, 10, hDCMemoire, 0, 0, BLACKNESS); //mask shots

        Sleep(1);


    }

    Sleep(150);

    CleanUpGameScreen(gameWin, 0, 0, pGame->ui.w, 490);

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->weaponBitmap);

    CleanUPResources(gameWin, hDC, hDCMemoire, pGame->explosionBitmap);

    test = FALSE;

    return 0;

}

DWORD WINAPI DrawPlayer(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    HANDLE AsyncShots;

    if (!gameWin) {

        ShowGameError("[FATAL ERROR] Can't find the specified window ('Starun')");
    }

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    HDC hDCheartPoint = CreateCompatibleDC(hDC);

    SelectObject(hDCMemoire, pGame->spaceshipBitmap);

    int gameWinW = pGame->ui.w;

    int gameWinH = pGame->ui.h;

    int marge = 50;

    while (ANIMATION) {

            pGame->player.x = (gameWinW / 2) - (pGame->player.bSize / 2) + pGame->player.direction;

            pGame->player.y = (gameWinH - pGame->player.bSize) - (pGame->player.bSize / 2);

            BitBlt(hDC, pGame->player.x, pGame->player.y, gameWinW, gameWinH, hDCMemoire, 0, 0, SRCCOPY);

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

             PlayerState(pGame, gameWin, hDC, hDCMemoire, hDCheartPoint);

    }


            DrawEnd(pGame, pGame->spaceshipBitmap, gameWin, hDC, hDCMemoire, pGame->player.gamescore);


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

    SelectObject(hDCMemoire, pGame->alienBitmap);

    BitBlt(hDCMemoire, 0, 0, 0, 0, hDC, 0, 0, SRCCOPY); // capture

    float speed = 2.0;

    int aliens[4] = {0, 90, 180, 265};

    srand (time(NULL));

    for (int i = 0; i < NBR_ALIEN; i++) {

            //pGame->player.dataAccess = FALSE;

            pGame->alien[i].x = rand() % (pGame->ui.w - 80);

            pGame->alien[i].y = 0;

            pGame->alien[i].type = aliens[rand() % 4];

            for (int k = 0; k < pGame->ui.h - 10; k += speed) {

                pGame->alien[i].y = k;

                if(!ANIMATION || pGame->player.shotCollision) {
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

            speed += 0.05;

            pGame->player.gamescore += 10;

        }

        CleanUPResources(gameWin, hDC, hDCMemoire, pGame->alienBitmap);

    return 0;
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


