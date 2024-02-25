#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

void PlayerState(struct GAME *pGame, HWND gameWin, HDC hDC, HDC hDCGame, HDC hDCHP);

void ShowEndScore(struct GAME *pGame, HDC hDC, int playerScore, int state);

void ShowGameError(LPCSTR message);

#endif PLAYER_H

