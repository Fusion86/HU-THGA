#pragma once

#include <vector>
#include <string>
#include <list>

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/ResourcesCollection.hpp>
#include <Misc/Globals.hpp>

#include <Listener/UnitListener.hpp>



enum class UnitType;
class ConstructionManager;
class SoundManager;
class Unit;
class BaseRenderer;
class Spawnable;



struct Animation
{
    static inline const char* IDLE      = "idle";
    static inline const char* WALKING   = "walking";
    static inline const char* ATTACKING = "attacking";

    std::string id;
    int         frameCount;
};

struct UnitStats
{
    /// How much damage this unit can sustain.
    int healthPoints;

    /// How much damage this unit does vs other units (before effectiveAgainst and ineffectiveAgainst).
    int attackPoints;

    /// How much damage this units does vs buildings.
    int damageAgainstBuildings;

    /// How fast units move over the map.
    int movementStats;

    /// Does 1.5x damage against these units.
    std::vector<UnitType> effectiveAgainst;

    /// Does 0.5 damage against these units.
    std::vector<UnitType> ineffectiveAgainst;

    /// Attack Range.
    float attackRange = 0.5;

    /// How often this unit attacks (deals damage) per second.
    ///
    /// DPS = attackSpeed * attackPoints * (effective_mult)
    /// effective_mult = 1 default | 1.5 effectiveAgainst | 0.5 ineffectiveAgainst
    float attackSpeed = 0.5;

    /// Unit tier, used for selecting the sprite.
    int tier;

    /// How much it costs to produce one of this unit type.
    ResourcesCollection price;

    /// Animationdata
    std::vector<Animation> animations;
};

class GameplayManager;

class UnitManager final
{
  public:
    /// Create the unitmanager
    /// \param mConstructionManager constructionManager, used for
    /// \param mSoundManager
    UnitManager(ConstructionManager& mConstructionManager, SoundManager& mSoundManager);

    void update(float deltaTime, GameplayManager& gameplayManager);

    /// Spawn a unit of a given Type at the given position
    /// \param type UnitType to spawn
    /// \param position Position to spawn the unit at
    /// \param owner Owner of the unit
    /// TODO Add lane assignment to this function aswell
    void spawnUnit(UnitType type, vec3 position, Globals::lane lane, Globals::side owner);

    bool tryBuyUnit(ResourcesCollection& resourceInventory, UnitType type, vec3 position, Globals::lane lane,
                    Globals::side owner);

    /// Get a reference to the list of all Units
    /// \return A refernce to this list of units
    std::vector<std::shared_ptr<Unit>>& getUnits();

    /// Render all units using the given renderer
    /// \param renderer Renderer to use
    void renderAll(BaseRenderer& renderer);

  private:
    std::map<UnitType, UnitStats>      m_unitStats;
    std::vector<std::shared_ptr<Unit>> m_units;

    ConstructionManager&     m_constructionManager;
    SoundManager&            m_soundManager;
    std::list<UnitListener*> m_unitListeners;

    /// Perform AI for a simple attacker
    ///
    /// \details Basically just walks forward until it can attack something, and attack it
    /// \param attacker Attacking unit to update
    /// \param deltaTime
    void m_updateSimpleAttacker(Unit& attacker, float deltaTime);

    /// Perform AI for a simple destructor
    ///
    /// \details Basically just walks forward until it can attack something, and attack it
    /// \param destructor Destructing unit to update
    /// \param deltaTime
    void m_updateSimpleDestructor(Unit& destructor, float deltaTime);

    /// Update a collecting unit
    void m_updateCollector(const std::shared_ptr<Unit>& pUnit, float d, GameplayManager& gameplayManager);

    /// Update a defending unit
    ///
    /// \details If the unit needs to move into a building, move the unit towards the building, and enter it if in range
    /// If it has no construction assigned, it tries to return to the castle
    /// \param defender Defending unit to update
    /// \param deltaTime
    void m_updateDefender(const std::shared_ptr<Unit>& defender, float deltaTime);

    /// Try to attack a given unit
    /// \param distance Distance to the other unit, used to check if the unit is in attack range
    /// \param attacker Unit that will try to attack
    /// \param defender Unit the attacker will try to attack
    /// \param deltaTime
    /// \return True if the unit is within attack range (note, also returns true if the attacker is on cooldown)
    bool m_tryAttack(Unit& attacker, Spawnable& defender, float distance, int attackPoints, float deltaTime);

    /// Try to move forwards
    ///
    /// \details should check if the unit is about to walk "into" another friendly unit.
    bool m_tryMove(Unit& pUnit, float d, std::shared_ptr<Spawnable> target = std::shared_ptr<Spawnable>{});

    /// Change the animation for a unit, to the one with the given name
    /// \param pUnit Unit to change animation for
    /// \param id String id of the animation
    void m_changeAnimation(Unit& pUnit, const std::string& id);

    /// Change the animation for a unit, to the one with the given index in the animation array (mostly used internally)
    /// \param pUnit Unit to change animation for
    /// \param index Index of the animation
    void m_changeAnimation(Unit& pUnit, const int& index);

  public:
    [[nodiscard]] const std::map<UnitType, UnitStats>& getUnitStats() const;
    void                                               addUnitListener(UnitListener* listener);
};
