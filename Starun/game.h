#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <winuser.h>
#include <mmsystem.h>
#include <sndfile.h>
#include <al.h>
#include <alc.h>

#ifndef GAME_H
#define GAME_H

#include "entities.h" //inclusion multiples de ces fichiers
#include "utils.h"
#include "player.h"

#define NBR_ALIEN 44


extern BOOL ANIMATION;

extern BOOL PLAY_AGAIN;

struct ALIEN {

    int x;

    int y;

    int type;


};

struct PLAYER {

    int direction;

    int x;

    int y;

    int speed;

    int bSize;

    int gamescore;

    int heartPoint;

    BOOL shotCollision;

    BOOL firerate;

};

struct UI {

    int w;

    int h;

    COLORREF color;

    LPCSTR gameName;
};

struct SOUND {

    ALCdevice* device;
    ALCcontext* context;

    ALshort* gameMusic;
    ALuint  gameMusicSource;
    ALuint gameMusicBuffer;

    ALshort* gameExplosion;
    ALuint gameExplosionSource;
    ALuint gameExplosionBuffer;

    ALshort* gameVictory;
    ALuint gameVictorySource;
    ALuint gameVictoryBuffer;

    ALshort* gameGameover;
    ALuint gameoverSource;
    ALuint gameoverBuffer;

    ALshort* gameEating;
    ALuint gameEatingSource;
    ALuint gameEatingBuffer;

};

struct GAME {

    struct ALIEN alien[NBR_ALIEN];

    struct PLAYER player;

    struct UI ui;

    struct SOUND sound;

    BOOL collisionDetected;

    BOOL END;

    HBITMAP alienBitmap;

    HBITMAP spaceshipBitmap;

    HBITMAP earthBitmap;

    HBITMAP heartPoint3Bitmap;

    HBITMAP heartPoint2Bitmap;

    HBITMAP heartPoint1Bitmap;

    HBITMAP weaponBitmap;

    HBITMAP explosionBitmap;

    HANDLE hThreadAlien;

    HANDLE hThreadPlayer;

    int event;

    /*HBITMAP coinBitmap;
    struct COIN coin;
    */

};

void LoadTexture(struct GAME *pGame, HBITMAP *bitmap, int resId, char *errorMessage);

void SetNewGame(struct GAME *pGame);

BOOL CheckCollision(HBITMAP hEntitie1, int xEntitie1, int yEntitie1, HBITMAP hEntitie2, int xEntitie2, int yEntitie2);

void RunGame(struct GAME *pGame);


#endif // GAME_H


/*struct COIN { BONUS

    int x;

    int y;

    int addCoin;

};*/

/*extern int w, h;

extern COLORREF color;

extern BOOL ANIMATION;

extern char *gameName;

extern BOOL COLLISION;

extern BOOL ANIMATION;

extern int gamescore;

void RunGame(struct  GAME *pGame);*/


//extern    BOOL dataAccess;
//extern BOOL COLLISION;


