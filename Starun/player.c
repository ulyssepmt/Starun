#include "player.h"


/*Fonction vérifiant deux états du joueur : PVs et score; */
/*void PlayerState(struct GAME *pGame, HWND gameWin, HDC hDC, HDC hDCGame, HDC hDCHP) {

    //Gestion du score du joueur

    char buffer[20];

    SetTextColor(hDC, RGB(255, 255, 255));

    if (pGame->player.gamescore >= 400) {

        ANIMATION_ALIEN = FALSE;



       // pGame->END = TRUE;

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

        BossState(pGame, hDC);
    }

    //Gestion des PVs du joueur

    SelectObject(hDCHP, pGame->heartPoint3Bitmap);

    if(PlayerLife(pGame) == 2) {

       SelectObject(hDCHP, pGame->heartPoint2Bitmap);
    }

    else if(PlayerLife(pGame) == 1) {

       SelectObject(hDCHP, pGame->heartPoint1Bitmap);
    }

    else if(PlayerLife(pGame) == 0) {


            pGame->boss.heartPoint = 0;

            pGame->END = FALSE;

            CleanUPResources(gameWin, 0, hDCHP, pGame->heartPoint3Bitmap);

            CleanUPResources(gameWin, 0, hDCHP, pGame->heartPoint2Bitmap);

            CleanUPResources(gameWin, 0, hDCHP, pGame->heartPoint1Bitmap);
    }

    BitBlt(hDC, 390, 0, 100, 100, hDCHP, 0, 0, SRCCOPY);
}*/
