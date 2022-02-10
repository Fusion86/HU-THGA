#include "Worker.hpp"

std::shared_ptr<Building> Worker::getCurrentTargetBuilding() const
{
    return m_currentTargetBuilding;
}

void Worker::setCurrentTargetBuilding(std::shared_ptr<Building> cTargetBuilding)
{
    Worker::m_currentTargetBuilding = std::move(cTargetBuilding);
}

Worker::State Worker::getCurrentState() const
{
    return m_currentState;
}

void Worker::setCurrentState(Worker::State cState)
{
    Worker::m_currentState = cState;
}

void Worker::resetCurrentCollectCooldown()
{
    currentCollectCooldown = Globals::WORKER_COLLECTION_COOLDOWN_MS;
}

ResourceType Worker::getCurrentResource() const
{
    return m_currentResource;
}

void Worker::setCurrentResource(ResourceType cResource)
{
    Worker::m_currentResource = cResource;
}
