#pragma once


#include <vector>
#include <list>
#include <memory>

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/Globals.hpp>
#include <Listener/BuildingListener.hpp>
#include <Listener/UnitListener.hpp>
#include <Opponent/Trigger/BuildingTrigger.hpp>
#include <Opponent/Trigger/UnitTrigger.hpp>

class UnitManager;
class ConstructionManager;
class GameplayManager;
class TerrainManager;
class Construction;
class Goal;

template<typename type>
class SubGoal;

class OpponentManager : public BuildingListener, public UnitListener
{

  private:
    UnitManager&         m_unitManager;
    ConstructionManager& m_constructionManager;
    GameplayManager&     m_gameplayManager;
    TerrainManager&      m_terrainManager;
    float                m_timeSinceLastTick = Globals::UPDATE_TICK_INTERVAL;
    int                  m_tickCounter       = 0;
    std::vector<std::vector<std::pair<vec3, std::shared_ptr<Construction>>>> m_buildingSpots;
    std::list<std::shared_ptr<Goal>>                                         goals;
    std::list<BuildingTrigger>                                               m_buildingTriggers;
    std::list<UnitTrigger>                                                   m_unitTriggers;
    ResourcesCollection                                                      m_workerDistribution = {10, 10, 5, 5};

    /// Try to perform the goal with the highest priority
    ///
    /// \details If the goal with the highest can be performed with the current resources, it is.
    /// Otherwise, worker distribution is adjusted to meet the most needed requirement for the goal.
    /// \param tickCounter Current total tickcount
    /// \param d Deltatime since last update
    void m_tryGoals(int tickCounter, const float d);

    /// Perform a goal
    ///
    /// \details Create all spawnables listed in a goal, and mark it as performed.
    /// If the goal is non-repeating, remove it from the goals list.
    /// \param goal  Goal to perform
    void                                            m_reachGoal(Goal& goal);

    /// Get the first open build position on a lane, according to a strategy
    ///
    /// \details Strategy can be front, back or random.
    /// Front places the building as close to the center of the map as possible
    /// Back places the building as close as possible to the player's castle
    /// Random chooses a random spot
    /// \param sGoal Building subgoal to build
    /// \param lane Lane to build on
    /// \return The position that was chosen, and the construction that was built
    std::pair<vec3, std::shared_ptr<Construction>>& m_getBuildPosition(const SubGoal<BuildingType>& sGoal,
                                                                       Globals::lane                lane);

    /// Check if a lane has free build spots
    /// \param lane Lane to check
    /// \return True if there are spots available on the lane
    bool                                            m_hasFreeSpots(Globals::lane lane);

    /// Check if a giving spot is available for building
    ///
    /// \details If the construction that is currently on the spot is destroyed, the pointer to it is reset
    /// \param spot Spot to check
    /// \return True if the spot can be built on
    bool                                            m_spotFree(std::pair<vec3, std::shared_ptr<Construction>>& spot);

    /// Change current worker goal according to worker distribution
    void m_distributeWorkers();

    /// Create and add a goal to the list of goals
    ///
    /// \details For details see Goal::Goal
    /// Fills in the unitManager and constructionManager parameters
    /// \param spawnUnits
    /// \param makeBuildings
    /// \param priority
    /// \param delay
    /// \param priorityRampupSeconds
    /// \param repeat
    /// \param initialTime
    void m_addGoal(const std::vector<SubGoal<UnitType>>&     spawnUnits,
                   const std::vector<SubGoal<BuildingType>>& makeBuildings, int priority, float delay = 0.0,
                   float priorityRampupSeconds = 0.0, bool repeat = false, float initialTime = -1.0);

    /// Create a goal and return it
    ///
    /// \details For details see Goal::Goal
    /// Fills in the unitManager and constructionManager parameters
    /// \param spawnUnits
    /// \param makeBuildings
    /// \param priority
    /// \param delay
    /// \param priorityRampupSeconds
    /// \param repeat
    /// \param initialTime
    /// \return
    Goal m_makeGoal(const std::vector<SubGoal<UnitType>>&     spawnUnits,
                    const std::vector<SubGoal<BuildingType>>& makeBuildings, int priority, float delay = 0.0,
                    float priorityRampupSeconds = 0.0, bool repeat = false, float initialTime = -1.0);

    /// Create and add a building trigger to the list of triggers
    ///
    /// \details For details see BuildingTrigger::BuildingTrigger
    /// \param triggeredBuildings
    /// \param triggerType
    /// \param owner
    /// \param goal
    /// \param performOnSameLane
    void m_addBuildingTrigger(std::list<BuildingType> triggeredBuildings, BuildingTrigger::Type triggerType,
                              Globals::side owner, Goal goal, bool performOnSameLane = true);

    /// Create and add a unit trigger to the list of triggers
    ///
    /// \details For details see UnitTrigger::UnitTrigger
    /// \param triggeredBuildings
    /// \param triggerType
    /// \param owner
    /// \param goal
    /// \param performOnSameLane
    void m_addUnitTrigger(std::list<UnitType> triggeredUnits, UnitTrigger::Type triggerType, Globals::side owner,
                          Goal goal, bool performOnSameLane = true);

  public:
    /// Create the opponent manager
    /// \param unitManager Unitmanager, used for calculating costs, and registering the unit listener
    /// \param constructionManager Constructionmanager, used for calculating costs, and registering the construction listener
    /// \param mGameplayManager Gameplaymanager, used for getting the resource and worker counts
    /// \param mTerrainManager Terrainmanager, used to get the castle objects
    OpponentManager(UnitManager& unitManager, ConstructionManager& constructionManager,
                    GameplayManager& mGameplayManager, TerrainManager& mTerrainManager);

    /// Update the opponent AI and try to perform listed goals
    /// \param deltaTime Time since last update
    void update(float deltaTime);

    /// Called when a building is destroyed, tries to match the building to a trigger, and if needed, adds the trigger's goal to the goal list.
    /// \param building Pointer to the building that was destroyed
    void onBuildingDestroyed(std::shared_ptr<Building> building) override;

    /// Called when a building is created, tries to match the building to a trigger, and if needed, adds the trigger's goal to the goal list.
    /// \param building Pointer to the building that was created
    void onBuildingCreated(std::shared_ptr<Building> building) override;

    /// Called when a unit is spawned, tries to match the unit to a trigger, and if needed, adds the trigger's goal to the goal list.
    /// \param unit Pointer to the unit that was spawned
    void onUnitSpawned(std::shared_ptr<Unit> unit) override;

    /// Called when a unit is killed, tries to match the unit to a trigger, and if needed, adds the trigger's goal to the goal list.
    /// \param unit Pointer to the unit that was killed
    void onUnitKilled(std::shared_ptr<Unit> unit) override;
};
