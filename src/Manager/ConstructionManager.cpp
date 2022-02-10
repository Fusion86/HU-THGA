#include "ConstructionManager.hpp"

#include <Misc/Globals.hpp>
#include <Misc/Logger.hpp>

#include <ResourceLoading/EntityFactory.hpp>

#include <Construction/Castle.hpp>
#include <Construction/GridSquare.hpp>
#include <Construction/Building.hpp>
#include <Construction/Barrack.hpp>
#include <Construction/Butchery.hpp>
#include <Construction/Goldmine.hpp>
#include <Construction/Stonemine.hpp>
#include <Construction/Woodshop.hpp>
#include <Construction/Tower.hpp>

#include <Manager/GuiManager.hpp>
#include <Manager/TerrainManager.hpp>

#include <Entity/GameEntity.hpp>
#include <Entity/BaseEntity.hpp>
#include <Unit/Unit.hpp>
#include <Listener/BuildingListener.hpp>
#include <Renderer/GameEntityRenderer.hpp>

ConstructionManager::ConstructionManager(GuiManager& mGuiManager, TerrainManager& terrainManager)
    : m_guiManager(mGuiManager)
{

    m_buildingStats[BuildingType::BARRACK] = {
        100,
        ResourcesCollection{100, 100, 0, 0},
        {UnitType::SWORDSMAN, UnitType::ARCHER, UnitType::KNIGHT, UnitType::CROSSBOWMAN, UnitType::RAM}};

    // NOTE: Health in the HUD doesn't change when you change the castle's health!
    m_buildingStats[BuildingType::CASTLE]    = {100, ResourcesCollection{0, 0, 0, 0}, {UnitType::WORKER}};
    m_buildingStats[BuildingType::BUTCHERY]  = {100, ResourcesCollection{100, 100, 0, 0}, {}};
    m_buildingStats[BuildingType::GOLDMINE]  = {100, ResourcesCollection{100, 100, 0, 0}, {}};
    m_buildingStats[BuildingType::STONEMINE] = {100, ResourcesCollection{100, 100, 0, 0}, {}};
    m_buildingStats[BuildingType::WOODSHOP]  = {100, ResourcesCollection{100, 100, 0, 0}, {}};
    m_buildingStats[BuildingType::TOWER]     = {100, ResourcesCollection{0, 100, 0, 150}, {}};

    for (auto& buildingType : m_buildingStats)
    {
        auto window = std::make_shared<CreationWindow>(Building::typeString(buildingType.first), Globals::GUI_SIZE);
        for (auto& unit : buildingType.second.spawnableUnits)
        {
            window->addSpawnButton(Unit::typeString(unit));
        }
        m_guiManager.addWindow(window);
    }

    auto castleSquare =
        m_spawnGridSquare(vec3(-25 + Globals::CASTLE_OFFSET - (Globals::HALF_MAP_SIZE / Globals::GRIDSQUARE_COUNT),
                               Globals::LANE_Y.at(Globals::lane::MID), Globals::LANE_Z.at(Globals::lane::MID)),
                          Globals::lane::MID);
    auto castle = std::make_shared<Castle>(terrainManager.getCastle(Globals::side::LEFT), Globals::lane::MID);
    castleSquare->setBuilding(castle);
    castleSquare->setSubItemHidden(true);
    castleSquare->setNonInteractable(true);
    /// Generate grid
    int laneCount = 3;
    for (int y = 0; y < laneCount; y++)
    {
        auto lane = (Globals::lane)(y + 1);
        for (int x = 0; x < (Globals::GRIDSQUARE_COUNT); x++)
        {
            m_spawnGridSquare(
                vec3(-25 + Globals::CASTLE_OFFSET + x * (Globals::HALF_MAP_SIZE / Globals::GRIDSQUARE_COUNT),
                     Globals::LANE_Y.at(lane), Globals::LANE_Z.at(lane)),
                lane);
        }
    }
}

void ConstructionManager::update(const float deltaTime)
{

    for (size_t i = 0; i < m_constructions.size(); i++)
    {
        if (m_constructions[i]->checkDestroyed())
        { // Check if the unit was destroyed
            std::shared_ptr<GridSquare> square = std::dynamic_pointer_cast<GridSquare>(m_constructions[i]);
            std::shared_ptr<Building>   building;
            if (square)
            {
                building = square->getBuilding();
            }
            else
            {
                building = std::dynamic_pointer_cast<Building>(m_constructions[i]);
            }

            if (building)
            {
                for (auto* listener : m_buildingListeners)
                {
                    listener->onBuildingDestroyed(building);
                }
            }

            m_constructions.erase(m_constructions.begin() + i);
            i--;
            continue;
        }
    }
}

std::vector<std::shared_ptr<Construction>>& ConstructionManager::getConstructions()
{
    return m_constructions;
}

std::shared_ptr<GridSquare> ConstructionManager::m_spawnGridSquare(vec3 position, Globals::lane lane)
{
    const std::shared_ptr<GameEntity>& gameEntity = EntityFactory::createGameEntity(
        "grid square", position, vec3(0.0f), vec3(0.4f, 0.4f, 0.4f), "gridSquare.png", "plane.obj");
    auto square = std::make_shared<GridSquare>(gameEntity, lane);
    m_constructions.push_back(square);
    return square;
}

std::shared_ptr<Construction> ConstructionManager::getConstructionById(const std::string& id)
{
    for (auto& construction : m_constructions)
    {
        if (construction->getGameEntity()->getId() == id)
        {
            return construction;
        }
    }
    return nullptr;
}

void ConstructionManager::renderAll(GameEntityRenderer& renderer)
{
    for (auto& construction : getConstructions())
    {
        auto grid = std::dynamic_pointer_cast<GridSquare>(construction);
        if (grid != nullptr) // If grid
        {
            if (m_gridsEnabled)
            {

                if (!grid->isSubItemHidden())
                {
                    if (grid->getBuilding() != nullptr)
                    {
                        renderer.render(*grid->getBuilding()->getGameEntity());
                    }
                    else
                    {
                        renderer.render(*construction->getGameEntity());
                    }
                }
            }
        }
        else
        {
            renderer.render(*construction->getGameEntity());
        }
    }
}

std::shared_ptr<Construction> ConstructionManager::tryBuyConstruction(ResourcesCollection& resourceInventory,
                                                                      BuildingType type, vec3 position,
                                                                      GridSquare& square, Globals::side owner)
{
    const auto& unitPrice = m_buildingStats[type].price;
    if (resourceInventory < unitPrice)
    {
        // Only show warnings for the player, because the bot calls this method a lot.
        if (owner == Globals::side::LEFT)
        {
            Logger::throwInfo("You don't have enough resources to buy this unit!\nPrice:\n", unitPrice,
                              "\nAvailable:\n", resourceInventory);
        }

        return std::shared_ptr<Construction>{};
    }
    else
    {
        resourceInventory -= unitPrice;
        return spawnBuilding(type, position, square, owner);
    }
}

std::shared_ptr<Construction> ConstructionManager::tryBuyConstruction(ResourcesCollection& resourceInventory,
                                                                      BuildingType type, Globals::lane lane,
                                                                      vec3 position, Globals::side owner)
{
    const auto& unitPrice = m_buildingStats[type].price;
    if (resourceInventory < unitPrice)
    {
        // Only show warnings for the player, because the bot calls this method a lot.
        if (owner == Globals::side::LEFT)
        {
            Logger::throwInfo("You don't have enough resources to buy this unit!\nPrice:\n", unitPrice,
                              "\nAvailable:\n", resourceInventory);
        }

        return std::shared_ptr<Construction>{};
    }
    else
    {
        resourceInventory -= unitPrice;
        auto building = spawnBuilding(type, lane, position, owner);
        m_constructions.emplace_back(building);
        return building;
    }
}

std::shared_ptr<Building> ConstructionManager::spawnBuilding(BuildingType type, Globals::lane lane, vec3 position,
                                                             Globals::side owner)
{
    std::string name = Building::typeString(type);

    std::shared_ptr<Building> building = nullptr;
    switch (type)
    {
        case BuildingType::BARRACK:
            building = std::make_shared<Barrack>(position, lane, owner);
            break;
        case BuildingType::CASTLE:
            Logger::throwError("Why are we spawning in a new castle, we shouldn't do that");
            break;
        case BuildingType::BUTCHERY:
            building = std::make_shared<Butchery>(position, lane, owner);
            break;
        case BuildingType::GOLDMINE:
            building = std::make_shared<Goldmine>(position, lane, owner);
            break;
        case BuildingType::WOODSHOP:
            building = std::make_shared<Woodshop>(position, lane, owner);
            break;
        case BuildingType::STONEMINE:
            building = std::make_shared<Stonemine>(position, lane, owner);
            break;
        case BuildingType::TOWER:
            std::cout << "SPAWNING TOWER\n";
            building = std::make_shared<Tower>(position, lane, owner);
            break;
        default:
            Logger::throwError("Trying to spawn building for which spawning method is unknown");
            break;
    }

    building->currentHealth = m_buildingStats[building->type].healthPoints;
    const char* hack        = owner == Globals::side::LEFT ? "left" : "right";
    Logger::throwInfo("Spawned unit '", type, "' for owner '", hack, "'");
    for (auto& listener : m_buildingListeners)
    {
        listener->onBuildingCreated(building);
    }
    return building;
}

std::shared_ptr<Construction> ConstructionManager::spawnBuilding(BuildingType type, vec3 position, GridSquare& square,
                                                                 Globals::side owner)
{

    auto building = spawnBuilding(type, square.getCurrentLane(), position, owner);
    square.setBuilding(building);
    return building;
}

const std::map<BuildingType, BuildingStats>& ConstructionManager::getBuildingStats() const
{
    return m_buildingStats;
}
void ConstructionManager::addBuildingListener(BuildingListener* listener)
{
    m_buildingListeners.emplace_back(listener);
}
void ConstructionManager::setGridsEnabled(bool gridsEnabled)
{
    m_gridsEnabled = gridsEnabled;
}
