#pragma once

#include <iostream>
#include <vector>

/// Enum of all available resource types
enum class ResourceType
{
    FOOD,
    WOOD,
    GOLD,
    STONE
};

struct ResourcesCollection
{
    /// Vector of all resource types, for looping over types
    static inline const std::vector<ResourceType> allTypes = {ResourceType::FOOD, ResourceType::WOOD,
                                                              ResourceType::GOLD, ResourceType::STONE};

    int food  = 0;
    int wood  = 0;
    int gold  = 0;
    int stone = 0;

    /// Adds all resource counts of the right hand to those of the left hand
    ResourcesCollection& operator+=(const ResourcesCollection& rhs)
    {
        food += rhs.food;
        wood += rhs.wood;
        gold += rhs.gold;
        stone += rhs.stone;
        return *this;
    }

    friend ResourcesCollection operator+(const ResourcesCollection lhs, const ResourcesCollection& rhs)
    {
        ResourcesCollection x = lhs;
        x += rhs;
        return x;
    }

    ResourcesCollection& operator-=(const ResourcesCollection& rhs)
    {
        food -= rhs.food;
        wood -= rhs.wood;
        gold -= rhs.gold;
        stone -= rhs.stone;
        return *this;
    }

    friend ResourcesCollection operator-(ResourcesCollection lhs, const ResourcesCollection& rhs)
    {
        lhs -= rhs;
        return lhs;
    }

    /// Get the count of a resource by it's type
    /// \param rhs The type of resource
    /// \return The count of the resource
    int& operator[](const ResourceType& rhs)
    {
        switch (rhs)
        {
            case ResourceType::FOOD:
                return food;
            case ResourceType::WOOD:
                return wood;
            case ResourceType::GOLD:
                return gold;
            case ResourceType::STONE:
                return stone;
        }
    }

    const int& at(const ResourceType& rhs) const
    {
        switch (rhs)
        {
            case ResourceType::FOOD:
                return food;
            case ResourceType::WOOD:
                return wood;
            case ResourceType::GOLD:
                return gold;
            case ResourceType::STONE:
                return stone;
        }
    }

    bool operator<(const ResourcesCollection& rhs) const
    {
        return (food < rhs.food || wood < rhs.wood || gold < rhs.gold || stone < rhs.stone);
    }

    bool operator>(const ResourcesCollection& rhs) const
    {
        return (food > rhs.food && wood > rhs.wood && gold > rhs.gold && stone > rhs.stone);
    }

    friend std::ostream& operator<<(std::ostream& os, const ResourcesCollection& rhs)
    {
        os << "Food: " << rhs.food << "\tWood: " << rhs.wood << "\tStone:" << rhs.stone << "\tGold: " << rhs.gold;
        return os;
    }
};
