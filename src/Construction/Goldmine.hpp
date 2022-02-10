#pragma once

#include <glm/glm.hpp>
using glm::vec3;

#include <ResourceLoading/EntityFactory.hpp>
#include <Misc/Globals.hpp>

#include <Construction/Building.hpp>

class Goldmine : public Building
{
  public:
    /// Create a Goldmine
    ///
    /// \param position Position the goldmine should reside on
    /// \param currentLane Lane the goldmine is on
    /// \param owner Owner of the goldmine
    /// \param rotation Should the texture be rotated
    /// \param size Size of the goldmine
    Goldmine(vec3 position, Globals::lane currentLane, Globals::side owner = Globals::side::LEFT,
             vec3 rotation = vec3(0.0f, 180.0f, 0.0f), vec3 size = vec3{1.0, 0.6, 1.0})
        : Building(EntityFactory::createGameEntity("goldmine", position, rotation, size, "buildings/goldmine.png",
                                                   "plane.obj"),
                   currentLane, BuildingType::GOLDMINE, owner)
    {
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        return "goldmineWindow";
    }
};
