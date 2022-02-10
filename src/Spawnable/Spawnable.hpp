#pragma once

#include <Entity/GameEntity.hpp>
#include <Misc/Globals.hpp>
#include <Manager/SoundManager.hpp>
#include <Misc/Logger.hpp>
#include <Spawnable/Spawnable.hpp>
#include <utility>

class Spawnable
{
  protected:
    enum class SpawnableType
    {
        UNIT,
        CONSTRUCTION
    };

    std::shared_ptr<GameEntity> m_gameEntity;
    Globals::side               m_owner;
    SpawnableType               m_spawnableType;
    bool                        m_destroyed       = false;
    Globals::lane               m_currentLane     = Globals::lane::NONE;
    bool                        m_nonInteractable = false;

  protected:
    Spawnable(std::shared_ptr<GameEntity> mGameEntity, Globals::lane currentLane, Globals::side mOwner,
              SpawnableType type);

  public:
    int currentHealth = 0;

    /// Make the spawnable (non)interactable
    /// \param mNonInteractable If true, the spawnable will be noninteractable
    void setNonInteractable(bool mNonInteractable);

    /// Check if the spawnable is non interactable
    /// \return True if the spawnable is noninteractable
    [[nodiscard]] bool isNonInteractable() const;


    /// Set the spawnable's destroyed state
    /// \param destroyed The destroyed state
    void                                      setDestroyed(bool destroyed);
    [[nodiscard]] std::shared_ptr<GameEntity> getGameEntity() const;

    /// Get the current owner of the spawnable
    /// \return The owner
    [[nodiscard]] Globals::side getOwner() const;

    /// Define what to do when a spawnable is killed
    ///
    /// \details Note that this method needs to make sure the unit is removed (can be done by setting m_destroyed)
    /// \param attacker Attacker that killed the spawnable
    /// \param hitPoints Amount of hitpoints the spawnable was hit for when killed
    /// \param soundManager Soundmanager to use for playying dying sounds
    virtual void onDeath(Spawnable& attacker, int hitPoints, SoundManager& soundManager) = 0;

    /// Called when a spawnable is hit
    /// \param attacker Unit that attacked the spawnable
    /// \param hitPoints Damage that was done
    /// \param soundManager Soundmanager, to use for hit and death sounds
    virtual void onHit(Spawnable& attacker, int hitPoints, SoundManager& soundManager);

    /// Get the lane the spawnable is currently on
    /// \return  The lane
    [[nodiscard]] Globals::lane getCurrentLane() const;

    /// Change the lane the spawnable is currently on
    /// \param mCurrentLane The new lane
    void setCurrentLane(Globals::lane mCurrentLane);

    /// Check if the spawnable is destroyed
    /// \return True if the spawnable is destroyed
    [[nodiscard]] bool isDestroyed() const;
};
