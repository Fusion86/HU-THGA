#pragma once

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;

#include <Entity/BaseEntity.hpp>

/// \brief
/// This class is for gui entities (2D UI)
/// \details
/// You can use this entity for example for GUI buttons
class GuiEntity final : public BaseEntity
{
    vec2 m_position = vec2(0.0f);
    vec2 m_size     = vec2(0.0f);
    vec3 m_color    = vec3(1.0f);

    bool m_clickable = false;
    bool m_hovered   = false;
  public:
    /// Use the BaseEntity constructor for this entity
    using BaseEntity::BaseEntity;

    /// Change the position of the entity
    /// \param position The new position
    void setPosition(vec2 position);

    /// Change the size of the model
    /// \param size The new size of the model
    void setSize(vec2 size);

    /// Set the clickability of the Entity
    ///
    /// \details When an entity is non-clickable, it is ignored in cursor updates
    /// \param val Should the entity be clickable
    void setClickable(bool val);

    /// Mark an entity as hovered
    ///
    /// \details This is used internally when determining clicked/hovered items
    /// \param val Is the entity currently hovered
    void setHovered(bool val);

    /// Get the current position of the entity
    /// \return A vec3 containing the position
    vec2 getPosition();

    /// Get the current size of the model of the entity
    /// \return A vec3 containing the size
    vec2 getSize();

    /// Is this GuiEntity clickable
    ///
    /// \details See GuiEntity::setClickable for more information
    /// \return True if the entity is clickable
    bool isClickable();

    /// Is this GuiEntity currently hovered
    ///
    /// \details Mostly used internally, for more information look at GuiEntity::setHovered
    /// \return
    bool isHovered();

    /// Get the current color of the Entity
    /// \return A vec3 containing an RGB color
    vec3 getColor();
};
