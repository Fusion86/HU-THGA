#pragma once

#include <glm/glm.hpp>
using glm::vec3;

#include <Unit/Unit.hpp>
#include <Spawnable/Spawnable.hpp>
#include <Construction/Construction.hpp>
#include <Construction/GridSquare.hpp>
#include <Manager/GameplayManager.hpp>


class LaneInsight
{
  private:
    /// Struct to store any spawnable and it's distance to a certain point
    /// \tparam SpawnableType
    template <typename SpawnableType>
    struct SpawnableDistance
    {
        SpawnableType spawnable;
        float         distance;
    };

  public:
    /// Get the closest construction to a given spawnable
    /// \tparam constructionType If set to anything non-default, the method will only return constructions of that type
    /// \param me Spawnable to find closest construction for
    /// \param collection Collection of constructions to look in (will usually be constructionManager.getConstructions())
    /// \param sameLane Should the function only check on the same lane as the spawnable
    /// \param friendly Should the function check for friendly or enemy units
    /// \param lookBackward Should the function look forward or backward
    /// \return The construction pointer and the distance to "me"
    template <typename constructionType = Construction>
    static SpawnableDistance<std::shared_ptr<constructionType>>
    getClosestConstruction(Spawnable& me, std::vector<std::shared_ptr<Construction>>& collection,
                                        bool sameLane = true, bool friendly = false, bool lookBackward = false)
    {
        Globals::side ignoreOwner     = friendly ? Globals::invertSide(me.getOwner()) : me.getOwner();
        Globals::side lookDirection   = lookBackward ? me.getOwner() : Globals::invertSide(me.getOwner());
        vec3          myPosition      = me.getGameEntity()->getPosition();
        float         closestDistance = 999999;
        std::shared_ptr<constructionType> closestSpawnable = nullptr;

        for (auto construction : collection)
        {
            if (construction->getOwner() == ignoreOwner || construction.get() == &me)
            { // Unit is of the wrong side
                continue;
            }

            if (construction->isNonInteractable())
            {
                continue;
            }

            auto spawnablePosition = construction->getGameEntity()->getPosition();
            if (sameLane && construction->getCurrentLane() != me.getCurrentLane())
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
                auto square = std::dynamic_pointer_cast<GridSquare>(construction);
                if (square)
                {
                    construction = square->getBuilding();
                }
                auto wantedConstruction = std::dynamic_pointer_cast<constructionType>(construction);
                if (!wantedConstruction)
                {
                    continue;
                }
                closestDistance  = distance;
                closestSpawnable = wantedConstruction;
            }
        }
        return SpawnableDistance<std::shared_ptr<constructionType>>{closestSpawnable, closestDistance};
    }

    /// Get the closest unit to a given spawnable
    /// \param me Spawnable to find closest construction for
    /// \param collection Collection of units to look in (will usually be unitManager.getUnits())
    /// \param sameLane Should the function only check on the same lane as the spawnable
    /// \param friendly Should the function check for friendly or enemy units
    /// \param lookBackward Should the function look forward or backward
    /// \return The unit pointer and the distance to "me"
    static SpawnableDistance<std::shared_ptr<Unit>> getClosestUnit(Spawnable&                          me,
                                                                   std::vector<std::shared_ptr<Unit>>& collection,
                                                                   bool sameLane = true, bool friendly = false,
                                                                   bool lookBackward = false);

    /// Get the construction with the lowest x value
    /// \tparam FindType Type of construction to find
    /// \param owner Owner of the construction
    /// \param collection Collection of constructions to look in (will usually be constructionManager.getConstructions())
    /// \param lane Lane to look in, if this is NONE, will look on all lanes
    /// \return The pointer to the building
    template <typename FindType>
    static std::shared_ptr<FindType> findFirstConstructionOfType(Globals::side                               owner,
                                                                 std::vector<std::shared_ptr<Construction>>& collection,
                                                                 Globals::lane lane = Globals::lane::NONE)
    {
        static_assert(std::is_base_of_v<Construction, FindType>);
        std::shared_ptr<FindType> firstFound{};
        float                     closestX = 10000;
        for (auto spawnable : collection)
        {
            if (lane != Globals::lane::NONE && spawnable->getCurrentLane() != lane)
            { // Check if the lane matches
                continue;
            }
            auto square = std::dynamic_pointer_cast<GridSquare>(spawnable);
            if (square)
            {
                spawnable = square->getBuilding();
            }

            auto wantedType = std::dynamic_pointer_cast<FindType>(spawnable);
            if (!wantedType)
            {
                continue;
            }

            if (spawnable->getOwner() != owner)
            {
                continue;
            }

            float currentX = spawnable->getGameEntity()->getPosition().x;
            if (currentX < closestX)
            {
                firstFound = wantedType;
                closestX   = currentX;
            }
        }
        return firstFound;
    }

    /// Get the horizontal distance between two spawnables
    ///
    /// \details Always returns positive values
    /// \param firstSpawnable
    /// \param secondSpawnable
    /// \return The distance between the two spawnables
    static float getHorizontalDistance(Spawnable& firstSpawnable, Spawnable& secondSpawnable);

    /// Get the side on which one spawnable is relative to the other
    /// \param firstSpawnable
    /// \param secondSpawnable
    /// \return LEFT if secondSpawnable is on the LEFT of firstSpawnable, RIGHT otherwise
    static Globals::side getDirection(Spawnable& firstSpawnable, Spawnable& secondSpawnable);

    /// Count the number of units of a given type and owner
    /// \param type Type to count
    /// \param units Collection of units to look in (will usually be unitManager.getUnits())
    /// \param side Owner to count for
    /// \return The count of the units with the given filter
    static size_t countUnits(UnitType type, const std::vector<std::shared_ptr<Unit>>& units,
                             Globals::side side = Globals::side::LEFT);

    /// Calculate how many of each resources are gained by a given player when their workerGoal is met
    /// \param gameplayManager Gameplaymanager used to get worker goals
    /// \param side Player to check resource increase for
    /// \return The amount of each resource gained per second
    static ResourcesCollection combinedResourceIncreasePerSecond(GameplayManager& gameplayManager, Globals::side side);

    /// Calculate how many seconds of resource gain a given side needs until a resource goal is met
    /// \param gameplayManager Gameplaymanager, used for calculating resource gains
    /// \param goal Goal that needs to be met
    /// \param side Player to check for
    /// \return A pair of the resourcetype that still needs the most time, and how much time it needs to reach the goal
    static std::pair<ResourceType, float> secondsUntilGoal(GameplayManager&           gameplayManager,
                                                           const ResourcesCollection& goal, Globals::side side);
};
