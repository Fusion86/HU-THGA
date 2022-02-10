#pragma once

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/Globals.hpp>
#include <ResourceLoading/EntityFactory.hpp>

#include <Construction/Building.hpp>

class Woodshop : public Building
{
  public:
    /// Create a Woodshop
    /// \param position  Position the woodshop should be on
    /// \param currentLane Lane the woodshop is on
    /// \param owner Owner of the woodshop
    /// \param rotation Rotation of the model of the woodshop
    /// \param size Size of the model of the woodshop
    Woodshop(vec3 position, Globals::lane currentLane, Globals::side owner = Globals::side::LEFT,
             vec3 rotation = vec3(0.0f, 180.0f, 0.0f), vec3 size = vec3{0.6})
        : Building(EntityFactory::createGameEntity("woodshop", position, rotation, size, "buildings/woodshop.png",
                                                   "plane.obj"),
                   currentLane, BuildingType::WOODSHOP, owner)
    {
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        return "woodshopWindow";
    }
};
