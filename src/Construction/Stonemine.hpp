#pragma once

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/Globals.hpp>
#include <ResourceLoading/EntityFactory.hpp>

#include <Construction/Building.hpp>

class Stonemine : public Building
{
  public:
    /// Create a stonemine
    /// \param position  Position the stonemine should be on
    /// \param currentLane Lane the stonemine is on
    /// \param owner Owner of the stonemine
    /// \param rotation Rotation of the model of the stonemine
    /// \param size Size of the model of the stonemine
    Stonemine(vec3 position, Globals::lane currentLane, Globals::side owner = Globals::side::LEFT,
              vec3 rotation = vec3(0.0f, 180.0f, 0.0f), vec3 size = vec3{1.0, 0.6, 1.0})
        : Building(EntityFactory::createGameEntity("stonemine", position, rotation, size, "buildings/stonemine.png",
                                                   "plane.obj"),
                   currentLane, BuildingType::STONEMINE, owner)
    {
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        return "stonemineWindow";
    }
};
