//
// Created by r3-manning on 29/01/2020.
//

#include "AudioManager.h"

/**
 *   @brief   Destructor
 *   @details Denitialises the soloud library
 */
AudioManager::~AudioManager()
{
  soloud.deinit();
}

/**
 *   @brief   Sets up the class
 *   @details Initialises the soloud library and sets up the audio files used in
 * the game
 *   @return  True if initialised correctly
 */
bool AudioManager::audioSetUp()
{
  if (soloud.init() != SoLoud::SO_NO_ERROR)
  {
    return false;
  }

  if (gameMusic.open("/data/Audio/Game Music.mp3"))
  {
    auto io_buffer = gameMusic.read();
    if (gameMusic_MP3.loadMem(io_buffer.as_unsigned_char(),
                              static_cast<unsigned int>(io_buffer.length),
                              false,
                              false))
    {
      return true;
    }
    gameMusic.close();
  }
  else
  {
    return false;
  }

  if (axe_Man.open("/data/Audio/Axe Man.mp3"))
  {
    auto io_buffer = axe_Man.read();
    if (axe_Man_MP3.loadMem(io_buffer.as_unsigned_char(),
                            static_cast<unsigned int>(io_buffer.length),
                            false,
                            false))
    {
      return true;
    }
    axe_Man.close();
  }
  else
  {
    return false;
  }

  if (Box.open("/data/Audio/Box.mp3"))
  {
    auto io_buffer = Box.read();
    if (Box_MP3.loadMem(io_buffer.as_unsigned_char(),
                        static_cast<unsigned int>(io_buffer.length),
                        false,
                        false))
    {
      return true;
    }
    Box.close();
  }
  else
  {
    return false;
  }

  if (Coin.open("/data/Audio/Coin.mp3"))
  {
    auto io_buffer = Coin.read();
    if (Coin_MP3.loadMem(io_buffer.as_unsigned_char(),
                         static_cast<unsigned int>(io_buffer.length),
                         false,
                         false))
    {
      return true;
    }
    Coin.close();
  }
  else
  {
    return false;
  }

  if (DownAFLoor.open("/data/Audio/Going Down A Floor.mp3"))
  {
    auto io_buffer = DownAFLoor.read();
    if (DownAFLoor_MP3.loadMem(io_buffer.as_unsigned_char(),
                               static_cast<unsigned int>(io_buffer.length),
                               false,
                               false))
    {
      return true;
    }
    DownAFLoor.close();
  }
  else
  {
    return false;
  }

  if (Heart.open("/data/Audio/Heart.mp3"))
  {
    auto io_buffer = Heart.read();
    if (Heart_MP3.loadMem(io_buffer.as_unsigned_char(),
                          static_cast<unsigned int>(io_buffer.length),
                          false,
                          false))
    {
      return true;
    }
    Heart.close();
  }
  else
  {
    return false;
  }

  if (HitHurt.open("/data/Audio/Hit_Hurt.mp3"))
  {
    auto io_buffer = HitHurt.read();
    if (HitHurt_MP3.loadMem(io_buffer.as_unsigned_char(),
                            static_cast<unsigned int>(io_buffer.length),
                            false,
                            false))
    {
      return true;
    }
    HitHurt.close();
  }
  else
  {
    return false;
  }

  if (PowerUp.open("/data/Audio/PowerUp.mp3"))
  {
    auto io_buffer = PowerUp.read();
    if (PowerUp_MP3.loadMem(io_buffer.as_unsigned_char(),
                            static_cast<unsigned int>(io_buffer.length),
                            false,
                            false))
    {
      return true;
    }
    PowerUp.close();
  }
  else
  {
    return false;
  }

  return true;
}
void AudioManager::playGameMusic()
{
  soloud.play(gameMusic_MP3);
}

void AudioManager::replayGameMusic(double deltaTime)
{
  if (time < 86)
  {
    time = time + deltaTime;
  }
  else if (time > 87)
  {
    soloud.play(gameMusic_MP3);
    time = 0;
  }

  if (time >= 86)
  {
    time++;
  }
}

void AudioManager::playAxeMan()
{
  soloud.play(axe_Man_MP3);
}

void AudioManager::playBox()
{
  soloud.play(Box_MP3);
}

/**
 *   @brief   Plays coin pickup audio
 */
void AudioManager::playCoin()
{
  soloud.play(Coin_MP3);
}

/**
 *   @brief   Plays audio for descending a floor
 */
void AudioManager::playDownAFloor()
{
  soloud.play(DownAFLoor_MP3);
}

/**
 *   @brief   Plays heart pickup audio
 */
void AudioManager::playHeart()
{
  soloud.play(Heart_MP3);
}

/**
 *   @brief   Plays hit audio
 */
void AudioManager::playHit()
{
  soloud.play(HitHurt_MP3);
}

/**
 *   @brief   Plays UI audio
 */
void AudioManager::playPowerUp()
{
  soloud.play(PowerUp_MP3);
}