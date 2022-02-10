#include "BuildingTrigger.hpp"

#include <algorithm>

BuildingTrigger::BuildingTrigger(std::list<BuildingType> triggeredBuildings, BuildingTrigger::Type triggerType,
                                 Globals::side owner, Goal goal, bool mPerformOnSameLane)
    : m_triggeredBuildings(std::move(triggeredBuildings)), m_triggerType(triggerType), m_owner(owner),
      m_goal(std::move(goal)), m_performOnSameLane(mPerformOnSameLane)
{
}

bool BuildingTrigger::checkTrigger(BuildingType building, BuildingTrigger::Type triggerType, Globals::lane lane,
                                   Globals::side owner)
{
    if (m_triggerType == triggerType || m_triggerType == Type::BOTH)
    {
        if (owner == m_owner)
        {
            if (std::find(m_triggeredBuildings.begin(), m_triggeredBuildings.end(), building) != m_triggeredBuildings.end())
            {

                if (m_performOnSameLane)
                {
                    for (auto& uGoal : m_goal.m_spawnUnits)
                    {
                        uGoal.lane = lane;
                    }
                    for (auto& bGoal : m_goal.m_makeBuildings)
                    {
                        bGoal.lane = lane;
                    }
                }
                return true;
            }
        }
    }
    return false;
}
std::shared_ptr<Goal> BuildingTrigger::getGoal()
{
    std::cout << "Making Building Trigger Goal" << std::endl;
    return std::make_shared<Goal>(m_goal);
}
