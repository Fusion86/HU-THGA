#pragma once

#include <Misc/Globals.hpp>
#include <Entity/GameEntity.hpp>
#include <Construction/Construction.hpp>


/// Possible types of buildings that can be spawned
enum class BuildingType
{
    BARRACK,
    CASTLE,
    BUTCHERY,
    GOLDMINE,
    WOODSHOP,
    STONEMINE,
    TOWER
};

class Building : public Construction
{
  public:
    const BuildingType type;

    /// Return the string representation of a building type
    /// \param t BuildingType
    /// \return String id
    static std::string typeString(const BuildingType& t)
    {
        switch (t)
        {
            case BuildingType::BARRACK:
                return "barrack";
            case BuildingType::CASTLE:
                return "castle";
            case BuildingType::BUTCHERY:
                return "butchery";
            case BuildingType::GOLDMINE:
                return "goldmine";
            case BuildingType::WOODSHOP:
                return "woodshop";
            case BuildingType::STONEMINE:
                return "stonemine";
            case BuildingType::TOWER:
                return "tower";
            default:
                return "ThisIsNotAKnownBuildingPleaseAddItToTypeStringMethod";
        }
    }

    /// Create a building
    /// \param gameEntity 2D entity to spawn for this building
    /// \param currentLane Lane to spawn the building on
    /// \param type Type of building
    /// \param owner Owner of the building
    Building(std::shared_ptr<GameEntity> gameEntity, Globals::lane currentLane, BuildingType type,
             Globals::side owner = Globals::side::LEFT)
        : Construction(std::move(gameEntity), currentLane, owner), type(type)
    {
    }
};

inline std::ostream& operator<<(std::ostream& os, const BuildingType& type)
{
    return os << Building::typeString(type);
}
