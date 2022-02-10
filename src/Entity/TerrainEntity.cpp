#include "TerrainEntity.hpp"
GLuint TerrainEntity::getBlendR()
{
    return m_blendR;
}

GLuint TerrainEntity::getBlendG()
{
    return m_blendG;
}

GLuint TerrainEntity::getBlendB()
{
    return m_blendB;
}

vec2 TerrainEntity::getBlendUvRepeatR()
{
    return m_blendUvRepeatR;
}

vec2 TerrainEntity::getBlendUvRepeatG()
{
    return m_blendUvRepeatG;
}

vec2 TerrainEntity::getBlendUvRepeatB()
{
    return m_blendUvRepeatB;
}

bool TerrainEntity::isBlendmapped()
{
    return m_isBlendmapped;
}

void TerrainEntity::setBlendUvRepeats(vec2 r, vec2 g, vec2 b)
{
    m_blendUvRepeatR = r;
    m_blendUvRepeatG = g;
    m_blendUvRepeatB = b;
    m_isBlendmapped  = true;
}

void TerrainEntity::setBlendTextures(GLuint r, GLuint g, GLuint b)
{
    m_blendR        = r;
    m_blendG        = g;
    m_blendB        = b;
    m_isBlendmapped = true;
}

