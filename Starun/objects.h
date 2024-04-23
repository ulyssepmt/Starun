#ifndef OBJECTS_H
#define OBJECTS_H
#include "game.h"

#define IDB_IMAGE1 101
#define IDB_IMAGE2 102
#define IDB_IMAGE3 103
#define IDB_IMAGE4 104
#define IDB_IMAGE5 105
#define IDB_IMAGE6 106
#define IDB_IMAGE7 107
#define IDB_IMAGE8 108
#define IDB_IMAGE9 109

#define IDB_IMAGE20 200
#define IDB_IMAGE21 201
#define IDB_IMAGE22 202
#define IDB_IMAGE23 203
#define IDB_IMAGE24 204
#define IDB_IMAGE25 205
#define IDB_IMAGE26 206
#define IDB_IMAGE27 207

#define IDB_IMAGE30 300
#define IDB_IMAGE31 301


DWORD WINAPI DrawPlayer(LPVOID lparam);

DWORD WINAPI DrawAliens(LPVOID lparam);

DWORD WINAPI DrawBoss(LPVOID lparam);

DWORD WINAPI RefreshGameFrames(LPVOID lparam);

DWORD WINAPI DrawExtras(LPVOID lparam);

#endif





