#include <Spawnable/Spawnable.hpp>

void Spawnable::setNonInteractable(bool mNonInteractable)
{
    m_nonInteractable = mNonInteractable;
}

Spawnable::Spawnable(std::shared_ptr<GameEntity> mGameEntity, Globals::lane currentLane, Globals::side mOwner,
                     Spawnable::SpawnableType type)
    : m_owner(mOwner), m_gameEntity(std::move(mGameEntity)), m_spawnableType(type), m_currentLane(currentLane)
{
}

std::shared_ptr<GameEntity> Spawnable::getGameEntity() const
{
    return m_gameEntity;
}

Globals::side Spawnable::getOwner() const
{
    return m_owner;
}

void Spawnable::onHit(Spawnable& attacker, int hitPoints, SoundManager& soundManager)
{
    if (hitPoints > currentHealth)
    {
        onDeath(attacker, hitPoints, soundManager);
    }
    else
    {
        currentHealth -= hitPoints;
    }
    // TODO maybe play a hit sound
}

Globals::lane Spawnable::getCurrentLane() const
{
    return m_currentLane;
}

void Spawnable::setCurrentLane(Globals::lane mCurrentLane)
{
    m_currentLane = mCurrentLane;
    m_gameEntity->setPosition(
        vec3(m_gameEntity->getPosition().x, Globals::LANE_Y.at(mCurrentLane), Globals::LANE_Z.at(mCurrentLane)));
}

bool Spawnable::isDestroyed() const
{
    return m_destroyed;
}

bool Spawnable::isNonInteractable() const
{
    return m_nonInteractable;
}
void Spawnable::setDestroyed(bool destroyed)
{
    m_destroyed = destroyed;
}
