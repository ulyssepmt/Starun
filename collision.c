#include "collision.h"


BOOL CheckCollision(int xObject1, int yObject1, int wObject1, int hObject1,
                    int xObject2, int yObject2, int wObject2, int hObject2) {


    RECT rcEntitie1 = {xObject1, yObject1, xObject1 + wObject1, yObject1 + hObject1};

    RECT rcEntitie2 = {xObject2 - 10, yObject2, xObject2 + wObject2, yObject2 + hObject2};

    RECT res;

    return IntersectRect(&res, &rcEntitie1, &rcEntitie2);

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

