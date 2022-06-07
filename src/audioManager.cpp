#include "audioManager.h"

/* MINI AUDIO */
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdio.h>

ma_engine meowEngine;
ma_engine roostEngine;
ma_engine collectEngine;
ma_engine powerupEngine;

int audioManager::initEngine(int id)
{
  ma_result result;
  ma_sound sound;

  if (id == 1)
    result = ma_engine_init(NULL, &meowEngine);
  if (id == 2)
    result = ma_engine_init(NULL, &roostEngine);
  if (id == 3)
    result = ma_engine_init(NULL, &collectEngine);
  if (id == 4)
    result = ma_engine_init(NULL, &powerupEngine);
  
  if (result != MA_SUCCESS) {
    return result;  // Failed to initialize the engine.
  }

  return 0;
}

void audioManager::uninitEngine()
{
  ma_engine_uninit(&meowEngine);
  ma_engine_uninit(&roostEngine);
  ma_engine_uninit(&collectEngine);
  ma_engine_uninit(&powerupEngine);
}

void audioManager::playMeow()
{
  ma_engine_play_sound(&meowEngine, "../resources/meow.wav", NULL);
}

void audioManager::playRoost()
{
  ma_engine_play_sound(&roostEngine, "../resources/roost.wav", NULL);
}

void audioManager::playCollect()
{
  ma_engine_play_sound(&collectEngine, "../resources/collect.wav", NULL);
}

void audioManager::playPowerup()
{
  ma_engine_play_sound(&powerupEngine, "../resources/powerup.wav", NULL);
}