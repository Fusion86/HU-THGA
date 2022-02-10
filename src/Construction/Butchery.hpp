#pragma once
#include <glm/glm.hpp>

#include <Misc/Globals.hpp>
#include <ResourceLoading/EntityFactory.hpp>

#include <Construction/Building.hpp>

using glm::vec3;

class Butchery : public Building
{
  public:
    /// Create a Butchery
    /// \param position Position to spawn on
    /// \param currentLane Lane the butchery is on
    /// \param owner Owner of the butchery
    /// \param rotation Rotation of the model (you can usually leave this at default)
    /// \param size Size of the model
    Butchery(vec3 position, Globals::lane currentLane, Globals::side owner = Globals::side::LEFT,
             vec3 rotation = vec3(0.0f, 180.0f, 0.0f), vec3 size = vec3{1.0, 0.8, 1.0})
        : Building(EntityFactory::createGameEntity("butchery", position, rotation, size, "buildings/butchery.png",
                                                   "plane.obj"),
                   currentLane, BuildingType::BUTCHERY, owner)
    {
    }

    string getBuildingMenuId() override
    {
        return "butcheryWindow";
    }

};
