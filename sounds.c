#include "sounds.h"

ALshort* LoadSoundFromResource(int resourceID, ALsizei* dataSize) {
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(resourceID), RT_RCDATA);

    if (!hResource) {

        ShowGameError("[FATAL ERROR] Sorry, can't find specified sound !");
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
    alGenSources(1, &pGame->sound.gameBossSource);
    alGenSources(1, &pGame->sound.gameExtraCoinSource);
    alGenSources(1, &pGame->sound.gameImpactSource);


    alGenBuffers(1, &pGame->sound.gameMusicBuffer);
    alGenBuffers(1, &pGame->sound.gameExplosionBuffer);
    alGenBuffers(1, &pGame->sound.gameVictoryBuffer);
    alGenBuffers(1, &pGame->sound.gameoverBuffer);
    alGenBuffers(1, &pGame->sound.gameEatingBuffer);
    alGenBuffers(1, &pGame->sound.gameBossBuffer);
    alGenBuffers(1, &pGame->sound.gameExtraCoinBuffer);
    alGenBuffers(1, &pGame->sound.gameImpactBuffer);


    ALsizei musicDataSize, explosionDataSize, victoryDataSize, gameoverDataSize, gameEatingDataSize,
    gameBossDataSize, gameExtraCoinDataSize, gameImpactDataSize;

    pGame->sound.gameMusic = LoadSoundFromResource(IDR_WAVE1, &musicDataSize);
    pGame->sound.gameExplosion = LoadSoundFromResource(IDR_WAVE2, &explosionDataSize);
    pGame->sound.gameVictory = LoadSoundFromResource(IDR_WAVE3, &victoryDataSize);
    pGame->sound.gameGameover = LoadSoundFromResource(IDR_WAVE4, &gameoverDataSize);
    pGame->sound.gameEating = LoadSoundFromResource(IDR_WAVE5, &gameEatingDataSize);
    pGame->sound.gameBoss = LoadSoundFromResource(IDR_WAVE6, &gameBossDataSize);
    pGame->sound.gameExtraCoin = LoadSoundFromResource(IDR_WAVE7, &gameExtraCoinDataSize);
    pGame->sound.gameImpact = LoadSoundFromResource(IDR_WAVE8, &gameImpactDataSize);


    if (!pGame->sound.gameMusic || !pGame->sound.gameExplosion || !pGame->sound.gameEating
        || !pGame->sound.gameVictory || !pGame->sound.gameGameover || !pGame->sound.gameBoss
        || !pGame->sound.gameExtraCoin || !pGame->sound.gameImpact) {

        ShowGameError("[ERROR] Error loading audio files\n");

        alcMakeContextCurrent(NULL);

        alcDestroyContext(pGame->sound.context);

        alcCloseDevice(pGame->sound.device);
    }


    alBufferData(pGame->sound.gameMusicBuffer, AL_FORMAT_MONO16, pGame->sound.gameMusic, musicDataSize, 96000);

    alBufferData(pGame->sound.gameExplosionBuffer, AL_FORMAT_MONO16, pGame->sound.gameExplosion, explosionDataSize, 48000);

    alBufferData(pGame->sound.gameVictoryBuffer, AL_FORMAT_MONO16, pGame->sound.gameVictory, victoryDataSize, 96000);

    alBufferData(pGame->sound.gameoverBuffer, AL_FORMAT_MONO16, pGame->sound.gameGameover, gameoverDataSize, 48000);

    alBufferData(pGame->sound.gameEatingBuffer, AL_FORMAT_MONO16, pGame->sound.gameEating, gameEatingDataSize, 96000);

    alBufferData(pGame->sound.gameBossBuffer, AL_FORMAT_MONO16, pGame->sound.gameBoss, gameBossDataSize, 20000);

    alBufferData(pGame->sound.gameExtraCoinBuffer, AL_FORMAT_MONO16, pGame->sound.gameExtraCoin, gameExtraCoinDataSize, 96000);

    alBufferData(pGame->sound.gameImpactBuffer, AL_FORMAT_MONO16, pGame->sound.gameImpact, gameImpactDataSize, 48000);


    alSourcei(pGame->sound.gameExplosionSource, AL_BUFFER, pGame->sound.gameExplosionBuffer);

    alSourcei(pGame->sound.gameVictorySource, AL_BUFFER, pGame->sound.gameVictoryBuffer);

    alSourcei(pGame->sound.gameoverSource, AL_BUFFER, pGame->sound.gameoverBuffer);

    alSourcei(pGame->sound.gameEatingSource, AL_BUFFER, pGame->sound.gameEatingBuffer);

    alSourcei(pGame->sound.gameBossSource, AL_BUFFER, pGame->sound.gameBossBuffer);

    alSourcei(pGame->sound.gameExtraCoinSource, AL_BUFFER, pGame->sound.gameExtraCoinBuffer);

    alSourcei(pGame->sound.gameImpactSource, AL_BUFFER, pGame->sound.gameImpactBuffer);

    alSourcei(pGame->sound.gameMusicSource, AL_BUFFER, pGame->sound.gameMusicBuffer);

    alSourcei(pGame->sound.gameMusicSource, AL_LOOPING, AL_TRUE); //Au cas où le joueur prend son temps *)


}

void ClearGameDataSound(struct GAME *pGame, int tMlls) {

        Sleep(tMlls); // attendre que le son se termine

        alDeleteBuffers(1, &pGame->sound.gameMusicBuffer);
        alDeleteBuffers(1, &pGame->sound.gameExplosionBuffer);
        alDeleteBuffers(1, &pGame->sound.gameVictoryBuffer);
        alDeleteBuffers(1, &pGame->sound.gameoverBuffer);
        alDeleteBuffers(1, &pGame->sound.gameEatingBuffer);
        alDeleteBuffers(1, &pGame->sound.gameBossBuffer);
        alDeleteBuffers(1, &pGame->sound.gameBossBuffer);
        alDeleteBuffers(1, &pGame->sound.gameImpactBuffer);



        free(pGame->sound.gameMusic);
        free(pGame->sound.gameExplosion);
        free(pGame->sound.gameVictory);
        free(pGame->sound.gameGameover);
        free(pGame->sound.gameEating);
        free(pGame->sound.gameBoss);
        free(pGame->sound.gameExtraCoin);
        free(pGame->sound.gameImpact);



        alDeleteSources(1, &pGame->sound.gameMusicSource);
        alDeleteSources(1, &pGame->sound.gameExplosionSource);
        alDeleteSources(1, &pGame->sound.gameVictorySource);
        alDeleteSources(1, &pGame->sound.gameoverSource);
        alDeleteSources(1, &pGame->sound.gameEatingSource);
        alDeleteSources(1, &pGame->sound.gameBossSource);
        alDeleteSources(1, &pGame->sound.gameExtraCoinSource);
        alDeleteSources(1, &pGame->sound.gameImpactSource);



        alcMakeContextCurrent(NULL);
        alcDestroyContext(pGame->sound.context);
        alcCloseDevice(pGame->sound.device);

}
