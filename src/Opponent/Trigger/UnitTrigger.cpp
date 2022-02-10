#include "UnitTrigger.hpp"

#include <algorithm>

UnitTrigger::UnitTrigger(std::list<UnitType> triggeredUnits, UnitTrigger::Type triggerType, Globals::side owner,
                         Goal goal, bool mPerformOnSameLane)
    : m_triggeredUnits(std::move(triggeredUnits)), m_triggerType(triggerType), m_goal(std::move(goal)),
      m_performOnSameLane(mPerformOnSameLane), m_owner(owner)
{
}

bool UnitTrigger::checkTrigger(UnitType unit, UnitTrigger::Type triggerType, Globals::lane lane, Globals::side owner)
{
    if (m_triggerType == triggerType || m_triggerType == Type::BOTH)
    {
        if (owner == m_owner)
        {
            if (std::find(m_triggeredUnits.begin(), m_triggeredUnits.end(), unit) != m_triggeredUnits.end())
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
std::shared_ptr<Goal> UnitTrigger::getGoal()
{
    return std::make_shared<Goal>(m_goal);
}
