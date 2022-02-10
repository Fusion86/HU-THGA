#pragma once

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include <Entity/GameEntity.hpp>
#include <GL/glew.h>

/// \brief
/// This class is for a terrain entity
/// \details
/// You can use this entity for example for a 3D terrain model with multiple textures.
/// This entity uses blendmapping: mapping colors from a texture onto the terrain surface
/// and converting the colors to parts of a normal texture (such as grass).
class TerrainEntity final : public GameEntity
{
    GLuint m_blendR = 0;
    GLuint m_blendG = 0;
    GLuint m_blendB = 0;

    vec2 m_blendUvRepeatR = vec2(0);
    vec2 m_blendUvRepeatG = vec2(0);
    vec2 m_blendUvRepeatB = vec2(0);

    bool m_isBlendmapped = false;

  public:
    /// Use the GameEntity's constructor
    using GameEntity::GameEntity;

    /// Set the textures to use when applying blend textures
    /// \param r Texture to replace the color red with
    /// \param g Texture to replace the color green with
    /// \param b  Texture to replace the color blue with
    void setBlendTextures(GLuint r, GLuint g, GLuint b);

    /// Set the repeat values for each blend texture
    ///
    /// \details Dictates how many times a texture should be repeated on a model
    /// \param r Repeat value for the red blend texture
    /// \param g Repeat value for the green blend texture
    /// \param b Repeat value for the blue blend texture
    void setBlendUvRepeats(vec2 r, vec2 g, vec2 b);

    /// For explanation, look att TerrainEntity::setBlendTextures
    GLuint getBlendR();

    /// For explanation, look att TerrainEntity::setBlendTextures
    GLuint getBlendG();

    /// For explanation, look att TerrainEntity::setBlendTextures
    GLuint getBlendB();

    /// For explanation, look att TerrainEntity::setBlendUvRepeats
    vec2 getBlendUvRepeatR();

    /// For explanation, look att TerrainEntity::setBlendUvRepeats
    vec2 getBlendUvRepeatG();

    /// For explanation, look att TerrainEntity::setBlendUvRepeats
    vec2 getBlendUvRepeatB();

    /// Is this entity using blend mapping
    bool isBlendmapped();
};
