#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>
#include <sndfile.h>
#include <al.h>
#include <alc.h>

#ifndef GAME_H
#define GAME_H

#include "objects.h"
#include "utils.h"
#include "player.h"
#include "sounds.h"
#include "collision.h"

struct ALIEN {

    int x;

    int y;

    int type;

    int lockType;

    int nbrAlien;

    float speed;

    HBITMAP alien1Bitmap;
    HBITMAP alien2Bitmap;
    HBITMAP alien3Bitmap;
    HBITMAP alien4Bitmap;
    HBITMAP alien5Bitmap;
    HBITMAP alien6Bitmap;
    HBITMAP alien7Bitmap;
    HBITMAP alien8Bitmap;

    BITMAP alienBitmapInfos;

    BOOL ANIMATION_ALIEN;


};
struct BOSS {

    int x;

    int y;

    int bossShotX;

    int bossShotY;

    int heartPoint;

    BOOL damage;

    BOOL bossActivation;

    HBITMAP bossBitmap;

    HBITMAP bossWeaponBitmap;

    BITMAP bossBitmapInfos;

    BITMAP bossWeaponBitmapInfos;


};
struct PLAYER {

    int direction;

    int x;

    int y;

    int speed;

    int bSize;

    int heartPoint;

    BOOL shotCollision;

    BOOL firerate;

    BOOL ANIMATION_PLAYER;

    HBITMAP playerBitmap;

    HBITMAP playerWeaponBitmap;

    HBITMAP explosionBitmap;

    HBITMAP fullLifeBitmap;

    HBITMAP midLifeBitmap;

    HBITMAP lowLifeBitmap;

    BITMAP playerBitmapInfos;

    BITMAP playerWeaponBitmapInfos;

    BITMAP explosionBitmapInfos;

};

struct EXTRAS {

    int coinX;

    int asteroidX;

    int playerCoin;

    int numbCoin;

    BOOL asteroidExtrasOn;

    HBITMAP earthBitmap;

    HBITMAP coinBitmap;

    HBITMAP asteroidBitmap;

    BITMAP coinBitmapInfos;

    BITMAP asteroidBitmapInfos;


};

struct UI {

    int w;

    int h;

    COLORREF color;

    LPCSTR gameName;

    //HBITMAP backgroundBitmap;
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

    ALshort* gameBoss;
    ALuint gameBossSource;
    ALuint gameBossBuffer;

    ALshort* gameExtraCoin;
    ALuint gameExtraCoinSource;
    ALuint gameExtraCoinBuffer;

    ALshort* gameImpact;
    ALuint gameImpactSource;
    ALuint gameImpactBuffer;

    ALshort* gamePlayer;
    ALuint gamePlayerSource;
    ALuint gamePlayerBuffer;

};

struct GAME {

    //struct ALIEN alien[NBR_ALIEN];

    struct ALIEN *alien;

    struct PLAYER player;

    struct BOSS boss;

    struct UI ui;

    struct SOUND sound;

    struct EXTRAS extras;

    HANDLE hThreadAlien;

    HANDLE hThreadPlayer;

    HANDLE hThreadGameManager;

    HANDLE hThreadGameFrames;

    HANDLE hThreadExtras;

    HWND gameWin;

    BOOL collisionDetected;

    BOOL PLAY_AGAIN;

    int event;

    int END;

};

DWORD WINAPI GameManager(LPVOID lparam);

void LoadTexture(struct GAME *pGame, HBITMAP *bitmap, int resId, char *errorMessage);

void ExitGameThreads(HANDLE gameThread);

int RunGame(struct GAME *pGame);


#endif // GAME_H
