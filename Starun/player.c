#include "player.h"

int PlayerLife(struct GAME *pGame) {


    if(!pGame->collisionDetected && CheckCollision(pGame->spaceshipBitmap, pGame->player.x,
                               pGame->player.y, pGame->alienBitmap,
                               pGame->alien[pGame->event].x, pGame->alien[pGame->event].y)) {

        pGame->player.heartPoint--;

        pGame->player.gamescore -= 20;

        pGame->collisionDetected = true;

        alSourcePlay(pGame->sound.gameEatingSource);
    }


    return pGame->player.heartPoint;
}

/*Fonction vérifiant deux états du joueur : PVs et score; */
void PlayerState(struct GAME *pGame, HWND gameWin, HDC hDC, HDC hDCGame, HDC hDCHP) {

    /*Gestion du score du joueur*/

    char buffer[20];

    SetTextColor(hDC, RGB(255, 255, 255));

    if (pGame->player.gamescore >= 400) {

        ANIMATION = FALSE;

        pGame->END = TRUE;

    }
    else if (pGame->player.gamescore >= 300) {

        SetBkColor(hDC, RGB(127, 0, 252));

        wsprintf(buffer, "[LEVEL 4] Score : %d", pGame->player.gamescore);

        TextOut(hDC, 0, 0, buffer, lstrlen(buffer));
    }

    else if (pGame->player.gamescore >= 200) {

        SetBkColor(hDC, RGB(255, 0, 0));

        wsprintf(buffer, "[LEVEL 3] Score : %d", pGame->player.gamescore);

        TextOut(hDC, 0, 0, buffer, lstrlen(buffer));
    }
    else if (pGame->player.gamescore >= 100) {

        SetBkColor(hDC, RGB(0, 255, 0));

        wsprintf(buffer, "[LEVEL 2] Score : %d", pGame->player.gamescore);

        TextOut(hDC, 0, 0, buffer, lstrlen(buffer));
    }
    else if (pGame->player.gamescore >= 0) {

        SetBkColor(hDC, RGB(0, 0, 255));

        wsprintf(buffer, "[LEVEL 1] Score : %d", pGame->player.gamescore);

        TextOut(hDC, 0, 0, buffer, lstrlen(buffer));
    }

    /*Gestion des PVs du joueur*/

    SelectObject(hDCHP, pGame->heartPoint3Bitmap);


    if(PlayerLife(pGame) == 2) {

       SelectObject(hDCHP, pGame->heartPoint2Bitmap);
    }

    else if(PlayerLife(pGame) == 1) {

       SelectObject(hDCHP, pGame->heartPoint1Bitmap);
    }

    else if(PlayerLife(pGame) == 0) {

            ANIMATION = FALSE;

            pGame->END = FALSE;

            CleanUPResources(gameWin, 0, hDCHP, pGame->heartPoint3Bitmap);

            CleanUPResources(gameWin, 0, hDCHP, pGame->heartPoint2Bitmap);

            CleanUPResources(gameWin, 0, hDCHP, pGame->heartPoint1Bitmap);
    }

    BitBlt(hDC, 390, 0, 100, 100, hDCHP, 0, 0, SRCCOPY);
}

void ShowEndScore(struct GAME *pGame, HDC hDC, int playerScore, int state) {

    char *endString;

    char stats[20];

    if(state == 0) {
            //ANIMATION = FALSE; Obligé de while non ? afin de récupérer en permanence l'état du bool ANIMATION..
            endString = "GAME OVER";

            SetTextColor(hDC, RGB(255, 0, 0));

            SetBkColor(hDC, pGame->ui.color);

            TextOut(hDC, (pGame->ui.w/ 2) - 50, (pGame->ui.h/ 3) - (lstrlen(endString)), endString, lstrlen(endString));

            sprintf(stats, "SCORE: %d", playerScore);

            TextOut(hDC, (pGame->ui.w/ 2) - 50, (pGame->ui.h/ 3) + 10, stats, lstrlen(stats));


        }

   else if(state == 1) {
            endString = "MISSION ACCOMPLISHED !";

            SetTextColor(hDC, RGB(0, 255, 0));

            SetBkColor(hDC, pGame->ui.color);

            TextOut(hDC, (pGame->ui.w/ 2) - 90, (pGame->ui.h/ 3) - (lstrlen(endString)), endString, lstrlen(endString));

            sprintf(stats, "SCORE: %d", playerScore);

            TextOut(hDC, (pGame->ui.w/ 2) - 40, (pGame->ui.h/ 3) + 10, stats, lstrlen(stats));


        }
}
