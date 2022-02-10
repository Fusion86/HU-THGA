#pragma once

#include <Misc/Logger.hpp>
#include <Spawnable/Spawnable.hpp>
#include <Manager/SoundManager.hpp>

enum class UnitType
{
    NONE,

    // Attack units
    SWORDSMAN,
    ARCHER,
    KNIGHT,
    RAM,

    // Defensive units
    CROSSBOWMAN,

    // Gathering units
    WORKER,

    // Stealing units
    RAIDER,
};

std::ostream& operator<<(std::ostream& os, const UnitType& type);

class Unit : public Spawnable
{
  public:
    /// Get the string version of a unit type
    /// \param t Type to stringify
    /// \return The string representation of the type
    static const char* typeString(UnitType t);

    const UnitType unitType;
    bool           inBuilding;
    float          attackCooldown = 0;
    bool           isAttacking    = false;
    Globals::side  currentMovementDirection;

    /// Create a unit
    /// \param gameEntity Entity to render for the unit
    /// \param type Type of unit to create
    /// \param currentLane Lane the unit will start on
    /// \param owner Owner of the unit
    Unit(std::shared_ptr<GameEntity> gameEntity, UnitType type, Globals::lane currentLane, Globals::side owner);

    /// Called when the unit dies, can be used to play the right death sound
    /// \param attacker Attacker that killed the unit
    /// \param hitPoints Damage that was done killing the unit
    /// \param soundManager Soundmanager, used for playing a death sound
    void onDeath(Spawnable& attacker, int hitPoints, SoundManager& soundManager) override;

    UnitType getUnitType() const;
};
