#include "TerrainManager.hpp"

#include <ResourceLoading/EntityFactory.hpp>
#include <Renderer/BaseRenderer.hpp>


void TerrainManager::m_spawnTree(vec3 position)
{
    m_terrainItems.push_back(EntityFactory::createTerrainEntity("tree", position, vec3(0.3f), "terrain/tree.png", "",
                                                                "", "", vec2(5.0f, 5.0f), vec2(1.0f), vec2(),
                                                                "tree.obj", false));
}

void TerrainManager::m_initialize()
{

    // Terrain
    m_terrainItems.push_back(EntityFactory::createTerrainEntity(
        "mountain", vec3(0.0f, 0.0f, -7.0f), vec3(25.0f, 5.0f, 5.0f), "terrain/mountain.png", "terrain/grass.png",
        "terrain/stone.png", "terrain/path.png", vec2(5.0f, 5.0f), vec2(25.0f, 5.0f), vec2(25.0f, 5.0f), "mountain.obj",
        true));

    for (int i = 0; i < 5; i++)
    {
        // Lanes
        m_terrainItems.push_back(EntityFactory::createTerrainEntity(
            std::string{"lanes"} + std::to_string(i), vec3(-25.0 + (12.5 * i), 0.0f, -7.0f), vec3(5.0f, 5.0f, 5.0f),
            "terrain/lanes.png", "terrain/path.png", "terrain/grass.png", "terrain/rock.png", vec2(5.0f),
            vec2(25.0f, 5.0f), vec2(30.0f, 6.0f), "lanes.obj", true));
    }

    //    // Lanes
    //    m_terrainItems.push_back(EntityFactory::createTerrainEntity(
    //        "lanes", vec3(0.0f, 0.0f, -7.0f), vec3(5.0f, 5.0f, 5.0f), "terrain/lanes.png", "terrain/path.png",
    //        "terrain/grass.png", "terrain/rock.png", vec2(5.0f), vec2(25.0f, 5.0f), vec2(30.0f, 6.0f), "lanes.obj", true));
    //

    // Friendly castle
    m_castles[Globals::side::LEFT] = EntityFactory::createTerrainEntity(
        "friendlyCastle", vec3(-25.0f, 0.0f, -7.0f), vec3(5.0f), "terrain/castle.png", "terrain/castle_wall.jpg",
        "terrain/castle_door.png", "terrain/castle_window.png", vec2(10.0f), vec2(10.0f), vec2(0.0f), "castle.obj",
        true);
    m_terrainItems.push_back(m_castles[Globals::side::LEFT]);

    // Enemy castle
    m_castles[Globals::side::RIGHT] = EntityFactory::createTerrainEntity(
        "enemyCastle", vec3(25.0f, 0.0f, -7.0f), vec3(5.0f), "terrain/castle.png", "terrain/castle_wall.jpg",
        "terrain/castle_door.png", "terrain/castle_window.png", vec2(10.0f), vec2(10.0f), vec2(0.0f), "castle.obj",
        true);
    m_terrainItems.push_back(m_castles[Globals::side::RIGHT]);

    // Trees
    m_spawnTree(vec3(-21.0f, 5.5f, -7.5f));
    m_spawnTree(vec3(-18.0f, 8.5f, -8.5f));
    m_spawnTree(vec3(-15.0f, 9.0f, -8.5f));
    m_spawnTree(vec3(-13.0f, 11.0f, -9.5f));
    m_spawnTree(vec3(-10.0f, 10.0f, -8.5f));
    m_spawnTree(vec3(-7.0f, 5.5f, -7.5f));
    m_spawnTree(vec3(-4.0f, 9.5f, -8.5f));
    m_spawnTree(vec3(-1.0f, 5.5f, -7.5f));
    m_spawnTree(vec3(2.0f, 7.0f, -9.0f));
    m_spawnTree(vec3(5.0f, 9.75f, -9.0f));
    m_spawnTree(vec3(8.0f, 8.0f, -8.5f));
    m_spawnTree(vec3(11.0f, 6.0f, -8.5f));
    m_spawnTree(vec3(14.0f, 6.0f, -7.5f));
    m_spawnTree(vec3(17.0f, 5.0f, -7.5f));
}

TerrainManager::TerrainManager()
{
    m_initialize();
}

std::shared_ptr<TerrainEntity>& TerrainManager::getCastle(Globals::side side)
{
    return m_castles[side];
}

void TerrainManager::renderAll(BaseRenderer& renderer)
{
    for (const auto& entity : m_terrainItems)
    {
        renderer.render(*entity);
    }
}
