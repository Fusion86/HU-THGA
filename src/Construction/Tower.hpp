#pragma once

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/Globals.hpp>
#include <ResourceLoading/EntityFactory.hpp>

#include <Construction/Building.hpp>

class Tower : public Building
{
  public:
    /// Create a Tower
    /// \param position  Position the tower should be on
    /// \param currentLane Lane the tower is on
    /// \param owner Owner of the tower
    /// \param rotation Rotation of the model of the tower
    /// \param size Size of the model of the tower
    Tower(vec3 position, Globals::lane currentLane, Globals::side owner = Globals::side::LEFT,
          vec3 rotation = vec3(0.0f, 180.0f, 0.0f), vec3 size = vec3{1.0})
        : Building(
              EntityFactory::createGameEntity("tower", position, rotation, size, "buildings/tower.png", "plane.obj"),
              currentLane, BuildingType::TOWER, owner)
    {
        m_pathBlocking = true;
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        return "towerWindow";
    }
};
