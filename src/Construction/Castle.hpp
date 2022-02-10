#pragma once

#include <memory>

#include <Misc/Globals.hpp>

#include <Construction/Building.hpp>
#include <Entity/TerrainEntity.hpp>


class Castle : public Building
{
  public:
    /// Create a castle
    ///
    /// \param position Position the castle should reside on
    /// \param currentLane Lane the castle is on
    /// \param owner Owner of the castle
    Castle(const std::shared_ptr<TerrainEntity>& gameEntity, Globals::lane currentLane,
           Globals::side owner = Globals::side::LEFT)
        : Building(gameEntity, currentLane, BuildingType::CASTLE, owner)
    {
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        return "castleWindow";
    }
};
