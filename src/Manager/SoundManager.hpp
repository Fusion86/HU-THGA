#pragma once

#include <map>
#include <string>
#include <deque>
#include <SFML/Audio.hpp>

class SoundManager
{
  private:
    std::map<std::string, sf::SoundBuffer> m_sounds;
    std::deque<sf::Sound>                  m_nonRepeatingSounds;
    std::vector<sf::Sound>                 m_playingSounds;

  public:
    SoundManager();

    /// Play the sound with the given ID
    /// \param id String ID to play
    ///
    /// \details Look at SoundManager to find/add SoundIDs
    /// \param repeat Should the sound be played on repeat
    void play(const std::string& id, bool repeat = false);

    /// Update the soundmanager
    ///
    /// \details Checks if the sound in the front of the queue has finished, so it can be removed from the playing queue
    /// Note that this can only check the front, since otherwise playing sounds would be disturbed
    void update();
};
