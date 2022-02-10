#pragma once

#include <memory>
#include <list>
#include <vector>
#include <map>

#include <glm/glm.hpp>
using glm::vec3;

#include <Misc/ResourcesCollection.hpp>
#include <Misc/Globals.hpp>

class Construction;
class GuiManager;
enum class UnitType;
enum class BuildingType;
class BuildingListener;
class GridSquare;
class TerrainManager;
class GameEntityRenderer;
class Building;




struct BuildingStats
{
    int                 healthPoints;
    ResourcesCollection price;
    std::list<UnitType> spawnableUnits;
};

class ConstructionManager final
{
  private:
    std::vector<std::shared_ptr<Construction>> m_constructions;
    GuiManager&                                m_guiManager;
    std::map<BuildingType, BuildingStats>      m_buildingStats;
    std::list<BuildingListener*>               m_buildingListeners;
    bool                                       m_gridsEnabled = false;

    /// Spawn a gridsquare at the given position
    /// \param position
    std::shared_ptr<GridSquare> m_spawnGridSquare(vec3 position, Globals::lane lane);

    // TODO: A map with buildingStats, just as with UnitStats.

  public:

    ///
    /// \param gridsEnabled
    void setGridsEnabled(bool gridsEnabled);

    /// Create a constructionmanager
    /// \param mGuiManager A reference to the guimanager, used to create building windows
    /// \param terrainManager A reference to the terrainmanager, used to create buildings
    ConstructionManager(GuiManager& mGuiManager, TerrainManager& terrainManager);

    /// Update construcions, check if they are destroyed and fire events to the buildinglisteners when they are.
    /// \param deltaTime
    void update(float deltaTime);

    /// Get the vector of all constructions currently in the map
    ///
    /// \return A reference to the list containing pointers to all constructions
    std::vector<std::shared_ptr<Construction>>& getConstructions();

    /// Find the construction with the given ID
    ///
    /// \details When no construction is found with the given id, a nullpointer is returned
    /// \param id ID to look for
    /// \return A pointer to the requested construction
    std::shared_ptr<Construction> getConstructionById(const std::string& id);

    /// Try to buy and place a construction for a player
    /// \param resourceInventory A reference to the resource inventory of the player
    /// \param type Type of building to spawn
    /// \param position Position to spawn the building at
    /// \param square Gridsquare to spawn the building on
    /// \param owner The owner of the building
    /// \return A shared pointer to the built construction
    std::shared_ptr<Construction> tryBuyConstruction(ResourcesCollection& resourceInventory, BuildingType type,
                                                     vec3 position, GridSquare& square, Globals::side owner);

    /// Try to buy and place a construction for a player/opponent
    /// \param resourceInventory A reference to the resource inventory of the player
    /// \param type Type of building to spawn
    /// \param lane The lane to spawn the building on
    /// \param position Position to spawn the building at
    /// \param owner The owner of the building
    /// \return A shared pointer to the built construction
    std::shared_ptr<Construction> tryBuyConstruction(ResourcesCollection& resourceInventory, BuildingType type,
                                                     Globals::lane lane, vec3 position, Globals::side owner);

    /// Render all Constructions using the given renderer
    /// \param renderer Renderer to use
    void renderAll(GameEntityRenderer& renderer);

    /// Spawn a building for the player
    /// \param type Type of building to spawn
    /// \param position Position to spawn the building at
    /// \param square Gridsquare to associate the building with
    /// \param owner Owner of the building
    /// \return A shared pointer to the spawned building
    std::shared_ptr<Construction> spawnBuilding(BuildingType type, vec3 position, GridSquare& square,
                                                Globals::side owner);

    /// Spawn a building for the player/opponent
    /// \param type Type of building to spawn
    /// \param lane The lane to spawn the building on
    /// \param position Position to spawn the building at
    /// \param owner Owner of the building
    /// \return A shared pointer to the spawned building
    std::shared_ptr<Building> spawnBuilding(BuildingType type, Globals::lane lane, vec3 position, Globals::side owner);

    /// Get the building stats map
    ///
    /// \details This map contains all relevant building stats per building type
    /// \return The stats map
    [[nodiscard]] const std::map<BuildingType, BuildingStats>& getBuildingStats() const;

    /// Register a building listener to update when a building is spawned/destroyed
    /// \param listener The listener
    void addBuildingListener(BuildingListener* listener);
};
