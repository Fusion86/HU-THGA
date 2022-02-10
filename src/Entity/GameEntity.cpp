#include "GameEntity.hpp"
void GameEntity::setPosition(vec3 position)
{
    m_position = position;
}

void GameEntity::setRotation(vec3 rotation)
{
    m_rotation = rotation;
}

void GameEntity::setSize(vec3 size)
{
    m_size = size;
}

void GameEntity::setColor(vec3 color)
{
    m_color = color;
}

vec3 GameEntity::getPosition()
{
    return m_position;
}

vec3 GameEntity::getRotation()
{
    return m_rotation;
}

vec3 GameEntity::getSize()
{
    return m_size;
}

vec3 GameEntity::getColor()
{
    return m_color;
}

