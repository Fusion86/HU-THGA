#pragma once

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/Globals.hpp>
#include <ResourceLoading/EntityFactory.hpp>
#include <Construction/Building.hpp>


class Barrack : public Building
{
  public:
    /// Create a barrack
    ///
    /// \param position Position the barrack should reside on
    /// \param currentLane Lane the barrack is on
    /// \param owner Owner of the barrack
    /// \param rotation Should the texture be rotated
    /// \param size Size of the barrack
    Barrack(vec3 position, Globals::lane currentLane, Globals::side owner = Globals::side::LEFT,
            vec3 rotation = vec3(0.0f, 180.0f, 0.0f), vec3 size = vec3{0.7})
        : Building(EntityFactory::createGameEntity("barrack", position, rotation, size, "buildings/barrack.png",
                                                   "plane.obj"),
                   currentLane, BuildingType::BARRACK, owner)
    {
    }

    /// Get the menu ID of the CreationWindow that should open when this building is clicked
    ///
    /// \details Note that this menu id should be added in GuiManager
    /// \return A string containing the menu id
    string getBuildingMenuId() override
    {
        return "barrackWindow";
    }
};
