#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <stdbool.h>

#include "game.h"

void CleanUPResources(HWND hWND, HDC hDC, HDC hDCMem, HBITMAP hObject);

void CleanUpGameScreen(HWND gameInterface, int xPos, int yPos, int wRectangle, int hRectangle);

void EntitiesMask(HWND hwnd, HDC hDCEntitie, int wEntitie, int hEntitie, int xEntitie, int yEntitie) ;

//void InitGameSound(struct GAME *pGame);

void PlayGameSound(struct GAME *pGame, char* sound);

void ShowGameError(LPCSTR message);
#endif // UTILS_H


