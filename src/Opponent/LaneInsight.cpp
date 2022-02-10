#include "LaneInsight.hpp"
LaneInsight::SpawnableDistance<std::shared_ptr<Unit>>
LaneInsight::getClosestUnit(Spawnable& me, std::vector<std::shared_ptr<Unit>>& collection, bool sameLane, bool friendly,
                            bool lookBackward)
{
    Globals::side         ignoreOwner      = friendly ? Globals::invertSide(me.getOwner()) : me.getOwner();
    Globals::side         lookDirection    = lookBackward ? me.getOwner() : Globals::invertSide(me.getOwner());
    vec3                  myPosition       = me.getGameEntity()->getPosition();
    float                 closestDistance  = 999999;
    std::shared_ptr<Unit> closestSpawnable = nullptr;
    for (const auto& spawnable : collection)
    {
        if (spawnable->getOwner() == ignoreOwner || spawnable.get() == &me)
        { // Unit is of the wrong side
            continue;
        }

        if (spawnable->isNonInteractable())
        { // Unit is non interactable (probably in a building)
            continue;
        }

        auto spawnablePosition = spawnable->getGameEntity()->getPosition();
        if (sameLane && spawnable->getCurrentLane() != me.getCurrentLane())
        { // This spawnable is not on the same lane
            continue;
        }

        if (lookDirection == Globals::side::RIGHT && spawnablePosition.x < myPosition.x)
        { // This spawnable is on the wrong side
            continue;
        }

        if (lookDirection == Globals::side::LEFT && spawnablePosition.x > myPosition.x)
        { // This spawnable is on the wrong side
            continue;
        }

        float distance = std::abs(spawnablePosition.x - myPosition.x);
        if (distance < closestDistance)
        { // If the distance is lower than the previously known closest distance, store it
            closestDistance  = distance;
            closestSpawnable = spawnable;
        }
    }
    return SpawnableDistance<std::shared_ptr<Unit>>{closestSpawnable, closestDistance};
}



float LaneInsight::getHorizontalDistance(Spawnable& firstSpawnable, Spawnable& secondSpawnable)
{
    return fabs(firstSpawnable.getGameEntity()->getPosition().x - secondSpawnable.getGameEntity()->getPosition().x);
}

Globals::side LaneInsight::getDirection(Spawnable& firstSpawnable, Spawnable& secondSpawnable)
{
    float dir =
        (secondSpawnable.getGameEntity()->getPosition().x - firstSpawnable.getGameEntity()->getPosition().x);
    if (dir < 0)
    {
        return Globals::side::LEFT;
    }
    else
    {
        return Globals::side::RIGHT;
    }
}

size_t LaneInsight::countUnits(UnitType type, const std::vector<std::shared_ptr<Unit>>& units, Globals::side side)
{
    size_t count = 0;
    for (const auto& unit : units)
    {
        if (unit->unitType == type && unit->getOwner() == side)
        {
            count++;
        }
    }
    return count;
}

ResourcesCollection LaneInsight::combinedResourceIncreasePerSecond(GameplayManager& gameplayManager, Globals::side side)
{
    const auto& goal = gameplayManager.getWorkerGoal(side);

    float food = float(Globals::RESOURCE_PER_COLLECTION.at(ResourceType::FOOD)) /
                 (Globals::WORKER_COLLECTION_COOLDOWN_MS / 1000);
    float wood = float(Globals::RESOURCE_PER_COLLECTION.at(ResourceType::WOOD)) /
                 (Globals::WORKER_COLLECTION_COOLDOWN_MS / 1000);
    float stone = float(Globals::RESOURCE_PER_COLLECTION.at(ResourceType::STONE)) /
                  (Globals::WORKER_COLLECTION_COOLDOWN_MS / 1000);
    float gold = float(Globals::RESOURCE_PER_COLLECTION.at(ResourceType::GOLD)) /
                 (Globals::WORKER_COLLECTION_COOLDOWN_MS / 1000);

    food *= goal.food;
    wood *= goal.wood;
    stone *= goal.stone;
    gold *= goal.gold;

    return ResourcesCollection{int(food), int(wood), int(gold), int(stone)};
}

std::pair<ResourceType, float> LaneInsight::secondsUntilGoal(GameplayManager&           gameplayManager,
                                                             const ResourcesCollection& goal, Globals::side side)
{
    const auto& playerResources = gameplayManager.getResources(side);
    if (playerResources > goal)
    {
        return std::make_pair(ResourceType::FOOD, 0);
    }
    auto                resourcesPerSecond = combinedResourceIncreasePerSecond(gameplayManager, side);
    ResourcesCollection diff               = goal - playerResources;

    float        max     = float(diff.food) / float(resourcesPerSecond.food);
    ResourceType maxType = ResourceType::FOOD;

    float woodMax = float(diff.wood) / float(resourcesPerSecond.wood);
    if (woodMax > max)
    {
        maxType = ResourceType::WOOD;
    }

    float goldMax = float(diff.gold) / float(resourcesPerSecond.gold);
    if (goldMax > max)
    {
        maxType = ResourceType::GOLD;
    }

    float stoneMax = float(diff.stone) / float(resourcesPerSecond.stone);
    if (stoneMax > max)
    {
        maxType = ResourceType::STONE;
    }

    return std::make_pair(maxType, max);
}
