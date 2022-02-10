#include "Goal.hpp"

#include <Manager/ConstructionManager.hpp>
#include <Manager/UnitManager.hpp>

Goal::Goal(std::vector<SubGoal<UnitType>> spawnUnits, std::vector<SubGoal<BuildingType>> makeBuildings, int priority,
           UnitManager& unitManager, ConstructionManager& constructionManager, float delay, float priorityRampupSeconds,
           bool repeat, float initialTime)
    : m_spawnUnits(std::move(spawnUnits)), m_makeBuildings(std::move(makeBuildings)),
      m_priority(priority), m_totalCost{}, m_delay(delay), m_priorityRampupSeconds(priorityRampupSeconds),
      m_repeat(repeat)
{
    if (initialTime == -1.0)
    {
        m_remainingTimeS = delay;
    }
    else
    {
        m_remainingTimeS = initialTime;
    }
    for (auto& unit : m_spawnUnits)
    {
        m_totalCost += unitManager.getUnitStats().at(unit.goal).price;
    }

    for (auto& building : m_makeBuildings)
    {
        m_totalCost += constructionManager.getBuildingStats().at(building.goal).price;
    }
}

//Goal::Goal(Goal& goal)
//    : m_priority(goal.m_priority), m_totalCost(goal.m_totalCost), m_delay(goal.m_delay),
//      m_priorityRampupSeconds(goal.m_priorityRampupSeconds), m_repeat(goal.m_repeat), m_remainingTimeS(goal.m_remainingTimeS)
//{
//    for (auto& unit : goal.m_spawnUnits)
//    {
//        m_spawnUnits.emplace_back(unit);
//    }
//
//    for (auto& building : goal.m_makeBuildings)
//    {
//        m_makeBuildings.emplace_back(building);
//    }
//}
ResourcesCollection Goal::getCost()
{
    return m_totalCost;
}
int Goal::getPriority(float deltaTime)
{
    if (m_delay == 0.0)
    {
        return m_priority;
    }
    m_remainingTimeS -= deltaTime / 1000;
    if (m_remainingTimeS < 0)
    {
        m_remainingTimeS = 0;
    }

    if (m_remainingTimeS < m_priorityRampupSeconds)
    {
        float partDone = (m_priorityRampupSeconds - m_remainingTimeS) / m_priorityRampupSeconds;
        return int(partDone * m_priority);
    }
    else
    {
        if (m_remainingTimeS > (m_delay - m_priorityRampupSeconds))
        {
            float partDone = (m_delay - m_remainingTimeS) / m_priorityRampupSeconds;
            return int(-partDone * m_priority);
        }
        return 0;
    }
}
const std::vector<SubGoal<UnitType>>& Goal::getSpawnUnits() const
{
    return m_spawnUnits;
}
const std::vector<SubGoal<BuildingType>>& Goal::getMakeBuildings() const
{
    return m_makeBuildings;
}
void Goal::goalPerformed()
{
    m_remainingTimeS = m_delay;
}
bool Goal::isRepeating()
{
    return m_repeat;
}
