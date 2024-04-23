#include "game.h"

DWORD WINAPI DrawShots(LPVOID lparam) {

    struct GAME* pGame = (struct GAME *)lparam;

    HDC hDC = GetDC(pGame->gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    LoadTexture(pGame, &pGame->player.explosionBitmap, IDB_IMAGE4, "[FATAL ERROR] The game was unable to load the specified bitmap (explosion_texture.bmp)");
    LoadTexture(pGame, &pGame->player.playerWeaponBitmap, IDB_IMAGE3, "[FATAL ERROR] The game was unable to load the specified bitmap (shots_texture.bmp)");

    GetObject(pGame->player.explosionBitmap, sizeof(pGame->player.explosionBitmapInfos), &pGame->player.explosionBitmapInfos);
    GetObject(pGame->player.playerWeaponBitmap, sizeof(pGame->player.playerWeaponBitmapInfos), &pGame->player.playerWeaponBitmapInfos);

    SelectObject(hDCMemoire, pGame->player.playerWeaponBitmap);

    int xshot = pGame->player.x;

    int top = -50;

    for(int i = pGame->player.y - 25; i > top  /*bar*/ ; i--) {

        BitBlt(hDC, xshot + 42, i, 20, 20, hDCMemoire, 0, 0, SRCCOPY); // bitmap tirs

        if(!pGame->boss.bossActivation && CheckCollision(xshot, i, pGame->player.playerWeaponBitmapInfos.bmWidth, pGame->player.playerWeaponBitmapInfos.bmHeight,
                pGame->alien[pGame->event].x, pGame->alien[pGame->event].y, pGame->alien->alienBitmapInfos.bmWidth, pGame->alien->alienBitmapInfos.bmHeight))  {

                pGame->player.shotCollision = TRUE;

                alSourcePlay(pGame->sound.gameExplosionSource);

                SelectObject(hDCMemoire, pGame->player.explosionBitmap);

                TransparentBlt(hDC, pGame->alien[pGame->event].x - 20, pGame->alien[pGame->event].y - 20, pGame->player.explosionBitmapInfos.bmWidth, pGame->player.explosionBitmapInfos.bmHeight,
                               hDCMemoire, 0, 0, pGame->player.explosionBitmapInfos.bmWidth, pGame->player.explosionBitmapInfos.bmHeight, pGame->ui.color);

                break;


           }
           if (pGame->boss.bossActivation && CheckCollision(xshot, i, pGame->player.playerWeaponBitmapInfos.bmWidth, pGame->player.playerWeaponBitmapInfos.bmHeight,
                pGame->boss.x, pGame->boss.y, pGame->boss.bossBitmapInfos.bmWidth, pGame->boss.bossBitmapInfos.bmHeight))  {

                top = pGame->boss.y + 160;

                alSourcePlay(pGame->sound.gameExplosionSource);

                SelectObject(hDCMemoire, pGame->player.explosionBitmap);

                pGame->boss.heartPoint--;

                TransparentBlt(hDC, pGame->boss.x , pGame->boss.y, pGame->player.explosionBitmapInfos.bmWidth,
                               pGame->player.explosionBitmapInfos.bmHeight, hDCMemoire, 0, 0, pGame->player.explosionBitmapInfos.bmWidth, pGame->player.explosionBitmapInfos.bmHeight, SRCCOPY);
                               


                break;

           }

        BitBlt(hDC, xshot + 42, i + 20, 5, 10, hDCMemoire, 0, 0, BLACKNESS); //mask shots

        Sleep(1);


    }

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.playerWeaponBitmap);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.explosionBitmap);

    return 0;

}

void ShowPlayerScore(struct GAME *pGame, HDC hDC, int state) {

    char *endString;

    char stats[50];

    SIZE statsTextSize, endStringSize;


    if(state == 0) {

            endString = "GAME OVER";

            GetTextExtentPoint32(hDC, endString, lstrlen(endString), &endStringSize);

            SetTextColor(hDC, RGB(255, 0, 0));

            SetBkColor(hDC, pGame->ui.color);

            TextOut(hDC, (pGame->ui.w/ 2) - (endStringSize.cx / 2), 250, endString, lstrlen(endString));

        }

   else if(state == 1) {

            endString = "MISSION ACCOMPLISHED !";

            GetTextExtentPoint32(hDC, endString, lstrlen(endString), &endStringSize);

            SetTextColor(hDC, RGB(0, 255, 0));

            SetBkColor(hDC, pGame->ui.color);

            TextOut(hDC, (pGame->ui.w / 2) - (endStringSize.cx / 2), (pGame->ui.h / 3) - (lstrlen(endString)), endString, lstrlen(endString));

            if(pGame->extras.playerCoin == pGame->extras.numbCoin) {

                sprintf(stats, "SCORE: %d with *** stars!", pGame->extras.playerCoin);
            }

            else if(pGame->extras.playerCoin >=  pGame->extras.numbCoin / 2) {

                sprintf(stats, "SCORE: %d with ** stars!", pGame->extras.playerCoin);
            }

            else if(pGame->extras.playerCoin <= pGame->extras.numbCoin / 2) {

                sprintf(stats, "SCORE: %d with * stars!", pGame->extras.playerCoin);
            }

            GetTextExtentPoint32(hDC, stats, lstrlen(stats), &statsTextSize);

            TextOut(hDC, (pGame->ui.w / 2) - (statsTextSize.cx / 2), (pGame->ui.h / 3) + 10, stats, lstrlen(stats));


        }
}
void DrawPlayerLife(struct GAME *pGame, HDC hDC, HDC hDCMemoire) {

        /* Gestion PV Joueur */
        if(!pGame->collisionDetected && CheckCollision(pGame->player.x,

                               pGame->player.y, pGame->player.playerBitmapInfos.bmWidth, pGame->player.playerBitmapInfos.bmHeight,

                               pGame->alien[pGame->event].x, pGame->alien[pGame->event].y, 90, 95)) {


        pGame->player.heartPoint--;

        //pGame->extras.playerCoin--;

        pGame->collisionDetected = true;

        alSourcePlay(pGame->sound.gameEatingSource);
    }

    if(pGame->boss.bossActivation && !pGame->boss.damage && CheckCollision(pGame->boss.bossShotX, pGame->boss.bossShotY, pGame->boss.bossWeaponBitmapInfos.bmWidth, pGame->boss.bossWeaponBitmapInfos.bmHeight,
                              pGame->player.x, pGame->player.y, pGame->player.playerBitmapInfos.bmWidth, pGame->player.playerBitmapInfos.bmHeight)) {

                                alSourcePlay(pGame->sound.gameImpactSource);

                                pGame->player.heartPoint--;

                                pGame->boss.damage = TRUE;

            }


    SelectObject(hDCMemoire, pGame->player.fullLifeBitmap);

    if(pGame->player.heartPoint == 2) {

       SelectObject(hDCMemoire, pGame->player.midLifeBitmap);
    }

    else if(pGame->player.heartPoint == 1) {

       SelectObject(hDCMemoire, pGame->player.lowLifeBitmap);
    }

    else if(pGame->player.heartPoint == 0) { // GameOver

            pGame->alien->ANIMATION_ALIEN = FALSE;

            pGame->alien->nbrAlien = -1;

            pGame->boss.heartPoint = 0;

            CleanUPResources(pGame->gameWin, 0, hDCMemoire, pGame->player.fullLifeBitmap);

            CleanUPResources(pGame->gameWin, 0, hDCMemoire, pGame->player.midLifeBitmap);

            CleanUPResources(pGame->gameWin, 0, hDCMemoire, pGame->player.lowLifeBitmap);

            pGame->END = -1;

    }

    BitBlt(hDC, 390, 0, 100, 100,hDCMemoire, 0, 0, SRCCOPY);
}

DWORD WINAPI DrawPlayer(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    HANDLE AsyncShots;

    HDC hDC = GetDC(pGame->gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    HDC hDCheartPoint = CreateCompatibleDC(hDC);

    int gameWinW = pGame->ui.w;

    int gameWinH = pGame->ui.h;

    int marge = 50;

    SelectObject(hDCMemoire, pGame->player.playerBitmap);

    while (pGame->player.ANIMATION_PLAYER) {

            pGame->player.x = (gameWinW / 2) - (pGame->player.bSize / 2) + pGame->player.direction;

            pGame->player.y = (gameWinH - pGame->player.bSize) - (pGame->player.bSize / 2);

            BitBlt(hDC, pGame->player.x, pGame->player.y, pGame->player.playerBitmapInfos.bmWidth, pGame->player.playerBitmapInfos.bmHeight, hDCMemoire, 0, 0, SRCCOPY);

            Sleep(5);

            if(GetAsyncKeyState(VK_LEFT) != 0) {

                pGame->player.direction -= pGame->player.speed;

                if (pGame->player.direction <= -(gameWinW /2) + marge) {

                pGame->player.direction += pGame->player.speed;

                }
            }

            else if(GetAsyncKeyState(VK_RIGHT) != 0) {

                pGame->player.direction += pGame->player.speed;

                if (pGame->player.direction >= (gameWinW/ 2) - marge - 20) {

                    pGame->player.direction -= pGame->player.speed;

                }
            }
            else if(GetAsyncKeyState(0x58) != 0 && pGame->player.firerate)  { //X

                    pGame->player.firerate = FALSE;

                    AsyncShots = CreateThread(0, 0, DrawShots, (LPVOID)pGame, 0, 0);

                    WaitForSingleObject(AsyncShots, 150);

                    CloseHandle(AsyncShots);

            }

            DrawPlayerLife(pGame, hDC, hDCheartPoint);

    }

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.playerBitmap);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.playerWeaponBitmap);

    return 0;
}


DWORD WINAPI DrawAliens(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    HDC hDC = GetDC(pGame->gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    HBITMAP aliens[8] = { pGame->alien->alien1Bitmap,
                          pGame->alien->alien2Bitmap,
                          pGame->alien->alien3Bitmap,
                          pGame->alien->alien4Bitmap,
                          pGame->alien->alien5Bitmap,
                          pGame->alien->alien6Bitmap,
                          pGame->alien->alien7Bitmap,
                          pGame->alien->alien8Bitmap,
                        };
    srand(time(NULL));

    for (int i = 0; i < 50; i++) {

            pGame->alien[i].x = rand() % (pGame->ui.w - 90);

            pGame->alien[i].y = 0;

            SelectObject(hDCMemoire, aliens[pGame->alien->lockType]);


            for (int k = 0; k < pGame->ui.h - 10; k += pGame->alien->speed) {

                pGame->alien[i].y = k;

                if(pGame->player.shotCollision) {

                    break;

                }

                BitBlt(hDC, pGame->alien[i].x, pGame->alien[i].y, pGame->alien->alienBitmapInfos.bmWidth, pGame->alien->alienBitmapInfos.bmHeight, hDCMemoire, 0, 0, SRCCOPY);
                Sleep(5);

                //pGame->player.dataAccess = TRUE; <3

            }

            pGame->player.shotCollision = FALSE;

            pGame->player.firerate = TRUE;

            pGame->collisionDetected = FALSE;

            pGame->event += 1;

            pGame->alien[i].x = 0;

            pGame->alien[i].y = 0;

            if(!pGame->alien->ANIMATION_ALIEN) {

                pGame->alien->nbrAlien = -1;

                break;
            }

            pGame->alien->nbrAlien += 10;


        }

        CleanUpGameScreen(pGame->gameWin, 0, 0, 1000, 1000);

        DeleteObject(aliens);

        free(pGame->alien);

        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien1Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien2Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien3Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien4Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien5Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien6Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien7Bitmap);
        CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->alien->alien8Bitmap);

        return 0;
}

void DrawBossLife(struct GAME *pGame, HDC hDC) {

    char buffer[20];

    SetTextColor(hDC, RGB(255, 255, 255));

    SetBkColor(hDC, RGB(127, 0, 255));

    wsprintf(buffer, "[BOSS] Boss HP : %d", pGame->boss.heartPoint);

    TextOut(hDC, 0, 0, buffer, lstrlen(buffer));

}

DWORD WINAPI DrawBoss(LPVOID lparam) {

    struct GAME *pGame = (struct GAME*)lparam;

    HDC hDC = GetDC(pGame->gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    SelectObject(hDCMemoire, pGame->boss.bossBitmap);

    int distance = 0;

    pGame->boss.x = 20;

    //ANIMATION_ALIEN = FALSE;

    CleanUpGameScreen(pGame->gameWin, 0, 0, pGame->ui.w, pGame->ui.h);

    alSourcePlay(pGame->sound.gameBossSource);

    srand(time(NULL));

    while(pGame->boss.heartPoint > 0) {

        SelectObject(hDCMemoire, pGame->boss.bossBitmap);

        distance = rand() % (400);

        int step = (pGame->boss.x < distance) ? 1 : -1;

        while (pGame->boss.x != distance) {

            DrawBossLife(pGame, hDC);

            BitBlt(hDC, pGame->boss.x, pGame->boss.y /*20*/, pGame->boss.bossBitmapInfos.bmWidth, pGame->boss.bossBitmapInfos.bmHeight,
                   hDCMemoire, 0, 0, SRCCOPY);

            pGame->boss.x += step;

            Sleep(1);
        }

        pGame->boss.bossShotX = pGame->boss.x + 55;

        for(pGame->boss.bossShotY = pGame->boss.y + 160; pGame->boss.bossShotY < pGame->ui.h; pGame->boss.bossShotY+=5) {

            SelectObject(hDCMemoire, pGame->boss.bossWeaponBitmap);

            BitBlt(hDC, pGame->boss.bossShotX, pGame->boss.bossShotY, pGame->boss.bossWeaponBitmapInfos.bmWidth, pGame->boss.bossWeaponBitmapInfos.bmHeight,
                    hDCMemoire, 0, 0, SRCCOPY);

            Sleep(5);

        }

        pGame->boss.damage = FALSE;

        pGame->player.firerate = TRUE;

    }

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->boss.bossBitmap);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->boss.bossWeaponBitmap);

    pGame->player.ANIMATION_PLAYER = FALSE;


    if(pGame->boss.heartPoint == 0 && pGame->player.heartPoint > 0) pGame->END = 1; //Win


    return 0;
}

DWORD WINAPI DrawExtras(LPVOID lparam) {

    struct GAME *pGame = (struct GAME*)lparam;

    HDC hDC = GetDC(pGame->gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    int mov = 1, dir = 1, randExtras = 0;

    srand(time(NULL));

    while (pGame->alien->ANIMATION_ALIEN) {

        randExtras = rand() % 2;

        if (randExtras == 0) {

            pGame->extras.numbCoin++;

            pGame->extras.coinX = rand() % (pGame->ui.w - 80);

            SelectObject(hDCMemoire, pGame->extras.coinBitmap);

            for (int i = 0; (i < pGame->ui.h) && pGame->alien->ANIMATION_ALIEN; i++) {
                if (CheckCollision(pGame->extras.coinX, i, pGame->extras.coinBitmapInfos.bmWidth, pGame->extras.coinBitmapInfos.bmHeight, pGame->player.x, pGame->player.y,
                    pGame->player.playerBitmapInfos.bmWidth,
                    pGame->player.playerBitmapInfos.bmHeight)  ) {

                    alSourcePlay(pGame->sound.gameExtraCoinSource);

                    if(pGame->player.heartPoint >= 3)
                        pGame->player.heartPoint--;

                    pGame->player.heartPoint++;

                    pGame->extras.playerCoin++;

                    break;
                }
                Sleep(1);

                BitBlt(hDC, pGame->extras.coinX, i, 70, 70, hDCMemoire, 0, 0, SRCCOPY);
            }
        } else if ((randExtras == 1) && pGame->extras.asteroidExtrasOn) {

            SelectObject(hDCMemoire, pGame->extras.asteroidBitmap);

            for (int i = -30; (i < pGame->ui.h) && pGame->alien->ANIMATION_ALIEN; i++) {

                pGame->extras.asteroidX += (mov * dir);

                if (pGame->extras.asteroidX >= pGame->ui.w - 80 || pGame->extras.asteroidX <= 0) {
                    dir *= -1;
                }

                if (CheckCollision(pGame->extras.asteroidX, i, pGame->extras.asteroidBitmapInfos.bmWidth,  pGame->extras.asteroidBitmapInfos.bmHeight,
                                   pGame->player.x, pGame->player.y,pGame->player.playerBitmapInfos.bmWidth,
                                   pGame->player.playerBitmapInfos.bmHeight)) {

                    alSourcePlay(pGame->sound.gameImpactSource);

                    pGame->player.heartPoint--;

                    //pGame->extras.playerCoin--;

                    break;
                }
                Sleep(2);

                BitBlt(hDC, pGame->extras.asteroidX, i, pGame->extras.asteroidBitmapInfos.bmWidth,  pGame->extras.asteroidBitmapInfos.bmHeight, hDCMemoire, 0, 0, SRCCOPY);
            }
        }

        ReleaseDC(pGame->gameWin, hDCMemoire);

        if (pGame->alien->ANIMATION_ALIEN)
            Sleep(rand() % 2000);
    }

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->extras.coinBitmap);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->extras.asteroidBitmap);

    return 0;
}

DWORD WINAPI RefreshGameFrames(LPVOID lparam) {

    struct GAME *pGame = (struct GAME*)lparam;

    while(pGame->boss.heartPoint > 0) {

            if(pGame->boss.bossActivation) {

                CleanUpGameScreen(pGame->gameWin, 0, 160, pGame->ui.w, pGame->ui.h);

            }
            else {

                CleanUpGameScreen(pGame->gameWin, 0, 20, pGame->ui.w, pGame->ui.h);
            }

            Sleep(400);
    }
    return 0;

}
void DrawEnd(struct GAME *pGame, HDC hDC, HDC hDCMemoire) {

    int gameWinW = pGame->ui.w;

    int gameWinH = pGame->ui.h;

    alSourceStop(pGame->sound.gameMusicSource);

    pGame->extras.asteroidExtrasOn = FALSE;

    pGame->player.ANIMATION_PLAYER = FALSE;

    pGame->alien->ANIMATION_ALIEN = FALSE;

    ExitGameThreads(pGame->hThreadAlien);

    ExitGameThreads(pGame->hThreadPlayer);

    ExitGameThreads(pGame->hThreadGameFrames);

    ExitGameThreads(pGame->hThreadExtras);

    CleanUpGameScreen(pGame->gameWin, 0,0, 1000, 1000);

    if(pGame->END == 1) {

    alSourcePlay(pGame->sound.gameVictorySource);

    SelectObject(hDCMemoire, pGame->extras.earthBitmap);

    for (int z = 0; z < gameWinH - 280; z++) {

        BitBlt(hDC, gameWinW / 2 - 70, z, 130, 130, hDCMemoire, 0, 0, SRCCOPY);
        Sleep(1);
    }

    pGame->player.playerBitmap = (HBITMAP)LoadImage(GetModuleHandle(0),
                                          MAKEINTRESOURCE(IDB_IMAGE1),
                                          IMAGE_BITMAP,
                                          0, 0,
                                          LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

    SelectObject(hDCMemoire, pGame->player.playerBitmap);

    //Placement
    int schipX = (gameWinW / 2) - (pGame->player.bSize / 2);

    int schipY = (gameWinH - pGame->player.bSize) - (pGame->player.bSize / 2);

    BitBlt(hDC, schipX, schipY, gameWinW, gameWinH, hDCMemoire, 0, 0, SRCCOPY);

    for (int i = 0; i <= gameWinW; i += 8) {

        PatBlt(hDC, schipX, schipY,  gameWinW - i, gameWinH - i, BLACKNESS);

        StretchBlt(hDC, schipX, schipY, gameWinW - i, gameWinH - i, hDCMemoire, 0, 0, gameWinW, gameWinH, SRCCOPY);

        Sleep(15);

    }

    ShowPlayerScore(pGame, hDC, 1);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->extras.earthBitmap);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

    printf("\nPilot: Done, let's goooo!\n");

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);


    }
    else {

            CleanUpGameScreen(pGame->gameWin, 0, 0, 600, 600);

            ShowPlayerScore(pGame, hDC, 0);

            alSourcePlay(pGame->sound.gameoverSource);

            CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->extras.earthBitmap);

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);

            printf("\nPilot: AAAAAAAAAARG!\n");

            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);

    }

    /*CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.fullLifeBitmap);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.midLifeBitmap);

    CleanUPResources(pGame->gameWin, hDC, hDCMemoire, pGame->player.lowLifeBitmap);*/

    ClearGameDataSound(pGame, 2000);

    pGame->END = -1;

}

/*
La fonction ci-dessous devait afficher les textures du jeu avec une transparence appliquée sur la couleur noire.
La fonction RefreshGameFrames permettait ainsi d'afficher la map du jeu (voir dossier "background_ideas") avec des textures découpées,
rendant le tout épuré et plutôt pas mal.
Malheuresement, le déplacement d'un bitmap laisse des traces derrière lui (précédente copie), donc assez dégeulasse.
Je ne vois pas trop comment j'aurais pu résoudre ce problème même si en vrai, c'est peut être tout bête :/

Déçu car c'est le seul et dernier challenge technique que je n'ai pas pu résoudre :(

void DrawObject(HDC hDC, int xDest, int yDest, int wDest, int hDest, HDC hDCMemoire, int wSrc, int hSrc, COLORREF BKG) {
    RECT rect = { xDest, yDest, xDest + wDest, yDest + hDest/2 };

    // Fill the rectangle with the background color before drawing the bitmap
    FillRect(hDC, &rect, GetStockObject(BLACK_BRUSH));

    // Draw the bitmap
    TransparentBlt(hDC, xDest, yDest, wDest, hDest, hDCMemoire, 0, 0, wSrc, hSrc, BKG);

    Sleep(1);
}*/
