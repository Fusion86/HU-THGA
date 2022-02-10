#include "SoundLoader.hpp"

std::shared_ptr<sf::SoundBuffer> SoundLoader::m_loadSoundFile(const string& fileName)
{
    auto buf = std::make_shared<sf::SoundBuffer>();
    buf->loadFromFile(fileName);
    return buf;
}

const sf::SoundBuffer& SoundLoader::loadSoundFile(const string& filename)
{
begin:
    auto iterator = m_soundBufferMap.find(filename); // Search for existing Buffers
    if (iterator == m_soundBufferMap.end())
    {
        m_soundBufferMap.insert(std::make_pair(filename, m_loadSoundFile(filename))); // Insert new data
        goto begin;
    }
    else
    {
        return *iterator->second; // Return the corresponding OBJ parts
    }
}
