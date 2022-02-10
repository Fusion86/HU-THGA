#pragma once

#include <memory>

#include <Misc/Globals.hpp>

#include <Manager/SoundManager.hpp>
#include <Entity/GameEntity.hpp>
#include <Construction/Construction.hpp>
#include <Construction/Building.hpp>



class GridSquare final : public Construction
{
    std::shared_ptr<Building> m_building;
    bool                      m_subItemHidden = false;

  public:
    /// Create a gridsquare
    /// \param gameEntity 2D entity to spawn for this gridsquare
    /// \param currentLane Lane to spawn the gridsquare on
    GridSquare(std::shared_ptr<GameEntity> gameEntity, Globals::lane currentLane)
        : Construction(std::move(gameEntity), currentLane), m_building(nullptr)
    {
    }

    /// Replace the building on this square
    ///
    /// \details Note that this does nothing with the old building, you probably want to destroy that first
    /// \param building The new building that is located on this square
    void setBuilding(std::shared_ptr<Building> building)
    {
        // TODO: Doesn't work on bottom lane??
        // getGameEntity()->setVisible(false); // Hide grid square
        m_building = std::move(building);
        m_building->getGameEntity()->setColor(getGameEntity()->getColor());
    }

    /// Get the building that is currently associated with this square
    /// \return The building
    std::shared_ptr<Building> getBuilding()
    {
        return m_building;
    }

    /// Change the color of the square, and it's associated building
    /// \param color  The new color
    void setColor(vec3 color)
    {
        m_gameEntity->setColor(color);
        if (m_building != nullptr)
        {
            m_building->getGameEntity()->setColor(color);
        }
    }

    /// Get the menu ID of the CreationWindow that should open when this square is clicked
    ///
    /// \details When there is a building on the square, this returns the buildingMenuId of that building
    /// Otherwise returns the buildWindow.
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        if (m_building != nullptr)
        {
            return m_building->getBuildingMenuId();
        }
        else
        {
            return "buildWindow";
        }
    }

    /// Lower the health when the square is attacked
    ///
    /// \details Calls the onHit function of the associated building
    /// \param attacker Unit that attacked the square
    /// \param hitPoints Damage the square was hit with
    /// \param soundManager Soundmanager, to use for playing a hit/deth sound
    void onHit(Spawnable& attacker, int hitPoints, SoundManager& soundManager) override
    {
        if (m_building != nullptr)
        {
            m_building->onHit(attacker, hitPoints, soundManager);
        }
    }

    /// Checks if the subItem is currently hidden
    /// \return
    bool isSubItemHidden() const
    {
        return m_subItemHidden;
    }

    /// Disable rendering for the building associated with this square
    ///
    /// \details Only used for castle
    /// \param dontRenderSubItem Should we hide the building?
    void setSubItemHidden(bool dontRenderSubItem)
    {
        GridSquare::m_subItemHidden = dontRenderSubItem;
    }

    /// Check if the building associated with this square is destroyed, if so, reset it's shared_ptr
    /// \return True if the square itself is destroyed
    bool checkDestroyed() override
    {
        if (m_building != nullptr && m_building->isDestroyed())
        {
            m_building = nullptr;
            // getGameEntity()->setVisible(true); // Make grid square visible again
        }
        return Construction::checkDestroyed();
    }
};
