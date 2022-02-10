#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <Misc/Globals.hpp>
using glm::vec3;
using glm::vec2;

class TerrainEntity;
class BaseRenderer;

class TerrainManager
{
  private:
    std::vector<std::shared_ptr<TerrainEntity>>             m_terrainItems;
    std::map<Globals::side, std::shared_ptr<TerrainEntity>> m_castles;

    /// Spawn a tree on the terrain at the given position
    /// \param position Position to spawn a tree on
    void m_spawnTree(vec3 position);

    /// Initialize terrain, loads in terrain items and spawns trees
    void m_initialize();

  public:
    /// Create the terrainmanager
    TerrainManager();

    /// Get the castle entity for a given side
    /// \param side Side to get the castle for
    /// \return A pointer to the castle
    [[nodiscard]] std::shared_ptr<TerrainEntity>& getCastle(Globals::side side);

    /// Render all terrain entities using the given renderer
    /// \param renderer Renderer to use
    void renderAll(BaseRenderer& renderer);
};
