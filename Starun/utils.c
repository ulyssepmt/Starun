#include "utils.h"

void CleanUpGameScreen(HWND gameInterface, int xPos, int yPos, int wRectangle, int hRectangle) {

        PatBlt(GetDC(gameInterface), xPos, yPos, wRectangle, hRectangle, BLACKNESS);
}

void CleanUPResources(HWND hWND, HDC hDC, HDC hDCMem, HBITMAP hentitie) {

    if (hDCMem) {
        // Sélectionne un objet stocké précédemment dans le DC
        SelectObject(hDCMem, GetStockObject(NULL_BRUSH));

        DeleteDC(hDCMem);
    }

    if (hentitie) {
        DeleteObject(hentitie);
    }

    if (hDC) {
        ReleaseDC(hWND, hDC);
    }

    if(hWND) {
        DestroyWindow(hWND);
    }
}


void EntitiesMask(HWND hwnd, HDC hDCEntitie, int wEntitie, int hEntitie, int xEntitie, int yEntitie) {

    HBRUSH mask = CreateSolidBrush(RGB(0, 0, 0));

      if (mask != NULL) {
        SelectObject(hDCEntitie, mask);

        PatBlt(hDCEntitie, xEntitie, yEntitie, wEntitie, hEntitie, PATCOPY);//Transparence peut-être et non masque

        DeleteObject(mask);
    }
}


void ShowGameError(LPCSTR message) {

    MessageBox(0, message, "Starun", MB_OK | MB_ICONERROR);

    ExitProcess(1);

}
/* debugging
void checkALError(const char* msg) {
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        printf("OpenAL error (%s): %s\n", msg, alGetString(error));
    }
}
*/

ALshort* LoadSoundFromResource(int resourceID, ALsizei* dataSize) {
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceID), RT_RCDATA);
    if (!hResource) {
        ShowGameError("[FATAL ERROR] Sorry, can't find specified resources!");
    }

    HGLOBAL hGlobal = LoadResource(NULL, hResource);

    LPVOID pData = LockResource(hGlobal);

    DWORD dwSize = SizeofResource(NULL, hResource);

    *dataSize = dwSize;

    ALshort* pAudioData = (ALshort*)malloc(dwSize);

    memcpy(pAudioData, pData, dwSize);

    return pAudioData;
}

void InitGameSound(struct GAME *pGame) {

    pGame->sound.device = alcOpenDevice(NULL);
    pGame->sound.context = alcCreateContext(pGame->sound.device, NULL);
    alcMakeContextCurrent(pGame->sound.context);

    alGenSources(1, &pGame->sound.gameMusicSource);
    alGenSources(1, &pGame->sound.gameExplosionSource);
    alGenSources(1, &pGame->sound.gameVictorySource);
    alGenSources(1, &pGame->sound.gameoverSource);
    alGenSources(1, &pGame->sound.gameEatingSource);

    alGenBuffers(1, &pGame->sound.gameMusicBuffer);
    alGenBuffers(1, &pGame->sound.gameExplosionBuffer);
    alGenBuffers(1, &pGame->sound.gameVictoryBuffer);
    alGenBuffers(1, &pGame->sound.gameoverBuffer);
    alGenBuffers(1, &pGame->sound.gameEatingBuffer);

    ALsizei musicDataSize, explosionDataSize, victoryDataSize, gameoverDataSize, gameEatingDataSize;
    pGame->sound.gameMusic = LoadSoundFromResource(IDR_WAVE1, &musicDataSize);
    pGame->sound.gameExplosion = LoadSoundFromResource(IDR_WAVE2, &explosionDataSize);
    pGame->sound.gameVictory = LoadSoundFromResource(IDR_WAVE3, &victoryDataSize);
    pGame->sound.gameGameover = LoadSoundFromResource(IDR_WAVE4, &gameoverDataSize);
    pGame->sound.gameEating = LoadSoundFromResource(IDR_WAVE5, &gameEatingDataSize);

    if (!pGame->sound.gameMusic || !pGame->sound.gameExplosion || !pGame->sound.gameEating
        || !pGame->sound.gameVictory ||  !pGame->sound.gameGameover) {
        ShowGameError("Error loading audio files\n");
        alcMakeContextCurrent(NULL);
        alcDestroyContext(pGame->sound.context);
        alcCloseDevice(pGame->sound.device);
    }


    alBufferData(pGame->sound.gameMusicBuffer, AL_FORMAT_MONO16, pGame->sound.gameMusic, musicDataSize, 96000);
    //checkALError("Setting 1 buffer data");

    alBufferData(pGame->sound.gameExplosionBuffer, AL_FORMAT_MONO16, pGame->sound.gameExplosion, explosionDataSize, 48000);

    alBufferData(pGame->sound.gameVictoryBuffer, AL_FORMAT_MONO16, pGame->sound.gameVictory, victoryDataSize, 96000);

    alBufferData(pGame->sound.gameoverBuffer, AL_FORMAT_MONO16, pGame->sound.gameGameover, gameoverDataSize, 48000);

    alBufferData(pGame->sound.gameEatingBuffer, AL_FORMAT_MONO16, pGame->sound.gameEating, gameEatingDataSize, 96000);

    alSourcei(pGame->sound.gameMusicSource, AL_BUFFER, pGame->sound.gameMusicBuffer);

    alSourcei(pGame->sound.gameExplosionSource, AL_BUFFER, pGame->sound.gameExplosionBuffer);

    alSourcei(pGame->sound.gameVictorySource, AL_BUFFER, pGame->sound.gameVictoryBuffer);

    alSourcei(pGame->sound.gameoverSource, AL_BUFFER, pGame->sound.gameoverBuffer);

    alSourcei(pGame->sound.gameEatingSource, AL_BUFFER, pGame->sound.gameEatingBuffer);


}

void ClearGameDataSound(struct GAME *pGame, int tMlls) {

        Sleep(tMlls); // attendre que le son se termine

        alDeleteBuffers(1, &pGame->sound.gameMusicBuffer);
        alDeleteBuffers(1, &pGame->sound.gameExplosionBuffer);
        alDeleteBuffers(1, &pGame->sound.gameVictoryBuffer);
        alDeleteBuffers(1, &pGame->sound.gameoverBuffer);
        alDeleteBuffers(1, &pGame->sound.gameEatingBuffer);

        free(pGame->sound.gameMusic);
        free(pGame->sound.gameExplosion);
        free(pGame->sound.gameVictory);
        free(pGame->sound.gameGameover);
        free(pGame->sound.gameEating);

        alDeleteSources(1, &pGame->sound.gameMusicSource);
        alDeleteSources(1, &pGame->sound.gameExplosionSource);
        alDeleteSources(1, &pGame->sound.gameVictorySource);
        alDeleteSources(1, &pGame->sound.gameoverSource);
        alDeleteSources(1, &pGame->sound.gameEatingSource);

        alcMakeContextCurrent(NULL);
        alcDestroyContext(pGame->sound.context);
        alcCloseDevice(pGame->sound.device);

}
/*void PlayGameSound(struct GAME *pGame, char* sound) {

    if(strcmp(sound, "music") == 0){
           // alSourcePlay(pGame->sound.gameMusicSource);


    }
    else if(strcmp(sound, "explosion") == 0){
            alSourcePlay(pGame->sound.gameExplosionSource);

    }
    else if(strcmp(sound, "victory") == 0){
            alSourcePlay(pGame->sound.gameVictorySource);

    }
    else if(strcmp(sound, "gameover") == 0){
            alSourcePlay(pGame->sound.gameoverSource);

    }

}*/
/*void LoadSound(char* sound) {

    if(strcmp(sound, "music") == 0){

    //    PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\music.wav", NULL, SND_ASYNC);
    }

    if(strcmp(sound, "explosion") == 0){
        PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\explosion.wav", NULL, SND_ASYNC);
    }

    else if(strcmp(sound, "gameover") == 0){
        PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\gameover.wav", NULL, SND_ASYNC);
    }

    else if(strcmp(sound, "youwin") == 0){
        PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\youwin.wav", NULL, SND_ASYNC);
    }

}*/

/*DWORD WINAPI LoadSound(LPVOID lparam) {
    if(strcmp(sound, "music") == 0){
        PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\explosion.wav", NULL, SND_ASYNC);

    }

   if(strcmp(sound, "explosion") == 0){
        PlaySound("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\explosion.wav", NULL, SND_SYNC);

    }

    else if(strcmp(sound, "gameover") == 0){
       PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\gameover.wav", NULL, SND_ASYNC);
    }

    else if(strcmp(sound, "youwin") == 0){
        PlaySoundA("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\youwibn.wav", NULL, SND_ASYNC);
    }
}*/

/*void InitSound(struct GAME *pGame) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Erreur SDL_Init: %s\n", SDL_GetError());
        // Handle SDL initialization error appropriately
        exit(EXIT_FAILURE);  // or return some error code, depending on your design
    }

    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) < 0) {
        fprintf(stderr, "Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);  // or return some error code
    }

    pGame->sound.music = Mix_LoadWAV("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\music.wav");
    pGame->sound.youwin = Mix_LoadWAV("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\youwin.wav");
    pGame->sound.gameover = Mix_LoadWAV("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\gameover.wav");
    pGame->sound.explosion = Mix_LoadWAV("C:\\Users\\ulyss\\Documents\\CB Projetcs\\Starun\\Soundbox\\wav\\explosion.wav");

    if (!pGame->sound.music || !pGame->sound.youwin || !pGame->sound.gameover || !pGame->sound.explosion) {
        fprintf(stderr, "Erreur chargement des fichiers audio: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        exit(EXIT_FAILURE);  // or return some error code
    }
}

void PlaySyncSound(struct GAME *pGame, char* sound) {
    if(strcmp(sound, "music") == 0){
        int channel = Mix_PlayChannel(-1, pGame->sound.music, 0);
        if (channel == -1) {
            fprintf(stderr, "Erreur Mix_PlayChannel: %s\n", Mix_GetError());
}
    }

    if(strcmp(sound, "explosion") == 0){
        Mix_PlayChannel(-1, pGame->sound.explosion, 0);

    }

    else if(strcmp(sound, "gameover") == 0){
        Mix_PlayChannel(-1, pGame->sound.gameover, 0);
    }

    else if(strcmp(sound, "youwin") == 0){
        Mix_PlayChannel(-1, pGame->sound.youwin, 0);
    }
}*/






