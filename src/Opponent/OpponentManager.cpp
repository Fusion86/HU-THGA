//
// Created by niels on 26/01/2020.
//

#include "OpponentManager.hpp"

#include <Misc/StringStream.hpp>

#include <Unit/Unit.hpp>
#include <Construction/Building.hpp>
#include <Construction/Barrack.hpp>

#include <Manager/ConstructionManager.hpp>
#include <Manager/UnitManager.hpp>
#include <Opponent/LaneInsight.hpp>
#include <Manager/GameplayManager.hpp>
#include <Manager/TerrainManager.hpp>



OpponentManager::OpponentManager(UnitManager& unitManager, ConstructionManager& constructionManager,
                                 GameplayManager& mGameplayManager, TerrainManager& mTerrainManager)
    : m_unitManager(unitManager), m_constructionManager(constructionManager), goals{},
      m_gameplayManager(mGameplayManager), m_buildingSpots{{}, {}, {}}, m_terrainManager(mTerrainManager)
{
    for (int x = 0; x < Globals::GRIDSQUARE_COUNT - 1; x++)
    {
        float xPos = 0 + Globals::CASTLE_OFFSET + x * (Globals::HALF_MAP_SIZE / Globals::GRIDSQUARE_COUNT);
        for (int y = 0; y < 3; y++)
        {
            auto lane = (Globals::lane)(y + 1);
            m_buildingSpots[y].emplace_back(vec3{xPos, Globals::LANE_Y.at(lane), Globals::LANE_Z.at(lane)},
                                            std::shared_ptr<Construction>{});
        }
    }

    using uVec = std::vector<SubGoal<UnitType>>;
    using bVec = std::vector<SubGoal<BuildingType>>;

    // Woodshop, foodshop and a couple of workers
    m_addGoal({UnitType::WORKER, UnitType::WORKER},
              {{BuildingType::WOODSHOP, Place::BACK}, {BuildingType::BUTCHERY, Place::BACK}}, 500, 0, 0, false, 0);

    // Goldmine, stonemine and some more workers
    m_addGoal({UnitType::WORKER, UnitType::WORKER},
              {{BuildingType::STONEMINE, Place::BACK}, {BuildingType::GOLDMINE, Place::BACK}}, 499, 0, 0, false, 0);

    // Barrack on every lane at the beginning
    m_addGoal({},
              {{BuildingType::BARRACK, Globals::lane::TOP, Place::BACK},
               {BuildingType::BARRACK, Globals::lane::MID, Place::BACK},
               {BuildingType::BARRACK, Globals::lane::BOT, Place::BACK}},
              498, 0, 0, false, 0);

    // Wave of raiders
    m_addGoal(
        {UnitType::RAIDER, UnitType::RAIDER, UnitType::RAIDER, UnitType::RAIDER, UnitType::RAIDER, UnitType::RAIDER},
        {}, 100, rand() % 10 + 20, 5.0, true, rand() % 30 + 30);

    // Wave of raiders
    m_addGoal(
        {UnitType::SWORDSMAN, UnitType::RAIDER, UnitType::KNIGHT, UnitType::RAIDER, UnitType::ARCHER, UnitType::ARCHER},
        {}, 100, rand() % 50 + 20, 5.0, true, rand() % 60 + 30);

    // Continuous stream of workers
    m_addGoal({UnitType::WORKER}, {}, 500, 10.0, 3.0, true, 0);

    // Build a tower on lanes the enemy has barracks on
    auto g_mannedTower =
        m_makeGoal({UnitType::CROSSBOWMAN, UnitType::CROSSBOWMAN}, {{BuildingType::TOWER, Place::FRONT}}, 300);
    m_addBuildingTrigger({BuildingType::BARRACK}, BuildingTrigger::Type::CREATED, Globals::side::LEFT, g_mannedTower);

    // Replace destroyed barracks with new ones
    auto g_singleBarrack = m_makeGoal({}, {{BuildingType::BARRACK, Place::BACK}}, 300);
    m_addBuildingTrigger({BuildingType::BARRACK}, BuildingTrigger::Type::DESTROYED, Globals::side::RIGHT,
                         g_singleBarrack);

    auto g_ram = m_makeGoal({UnitType::RAM}, {}, 300, 20.0, 5.0, true, 0.0);
    m_addBuildingTrigger({BuildingType::TOWER}, BuildingTrigger::Type::CREATED, Globals::side::LEFT, g_ram);

    m_constructionManager.addBuildingListener(this);
    m_unitManager.addUnitListener(this);
}

void OpponentManager::update(const float deltaTime)
{
    // Tick system
    m_timeSinceLastTick += deltaTime;
    while (m_timeSinceLastTick >= Globals::UPDATE_TICK_INTERVAL)
    {
        m_tryGoals(m_tickCounter, m_timeSinceLastTick);
        m_distributeWorkers();
        m_tickCounter++;
        m_timeSinceLastTick -= Globals::UPDATE_TICK_INTERVAL;
    }
}
void OpponentManager::m_tryGoals(int tickCounter, const float d)
{
    std::shared_ptr<Goal> maxGoal;
    int                   maxVal = -1000;

    for (auto& goal : goals)
    {
        int priority = goal->getPriority(d);
        if (priority > maxVal)
        {
            maxGoal = goal;
            maxVal  = priority;
        }
    }

    if (!maxGoal)
    {
        return;
    }

    if (maxVal <= 0)
    {
        return;
    }
    const std::pair<ResourceType, float>& timeUntilGoal =
        LaneInsight::secondsUntilGoal(m_gameplayManager, maxGoal->getCost(), Globals::side::RIGHT);
    if (timeUntilGoal.second == 0)
    {
        auto res = m_gameplayManager.getResources(Globals::side::RIGHT);
        m_reachGoal(*maxGoal);
        if (!maxGoal->isRepeating())
        {
            goals.erase(std::find(goals.begin(), goals.end(), maxGoal));
        }
    }
    else
    {
        if (timeUntilGoal.second > 10)
        {
//            m_workerDistribution[timeUntilGoal.first]+= timeUntilGoal.second / 10;
        }
    }
}
void OpponentManager::m_reachGoal(Goal& goal)
{
    auto& resources = m_gameplayManager.getResources(Globals::side::RIGHT);
    for (auto& buildingGoal : goal.getMakeBuildings())
    {
        auto lane = buildingGoal.lane;
        if (lane == Globals::lane::NONE)
        {
            std::vector<Globals::lane> tried = {};
            while (tried.size() < 3)
            {
                lane = Globals::lane(rand() % 3 + 1);
                if (m_hasFreeSpots(lane))
                {
                    break;
                }
                tried.emplace_back(lane);
            }
            if (tried.size() == 3)
            {
                Logger::throwWarning("Ai wants to build but has no open spots");
                return;
            }
        }
        auto& buildPosition = m_getBuildPosition(buildingGoal, lane);
        std::cout << buildPosition.first << std::endl;
        buildPosition.second = m_constructionManager.tryBuyConstruction(resources, buildingGoal.goal, lane,
                                                                        buildPosition.first, Globals::side::RIGHT);
    }

    for (auto& unitGoal : goal.getSpawnUnits())
    {

        switch (unitGoal.goal)
        {
            case UnitType::WORKER:
                m_unitManager.tryBuyUnit(resources, UnitType::WORKER,
                                         m_terrainManager.getCastle(Globals::side::RIGHT)->getPosition(),
                                         Globals::lane::MID, Globals::side::RIGHT);
                break;
            default:
                Globals::lane lane = unitGoal.lane;
                if (unitGoal.lane == Globals::lane::NONE)
                {
                    lane = Globals::lane(rand() % 3 + 1);
                }
                auto barrack = LaneInsight::findFirstConstructionOfType<Barrack>(
                    Globals::side::RIGHT, m_constructionManager.getConstructions(), lane);
                if (!barrack)
                {
                    Logger::throwWarning("No barrack found");
                    return;
                }
                m_unitManager.tryBuyUnit(resources, unitGoal.goal, barrack->getGameEntity()->getPosition(), lane,
                                         Globals::side::RIGHT);
        }
    }

    goal.goalPerformed();
}

bool OpponentManager::m_hasFreeSpots(Globals::lane lane)
{
    for (auto& spot : m_buildingSpots[(int)lane - 1])
    {
        if (m_spotFree(spot))
        {
            return true;
        }
    }
    return false;
}

std::pair<vec3, std::shared_ptr<Construction>>& OpponentManager::m_getBuildPosition(const SubGoal<BuildingType>& sGoal,
                                                                                    Globals::lane                lane)
{
    // TODO lane placing strategies
    switch (sGoal.strategy)
    {
        case Place::RANDOM:
        {
            while (true)
            {
                auto& laneVector = m_buildingSpots[(int)lane - 1];
                int   index      = rand() % laneVector.size();
                if (!m_spotFree(laneVector[index]))
                {
                    continue;
                }
                return laneVector[index];
            }
            break;
        }
        case Place::FRONT:
            for (auto& spot : m_buildingSpots[(int)lane - 1])
            {
                if (m_spotFree(spot))
                    return spot;
            }
            break;
        case Place::BACK:
            for (size_t i = m_buildingSpots[(int)lane - 1].size() - 1; i >= 0; i--)
            {
                if (m_spotFree(m_buildingSpots[(int)lane - 1][i]))
                {
                    return m_buildingSpots[(int)lane - 1][i];
                }
            }
            break;
    }
    Logger::throwError("AI can't build a building (no spots free?)");
}

bool OpponentManager::m_spotFree(std::pair<vec3, std::shared_ptr<Construction>>& spot)
{
    if (spot.second)
    {
        if (spot.second->isDestroyed())
        {
            spot.second.reset();
        }
        else
        {
            return false;
        }
    }
    return true;
}

void OpponentManager::m_distributeWorkers()
{
    int   totalWorkers = LaneInsight::countUnits(UnitType::WORKER, m_unitManager.getUnits(), Globals::side::RIGHT);
    auto& goal         = m_gameplayManager.getWorkerGoal(Globals::side::RIGHT);
    auto& assigned     = m_gameplayManager.getWorkerAssignment(Globals::side::RIGHT);
    int   sumGoal      = goal.food + goal.wood + goal.gold + goal.stone;

    if (sumGoal != totalWorkers)
    {
        int distributionSum = m_workerDistribution.food + m_workerDistribution.wood + m_workerDistribution.gold +
                              m_workerDistribution.stone;
        int remainingWorkers = totalWorkers;

        float woodPerc  = float(m_workerDistribution.wood) / float(distributionSum);
        float goldPerc  = float(m_workerDistribution.gold) / float(distributionSum);
        float stonePerc = float(m_workerDistribution.stone) / float(distributionSum);

        goal.wood = int(float(totalWorkers) * woodPerc);
        remainingWorkers -= goal.wood;

        goal.stone = int(float(totalWorkers) * stonePerc);
        remainingWorkers -= goal.stone;

        goal.gold = int(float(totalWorkers) * goldPerc);
        remainingWorkers -= goal.gold;

        goal.food = remainingWorkers;
    }
}

void OpponentManager::onBuildingDestroyed(std::shared_ptr<Building> building)
{
    for (auto& trigger : m_buildingTriggers)
    {
        if (trigger.checkTrigger(building->type, BuildingTrigger::Type::DESTROYED, building->getCurrentLane(),
                                 building->getOwner()))
        {
            goals.emplace_back(trigger.getGoal());
        }
    }
}
void OpponentManager::onBuildingCreated(std::shared_ptr<Building> building)
{
    for (auto& trigger : m_buildingTriggers)
    {
        if (trigger.checkTrigger(building->type, BuildingTrigger::Type::CREATED, building->getCurrentLane(),
                                 building->getOwner()))
        {
            goals.emplace_back(trigger.getGoal());
        }
    }
}

void OpponentManager::onUnitSpawned(std::shared_ptr<Unit> unit)
{
    for (auto& trigger : m_unitTriggers)
    {
        if (trigger.checkTrigger(unit->getUnitType(), UnitTrigger::Type::SPAWNED, unit->getCurrentLane(),
                                 unit->getOwner()))
        {
            goals.emplace_back(trigger.getGoal());
        }
    }
}
void OpponentManager::onUnitKilled(std::shared_ptr<Unit> unit)
{
    for (auto& trigger : m_unitTriggers)
    {
        if (trigger.checkTrigger(unit->getUnitType(), UnitTrigger::Type::KILLED, unit->getCurrentLane(),
                                 unit->getOwner()))
        {
            goals.emplace_back(trigger.getGoal());
        }
    }
}

void OpponentManager::m_addGoal(const std::vector<SubGoal<UnitType>>&     spawnUnits,
                                const std::vector<SubGoal<BuildingType>>& makeBuildings, int priority, float delay,
                                float priorityRampupSeconds, bool repeat, float initialTime)
{

    goals.emplace_back(std::make_shared<Goal>(spawnUnits, makeBuildings, priority, m_unitManager, m_constructionManager,
                                              delay, priorityRampupSeconds, repeat));
}
void OpponentManager::m_addBuildingTrigger(std::list<BuildingType> triggeredBuildings,
                                           BuildingTrigger::Type triggerType, Globals::side owner, Goal goal,
                                           bool performOnSameLane)
{
    m_buildingTriggers.emplace_back(
        BuildingTrigger{std::move(triggeredBuildings), triggerType, owner, std::move(goal), performOnSameLane});
}
void OpponentManager::m_addUnitTrigger(std::list<UnitType> triggeredUnits, UnitTrigger::Type triggerType,
                                       Globals::side owner, Goal goal, bool performOnSameLane)
{

    m_unitTriggers.emplace_back(
        UnitTrigger{std::move(triggeredUnits), triggerType, owner, std::move(goal), performOnSameLane});
}
Goal OpponentManager::m_makeGoal(const std::vector<SubGoal<UnitType>>&     spawnUnits,
                                 const std::vector<SubGoal<BuildingType>>& makeBuildings, int priority, float delay,
                                 float priorityRampupSeconds, bool repeat, float initialTime)
{
    return Goal{spawnUnits, makeBuildings,         priority, m_unitManager, m_constructionManager,
                delay,      priorityRampupSeconds, repeat};
}
