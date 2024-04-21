#include "game.h"

void CreateGameUI(struct GAME *pGame, HANDLE hConsole, HWND hwndConsole, int xConsoleWindow, int yConsoleWindow) {

    SetConsoleTitle(pGame->ui.gameName);

    SetWindowPos(hwndConsole, HWND_TOPMOST, xConsoleWindow,
                 yConsoleWindow, 400, pGame->ui.h, SWP_SHOWWINDOW);


    SetConsoleTextAttribute(hConsole, 13);
    printf("  _________ __                             \n");
    printf(" /   _____//  |______ _______ __ __  ____  \n");
    printf(" \\_____  \\\\   __\\__  \\\\_  __ \\  |  \\/    \\ \n");
    printf(" /        \\|  |  / __ \\|  | \\/  |  /   |  \\\n");
    printf("/_______  /|__| (____  /__|  |____/|___|  /\n");
    printf("        \\/           \\/                 \\/ \n");
    printf("\n\n                   *********                   \n");

    SetConsoleTextAttribute(hConsole, 11);

    char synopsys[155] = "You're lost in space on a reconnaissance mission and need to get back to Earth. Can you make it back without being eaten by one of those horrible aliens ?";

    for(int i = 0; i <= strlen(synopsys); i++) {

        putchar(synopsys[i]);

        Sleep(10);
    }
    SetConsoleTextAttribute(hConsole, 12);

    printf("\n\n\t\tHow to play: \n\n");

    printf("Press [<][>] arrows to dodge aliens and asteroids.\nPress[X] key to explode aliens.\n\n");

    SetConsoleTextAttribute(hConsole, 12);
    printf("Coins restore your health by one HP.\nIf you get:\n\n");
    SetConsoleTextAttribute(hConsole, 15);
    printf("All the coins -> 3 stars.\n");
    SetConsoleTextAttribute(hConsole, 7);
    printf("More than half -> 2 stars.\n");
    SetConsoleTextAttribute(hConsole, 8);
    printf("Less than half -> 1 star.\n\n");
    SetConsoleTextAttribute(hConsole, 13);
    printf("Click on this window and press any key to continue...");

    getch();

    SetActiveWindow(GetWindow(pGame->ui.gameName, 0));

}

void LoadTexture(struct GAME *pGame, HBITMAP *bitmap, int resId, char *errorMessage) {

    *bitmap = (HBITMAP)LoadImage(GetModuleHandle(0),
                              MAKEINTRESOURCE(resId),
                              IMAGE_BITMAP,
                              0, 0,
                              LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

    if(!(*bitmap)) {

        ShowGameError(errorMessage);

    }

}


DWORD WINAPI GameManager(LPVOID lparam) {

    struct GAME *pGame = (struct GAME*)lparam;

    HANDLE hThreadBoss;

    HWND gameWin = FindWindow(pGame->ui.gameName, 0);

    HDC hDC = GetDC(gameWin);

    HDC hDCMemoire = CreateCompatibleDC(hDC);

    char buffer[20];

    SetTextColor(hDC, RGB(255, 255, 255));

    while (true) {

        pGame->alien->lockType = rand() % 2;

        if (pGame->alien->nbrAlien == -1) {

            break;
        }

        int level = (pGame->alien->nbrAlien >= 0) ? ((pGame->alien->nbrAlien / 100) + 1) : 0; //définis les niveaux

        switch (level) {
            case 1:
                SetBkColor(hDC, RGB(24, 197, 237));

                pGame->alien->lockType = (pGame->alien->lockType == 0) ? 0 : 1;

                pGame->alien->speed = 1.25;

                break;
            case 2:
                SetBkColor(hDC, RGB(120, 153, 0));

                pGame->alien->lockType = (pGame->alien->lockType == 0) ? 2 : 3;

                pGame->alien->speed = 2.25;

                pGame->extras.asteroidExtrasOn = TRUE;

                break;
            case 3:
                SetBkColor(hDC, RGB(255, 0, 0));

                pGame->alien->lockType = (pGame->alien->lockType == 0) ? 4 : 5;

                pGame->alien->speed = 3.25;


                break;
            case 4:
                SetBkColor(hDC, RGB(127, 0, 252));

                pGame->alien->lockType = (pGame->alien->lockType == 0) ? 6 : 7;

                pGame->alien->speed = 4.25;


                break;

            default:
                break;
        }

        wsprintf(buffer, "[LEVEL %d] Coin : %d", level, pGame->extras.playerCoin);

        TextOut(hDC, 0, 0, buffer, lstrlen(buffer));

        if (level >= 5) {

            pGame->boss.bossActivation = TRUE;

            pGame->alien->ANIMATION_ALIEN = FALSE;

            DrawBossLife(pGame, hDC);

            hThreadBoss = CreateThread(0, 0, &DrawBoss, (LPVOID)pGame, 0, 0);

            WaitForSingleObject(hThreadBoss, INFINITE);

            CloseHandle(hThreadBoss);

            break;
        }

        Sleep(50);
    }

    DrawEnd(pGame, hDC, hDCMemoire);
    pGame->PLAY_AGAIN = TRUE;

    return 0;
}

void InitializeGame(struct GAME *pGame) {

    // Initialisations des variables

    pGame->ui.color = RGB(0, 0, 0);

    pGame->player.bSize = 90;

    pGame->player.speed = 4;

    pGame->player.heartPoint = 3;

    pGame->player.firerate = TRUE;

    pGame->extras.playerCoin = 0;

    pGame->player.ANIMATION_PLAYER = TRUE;

    pGame->alien = (struct ALIEN*)malloc(50 * sizeof(struct ALIEN));

    pGame->alien->nbrAlien = 0;

    pGame->event = 0;

    pGame->alien->lockType = 0;

    pGame->alien->ANIMATION_ALIEN = TRUE;

    pGame->boss.heartPoint = 3;

    pGame->boss.x = 20;

    pGame->boss.y = 30;

    pGame->collisionDetected = FALSE;

    pGame->boss.bossActivation = FALSE;

    pGame->boss.heartPoint = 3;


    pGame->extras.coinX = 0;

    pGame->extras.asteroidX = 1;

    pGame->extras.asteroidExtrasOn = FALSE;

    pGame->extras.numbCoin = 0;

    pGame->END = -1;

    pGame->PLAY_AGAIN = FALSE;

    pGame->gameWin = FindWindow(pGame->ui.gameName, 0);

    if (! pGame->gameWin) {

        ShowGameError("[FATAL ERROR] Can't find the specified window ('Starun')");
    }


    LoadTexture(pGame, &pGame->player.playerBitmap, IDB_IMAGE1, "[FATAL ERROR] The game was unable to load the specified bitmap (player_texture.bmp)");
    LoadTexture(pGame, &pGame->extras.earthBitmap, IDB_IMAGE2, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_texture.bmp)");

    LoadTexture(pGame, &pGame->player.fullLifeBitmap, IDB_IMAGE5, "[FATAL ERROR] The game was unable to load the specified bitmap (full_life_texture.bmp)");
    LoadTexture(pGame, &pGame->player.midLifeBitmap, IDB_IMAGE6, "[FATAL ERROR] The game was unable to load the specified bitmap (mid_life_texture.bmp)");
    LoadTexture(pGame, &pGame->player.lowLifeBitmap, IDB_IMAGE7, "[FATAL ERROR] The game was unable to load the specified bitmap (low_life_texture.bmp)");
    LoadTexture(pGame, &pGame->boss.bossBitmap, IDB_IMAGE8, "[FATAL ERROR] The game was unable to load the specified bitmap (boss_texture.bmp)");
    LoadTexture(pGame, &pGame->boss.bossWeaponBitmap, IDB_IMAGE9, "[FATAL ERROR] The game was unable to load the specified bitmap (boss_shot_texture.bmp)");

    LoadTexture(pGame, &pGame->alien->alien1Bitmap, IDB_IMAGE20, "[FATAL ERROR] The game was unable to load the specified bitmap (A1.bmp)");
    LoadTexture(pGame, &pGame->alien->alien2Bitmap, IDB_IMAGE21, "[FATAL ERROR] The game was unable to load the specified bitmap (A2.bmp)");
    LoadTexture(pGame, &pGame->alien->alien3Bitmap, IDB_IMAGE22, "[FATAL ERROR] The game was unable to load the specified bitmap (A3.bmp)");
    LoadTexture(pGame, &pGame->alien->alien4Bitmap, IDB_IMAGE23, "[FATAL ERROR] The game was unable to load the specified bitmap (A4.bmp)");
    LoadTexture(pGame, &pGame->alien->alien5Bitmap, IDB_IMAGE24, "[FATAL ERROR] The game was unable to load the specified bitmap (A5.bmp)");
    LoadTexture(pGame, &pGame->alien->alien6Bitmap, IDB_IMAGE25, "[FATAL ERROR] The game was unable to load the specified bitmap (A6.bmp)");
    LoadTexture(pGame, &pGame->alien->alien7Bitmap, IDB_IMAGE26, "[FATAL ERROR] The game was unable to load the specified bitmap (A7.bmp)");
    LoadTexture(pGame, &pGame->alien->alien8Bitmap, IDB_IMAGE27, "[FATAL ERROR] The game was unable to load the specified bitmap (A8.bmp)");

    LoadTexture(pGame, &pGame->extras.coinBitmap, IDB_IMAGE30, "[FATAL ERROR] The game was unable to load the specified bitmap (coin_textures.bmp)");
    LoadTexture(pGame, &pGame->extras.asteroidBitmap, IDB_IMAGE31, "[FATAL ERROR] The game was unable to load the specified bitmap (asteroid_texture.bmp)");

     //LoadTexture(pGame, &pGame->ui.backgroundBitmap, IDB_IMAGE11, "[FATAL ERROR] The game was unable to load the specified bitmap (background_texture.bmp)"); :(

    GetObject(pGame->alien->alien1Bitmap, sizeof(pGame->alien->alienBitmapInfos), &pGame->alien->alienBitmapInfos);

    GetObject(pGame->player.playerBitmap, sizeof(pGame->player.playerBitmapInfos), &pGame->player.playerBitmapInfos);

    GetObject(pGame->boss.bossWeaponBitmap, sizeof(pGame->boss.bossWeaponBitmapInfos), &pGame->boss.bossWeaponBitmapInfos);

    GetObject(pGame->boss.bossBitmap, sizeof(pGame->boss.bossBitmapInfos), &pGame->boss.bossBitmapInfos);

    GetObject(pGame->extras.coinBitmap, sizeof(pGame->extras.coinBitmapInfos), &pGame->extras.coinBitmapInfos);

    GetObject(pGame->extras.asteroidBitmap, sizeof(pGame->extras.asteroidBitmapInfos), &pGame->extras.asteroidBitmapInfos);

    GetObject(pGame->extras.asteroidBitmap, sizeof(pGame->extras.asteroidBitmapInfos), &pGame->extras.asteroidBitmapInfos);

    InitGameSound(pGame);

    alSourcePlay(pGame->sound.gameMusicSource);

    pGame->hThreadAlien = CreateThread(0, 0, &DrawAliens, (LPVOID)pGame, 0, 0);

    pGame->hThreadPlayer = CreateThread(0, 0, &DrawPlayer, (LPVOID)pGame, 0, 0);

    pGame->hThreadGameFrames = CreateThread(0, 0, &RefreshGameFrames, (LPVOID)pGame, 0, 0);

    pGame->hThreadGameManager = CreateThread(0, 0, &GameManager, (LPVOID)pGame, 0, 0);

    pGame->hThreadExtras = CreateThread(0, 0, &DrawExtras, (LPVOID)pGame, 0, 0);

    /*memset(pGame->alien, 0, sizeof(struct ALIEN) * NBR_ALIEN);

    memset(&pGame->player, 0, sizeof(struct PLAYER));

    memset(&pGame->sound, 0, sizeof(struct SOUND));*/


}

void ExitGameThreads(HANDLE gameThread) {

    WaitForSingleObject(gameThread, INFINITE);

    CloseHandle(gameThread);

}

int RunGame(struct  GAME *pGame) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    HWND hwndConsole = GetConsoleWindow();

    char choice;

    int x = (((pGame->ui.w + 800) / 2) - (pGame->ui.w / 2)) + pGame->ui.w;

    int y = ((pGame->ui.h + 200) / 2) - (pGame->ui.h / 2);

    //Appels

    CreateGameUI(pGame, hConsole, hwndConsole, x, y);

    InitializeGame(pGame);

    ExitGameThreads(pGame->hThreadGameManager);

    while(pGame->PLAY_AGAIN) {

            while (kbhit()) { getch(); }

            printf("New game (y/n): ");

            if(scanf(" %c", &choice) == 1) {

                if(choice == 'y') {

                    CleanUpGameScreen(pGame->gameWin, 0, 0, pGame->ui.w,pGame->ui.h);

                    InitializeGame(pGame);

                    ExitGameThreads(pGame->hThreadGameManager);


            }   else if(choice == 'n') {

                ExitGameThreads(pGame->hThreadAlien);

                ExitGameThreads(pGame->hThreadPlayer);

                ExitGameThreads(pGame->hThreadGameFrames);

                ExitGameThreads(pGame->hThreadGameManager);

                ExitGameThreads(pGame->hThreadExtras);

                printf("See You, hero !\n\n");

                SetConsoleTextAttribute(hConsole, 15);

                printf("[CREDITS]: \n[textures used] https://www.freepik.com/author/macrovector (somes I edited)\n[icon used] https://goodstuffnononsense.com\n[victory sound] Lion Cookie Sounds\n\n");

                printf("Starun was coded in C by me, sk0za and finished the 21/04/2024. I learned a lot, especially about data structures. It was a great challenge !");

                getch();

                ExitProcess(0);
            }
        }

    }

    return 0;
}



