#include "Defender.hpp"

std::shared_ptr<Building>& Defender::getCurrentTargetBuilding()
{
    return m_currentTargetBuilding;
}
void Defender::setCurrentTargetBuilding(const std::shared_ptr<Building>& currentTargetBuilding)
{
    m_currentTargetBuilding = currentTargetBuilding;
}
Defender::State Defender::getCurrentState() const
{
    return m_currentState;
}
void Defender::setCurrentState(Defender::State newState)
{
    m_currentState = newState;
}
