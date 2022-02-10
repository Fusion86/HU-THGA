#pragma once
#include <memory>

#include <Misc/Globals.hpp>
#include <Misc/Logger.hpp>
#include <Spawnable/Spawnable.hpp>

class Construction : public Spawnable
{
  protected:
    bool m_pathBlocking = false;

  public:
    /// Is the construction path_blocking
    ///
    /// \details Units should stop walking when they bump into a path blocking structure
    /// \return
    [[nodiscard]] bool isPathBlocking() const
    {
        return m_pathBlocking;
    }

    /// Create a construction
    /// \param gameEntity GameEntity to use for rendering the construction
    /// \param currentLane  Lane the construction will spawn on
    /// \param owner Owner of the construction
    Construction(std::shared_ptr<GameEntity> gameEntity, Globals::lane currentLane,
                 Globals::side owner = Globals::side::LEFT)
        : Spawnable(std::move(gameEntity), currentLane, owner, SpawnableType::CONSTRUCTION)
    {
    }

    /// Get the game entity for rendering
    /// \return A gameentity
    std::shared_ptr<GameEntity> getGameEntity()
    {
        return m_gameEntity;
    }

    /// Called when the construction is destroyed
    ///
    /// \details Note that this method is usually automatically called by onHit
    /// \param attacker The unit the construction was hit by when being destroyed
    /// \param hitPoints The damage the last blow did
    /// \param soundManager A reference to the soundmanager, so we can play death sounds
    void onDeath(Spawnable& attacker, int hitPoints, SoundManager& soundManager) override
    {
        m_destroyed = true;
        Logger::throwInfo("Playing: construction_destroyed");
        soundManager.play("construction_destroyed");
    }

    /// Check if the construction is destroyed
    /// \return True if the construction was destroyed
    virtual bool checkDestroyed()
    {
        return m_destroyed;
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    virtual std::string getBuildingMenuId()
    {
        return "ThisIsNotAKnownBuilding";
    };
};
