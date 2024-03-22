#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <stdbool.h>

#include "game.h"

void CleanUPResources(HWND hWND, HDC hDC, HDC hDCMem, HBITMAP hObject);

void CleanUpGameScreen(HWND gameInterface, int xPos, int yPos, int wRectangle, int hRectangle);

void ShowGameError(LPCSTR message);
//void InitGameSound(struct GAME *pGame);

#endif // UTILS_H


