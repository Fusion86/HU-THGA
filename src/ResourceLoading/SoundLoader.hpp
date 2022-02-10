#pragma once

#include <map>
#include <iostream>
#include <SFML/Audio/SoundBuffer.hpp>
#include <memory>

using std::string;

/// \brief
/// This class serves as a loader/manager of Sound file
/// \details
/// Stores audio data in SoundBuffers, so they can easily be played without drive access
class SoundLoader final
{
  private:
  public:
    SoundLoader() = delete;

    ~SoundLoader() = delete;

    /// Try to retrieve a cached soundbuffer, or read an audio file and place the buffer in the cache
    /// fileName: path to filename of a wav/ogg/flac file
    static const sf::SoundBuffer& loadSoundFile(const string& filename);

  private:
    /// Loads a sound file into a buffer
    /// fileName: path to filename of a wav/ogg/flac file
    static std::shared_ptr<sf::SoundBuffer> m_loadSoundFile(const string& fileName);

    static inline std::map<string, std::shared_ptr<sf::SoundBuffer>> m_soundBufferMap = {};
};