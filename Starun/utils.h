#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <stdbool.h>

#include "game.h"

#define IDR_WAVE1 101
#define IDR_WAVE2 102
#define IDR_WAVE3 103
#define IDR_WAVE4 104
#define IDR_WAVE5 105



void CleanUPResources(HWND hWND, HDC hDC, HDC hDCMem, HBITMAP hentitie);

void EntitiesMask(HWND hwnd, HDC hDCEntitie, int wEntitie, int hEntitie, int xEntitie, int yEntitie) ;

void InitGameSound(struct GAME *pGame);

void PlayGameSound(struct GAME *pGame, char* sound);

void ClearGameDataSound(struct GAME *pGame, int tMlls);

#endif // UTILS_H


