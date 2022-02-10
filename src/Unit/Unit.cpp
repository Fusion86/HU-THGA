#include "Unit.hpp"
#include <Spawnable/Spawnable.hpp>
#include <utility>

std::ostream& operator<<(std::ostream& os, const UnitType& type)
{
    return os << Unit::typeString(type);
}

Unit::Unit(std::shared_ptr<GameEntity> gameEntity, UnitType type, Globals::lane currentLane, Globals::side owner)
    : Spawnable(std::move(gameEntity), currentLane, owner, SpawnableType::UNIT), unitType(type),
      currentMovementDirection(Globals::invertSide(owner))
{
}

void Unit::onDeath(Spawnable& attacker, int hitPoints, SoundManager& soundManager)
{
    m_destroyed = true;
    std::cout << "Playing: " << Unit::typeString(unitType) + std::string{"_death"} << std::endl;
    soundManager.play(Unit::typeString(unitType) + std::string{"_death"});
}

const char* Unit::typeString(UnitType t)
{
    switch (t)
    {
        case UnitType::SWORDSMAN:
            return "swordsman";
        case UnitType::ARCHER:
            return "archer";
        case UnitType::KNIGHT:
            return "knight";
        case UnitType::CROSSBOWMAN:
            return "crossbowman";
        case UnitType::WORKER:
            return "worker";
        case UnitType::RAIDER:
            return "raider";
        case UnitType::RAM:
            return "ram";
        default:
            Logger::throwError("Given UnitType does not exist!");
            return "Wrong UnitType";
    }
}

UnitType Unit::getUnitType() const
{
    return unitType;
}
