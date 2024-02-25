#include "game.h"

ANIMATION = TRUE; // Limite limite

BOOL PLAY_AGAIN = FALSE;

BOOL CheckCollision(HBITMAP hEntitie1, int xEntitie1, int yEntitie1, HBITMAP hEntitie2,
                    int xEntitie2, int yEntitie2) {

    BITMAP bmcheck1, bmcheck2;

    GetObject(hEntitie1, sizeof(bmcheck1), &bmcheck1);

    GetObject(hEntitie2, sizeof(bmcheck2), &bmcheck2);

    /*  detection = TRUE si les rectangles crées se chevauchent entre les coordx + largeur du bitmap
        correspondant à celui de l'aliens et y1 + 20 correspondant à ses coordy + sa hauteur(20)
        avec ceux du joueur
    */

    //left, top , right, bottom

    RECT rcEntitie1 = {xEntitie1, yEntitie1, xEntitie1 + bmcheck1.bmWidth, yEntitie1 + bmcheck1.bmHeight};
    RECT rcEntitie2 = {xEntitie2 - 10, yEntitie2, xEntitie2 + bmcheck2.bmWidth, yEntitie2 + 95};
    RECT res;

    return IntersectRect(&res, &rcEntitie1, &rcEntitie2);



}

void CreateGameUI(struct GAME *pGame, HANDLE hConsole, HWND hwndConsole, int xConsoleWindow, int yConsoleWindow) {

    SetConsoleTitle(pGame->ui.gameName);

    SetWindowPos(hwndConsole, HWND_TOPMOST, xConsoleWindow,
                 yConsoleWindow, 400, 400, SWP_SHOWWINDOW);


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

    printf("Press [<][>] arrows to dodge aliens.\n\n");

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

DWORD WINAPI WaitForSingleObjectAsync(LPVOID lparam) {

    struct GAME *pGame = (struct GAME*)lparam;

    WaitForSingleObject(pGame->hThreadAlien, INFINITE);

    WaitForSingleObject(pGame->hThreadPlayer, INFINITE);

    CloseHandle(pGame->hThreadAlien);

    CloseHandle(pGame->hThreadPlayer);

    PLAY_AGAIN = TRUE;

    return 0;

}

void InitializeGame(struct GAME *pGame) {

    // Initialisations des variables
    memset(pGame->alien, 0, sizeof(struct ALIEN) * NBR_ALIEN);

    memset(&pGame->player, 0, sizeof(struct PLAYER));

    memset(&pGame->sound, 0, sizeof(struct SOUND));


    pGame->ui.color = RGB(0, 0, 0);

    pGame->player.bSize = 90;

    pGame->player.speed = 4;

    pGame->player.gamescore = 0;

    pGame->player.heartPoint = 3;

    pGame->event = 0;

    pGame->player.shotCollision = NULL;

    pGame->collisionDetected = FALSE;

    LoadTexture(pGame, &pGame->earthBitmap, IDB_IMAGE3, "[FATAL ERROR] The game was unable to load the specified bitmap (earth_entitie.bmp)");

    LoadTexture(pGame, &pGame->spaceshipBitmap, IDB_IMAGE, "[FATAL ERROR] The game was unable to load the specified bitmap (player_entitie.bmp)");

    LoadTexture(pGame, &pGame->alienBitmap, IDB_IMAGE2, "[FATAL ERROR] The game was unable to load the specified bitmap (enemy_entitie.bmp)");

    LoadTexture(pGame, &pGame->heartPoint3Bitmap, IDB_IMAGE6, "[FATAL ERROR] The game was unable to load the specified bitmap (enemy_entitie.bmp)");

    LoadTexture(pGame, &pGame->heartPoint2Bitmap, IDB_IMAGE7, "[FATAL ERROR] The game was unable to load the specified bitmap (enemy_entitie.bmp)");

    LoadTexture(pGame, &pGame->heartPoint1Bitmap, IDB_IMAGE8, "[FATAL ERROR] The game was unable to load the specified bitmap (enemy_entitie.bmp)");

    ANIMATION = TRUE;

    //pGame->player.dataAccess = NULL; <3

}
void RunGame(struct  GAME *pGame) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    HWND hwndConsole = GetConsoleWindow();

    char choice;

    int x = (((pGame->ui.w + 800) / 2) - (pGame->ui.w / 2)) + pGame->ui.w;

    int y = ((pGame->ui.h + 200) / 2) - (pGame->ui.h / 2);

    InitializeGame(pGame);

    //Appels

    CreateGameUI(pGame, hConsole, hwndConsole, x, y);

    InitGameSound(pGame);

    alSourcePlay(pGame->sound.gameMusicSource);

    pGame->hThreadAlien = CreateThread(0, 0, &DrawAliens, (LPVOID)pGame, 0, 0);

    pGame->hThreadPlayer = CreateThread(0, 0, &DrawPlayer, (LPVOID)pGame, 0, 0);

    WaitForSingleObjectAsync(pGame);

    while(PLAY_AGAIN) {

            printf("New game (y/n): ");

            if(scanf("%c", &choice) == 1) {

                if(choice == 'y') {

                    CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0), 0, 0, pGame->ui.w,pGame->ui.h);

                    //ResetGame(pGame);
                    InitializeGame(pGame);

                    InitGameSound(pGame);

                    alSourcePlay(pGame->sound.gameMusicSource);

                    pGame->hThreadAlien = CreateThread(0, 0, &DrawAliens, (LPVOID)pGame, 0, 0);

                    pGame->hThreadPlayer = CreateThread(0, 0, &DrawPlayer, (LPVOID)pGame, 0, 0);

                    WaitForSingleObjectAsync(pGame);

            }   else if(choice == 'n') {

                printf("See You, hero !\n\n");

                SetConsoleTextAttribute(hConsole, 14);

                printf("Credits : \n[bitmaps used] https://www.freepik.com/author/macrovector\n[icon used] https://goodstuffnononsense.com\n");

                ExitProcess(0);
            }
        }

       while (getchar() != '\n');
    }

}


/*
Ancienne fonction détection quand je n'avais alors pas connaissance de la fonction
IntersectRect().
La fonction fonctionnait 2/3 du temps (= pas précise) et vérifiait en permanence si les coordy
des aliens générés étaient supérieures à celles du joueur (vaisseau) et (&&) si les coordx des aliens
étaient comprises entre les coordx du joueur et les coordsx + la largeur du bitmap du joueur..

(pas terrible mais pas trop mal pour une première fois haha...)
DWORD WINAPI CheckCollision(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    while(true) {
          //  ShowAlienCoord(pGame->event, pGame->alien[pGame->event].x, pGame->alien[pGame->event].y);
    if(pGame->player.dataAccess == TRUE) {


        if ((pGame->alien[pGame->event].y >= pGame->spaceship.y + 50) && // 82 = taille du vaisseau

            ((pGame->alien[pGame->event].x  >= (pGame->spaceship.x - pGame->spaceship.bSize)) &&

            (pGame->alien[pGame->event].x  <= (pGame->spaceship.x + pGame->spaceship.bSize)))) {


                ANIMATION = FALSE;

                CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0));

                ShowEndScore(pGame, GetDC(FindWindow(pGame->ui.gameName, 0)), 0);

                //PrintScore(GetDC(FindWindow(gameName, 0)), gamescore, 0);
                //COLLISION = TRUE;

                }

            }


        }

    return 0;
}
DWORD WINAPI CheckCollision(LPVOID lparam) {

    struct GAME *pGame = (struct GAME *)lparam;

    while(ANIMATION) {

        if(pGame->player.dataAccess == TRUE) {

            if(BitmapCollision(pGame->alienBitmap, pGame->alien[pGame->event].x, pGame->alien[pGame->event].y, pGame->spaceshipBitmap, pGame->spaceship.x, pGame->spaceship.y)){
                ANIMATION = FALSE;

                CleanUpGameScreen(FindWindow(pGame->ui.gameName, 0));

                ShowEndScore(pGame, GetDC(FindWindow(pGame->ui.gameName, 0)), 0);

                break;
            }

        }
    }

}

    HANDLE hThreadCollisions = CreateThread(0, 0, &CheckCollision, (LPVOID)pGame, 0, 0);

*/

