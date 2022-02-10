#pragma once

#include <iostream>

#include <Misc/Globals.hpp>
#include <Entity/SkyEntity.hpp>
#include <Entity/TerrainEntity.hpp>
#include <Entity/GameEntity.hpp>
#include <Entity/GuiEntity.hpp>

/// \brief
/// This class serves as factory pattern entity creator
/// \details
/// There are 4 types of entities: sky, terrain, game(actor), gui
class EntityFactory final
{
  public:
    EntityFactory() = delete;

    /// Creates and returns polymorphic pointer of type sky entity.
    /// id: unique string ID of entity
    /// left - back: filenames for sky textures
    static std::shared_ptr<SkyEntity> createSkyEntity(STRING id, STRING right, STRING left, STRING top, STRING bottom,
                                                      STRING back, STRING front);

    /// Creates and returns polymorphic pointer of type terrain entity.
    /// id: unique string ID of entity
    /// position: xyz position of terrain model
    /// size: xyz size of terrain model
    /// blendFile: filename of blendmap texture
    /// blendR - blendB: filenames for corresponding blendmap textures
    /// uvRepeatR - uvRepeatB: uv(texture) coordinate repeats for corresponding blendmap textures
    /// objFile: filename of .obj model file
    static std::shared_ptr<TerrainEntity> createTerrainEntity(STRING id, vec3 position, vec3 size, STRING textureFile,
                                                              STRING blendR, STRING blendG, STRING blendB,
                                                              vec2 uvRepeatR, vec2 uvRepeatG, vec2 uvRepeatB,
                                                              STRING objFile, bool blendmapping);

    /// Creates and returns polymorphic pointer of type game entity.
    /// id: unique string ID of entity
    /// position: xyz position of terrain model
    /// rotation: xyz rotation of terrain model
    /// size: xyz size of terrain model
    /// texFile: filename of model texture
    /// objFile: filename of .obj model file
    static std::shared_ptr<GameEntity> createGameEntity(STRING id, vec3 position, vec3 rotation, vec3 size,
                                                        STRING texFile, STRING objFile);

    /// Creates and returns polymorphic pointer of type game entity.
    /// id: unique string ID of entity
    /// x,y,w,h: position and size of GUI rectangle
    /// texFile: filename of model texture
    static std::shared_ptr<GuiEntity> createGuiEntity(STRING id, float x, float y, float w, float h, STRING texFile,
                                                      bool clickable, bool texfiltering);

  private:
    /// Create a new unitID for a unit with the given name
    /// \param name Typename of the unit to make an ID for
    /// \return A unit ID, consisting of a unit type and a globally incremented index
    static std::string m_createUnitId(std::string name);

    static inline int m_unitIdCounter = 0;
};
