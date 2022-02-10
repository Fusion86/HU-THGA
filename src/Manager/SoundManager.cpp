#include "SoundManager.hpp"
#include <ResourceLoading/SoundLoader.hpp>


SoundManager::SoundManager()
{
    m_sounds["main_score"]             = SoundLoader::loadSoundFile("Assets/Audio/Main_Score.ogg");
    m_sounds["archer_death"]           = SoundLoader::loadSoundFile("Assets/Audio/Archer_Death.ogg");
    m_sounds["swordsman_death"]        = SoundLoader::loadSoundFile("Assets/Audio/Swordsman_Death.ogg");
    m_sounds["construction_destroyed"] = SoundLoader::loadSoundFile("Assets/Audio/Construction_Destroyed.ogg");
}

void SoundManager::play(const std::string& id, bool repeat)
{
    if (repeat)
    {
        m_playingSounds.emplace_back(sf::Sound{m_sounds[id]});
        m_playingSounds.back().setLoop(repeat);
        m_playingSounds.back().play();
    }
    else
    {
        m_nonRepeatingSounds.emplace_back(sf::Sound{m_sounds[id]});
        m_nonRepeatingSounds.back().setLoop(repeat);
        m_nonRepeatingSounds.back().play();
    }
}

void SoundManager::update()
{
    if (!m_nonRepeatingSounds.empty() && m_nonRepeatingSounds.front().getStatus() == sf::SoundSource::Stopped)
    {
        m_nonRepeatingSounds.pop_front();
    }
}
