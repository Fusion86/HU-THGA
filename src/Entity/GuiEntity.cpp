#include "GuiEntity.hpp"
void GuiEntity::setPosition(vec2 position)
{
    m_position = position;
}
void GuiEntity::setSize(vec2 size)
{
    m_size = size;
}
void GuiEntity::setClickable(bool val)
{
    m_clickable = val;
}
void GuiEntity::setHovered(bool val)
{
    m_hovered = val;
}
vec2 GuiEntity::getPosition()
{
    return m_position;
}
vec2 GuiEntity::getSize()
{
    return m_size;
}
bool GuiEntity::isClickable()
{
    return m_clickable;
}
bool GuiEntity::isHovered()
{
    return m_hovered;
}
vec3 GuiEntity::getColor()
{
    return m_color;
}
