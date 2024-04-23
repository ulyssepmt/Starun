#ifndef SOUNDS_H
#define SOUNDS_H

#include "game.h"
#define IDR_WAVE1 101
#define IDR_WAVE2 102
#define IDR_WAVE3 103
#define IDR_WAVE4 104
#define IDR_WAVE5 105
#define IDR_WAVE6 106
#define IDR_WAVE7 107
#define IDR_WAVE8 108

void InitGameSound(struct GAME *pGame);

void ClearGameDataSound(struct GAME *pGame, int tMlls);

#endif // SOUNDS_H
