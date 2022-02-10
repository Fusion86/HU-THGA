#pragma once

#include <glm/glm.hpp>
using glm::vec3;


#include <Entity/BaseEntity.hpp>


/// \brief
/// This class is for game entities (actors in the 3D world)
/// \details
/// You can use this entity for example for buildings or walking characters
class GameEntity : public BaseEntity
{

    vec3 m_position = vec3(0.0f);
    vec3 m_rotation = vec3(0.0f);
    vec3 m_size     = vec3(0.0f);
    vec3 m_color    = vec3(1.0f);

  public:
    /// Use the constructor of baseentity here aswell
    using BaseEntity::BaseEntity;

    unsigned int totalFrames    = 1;
    unsigned int currentFrame   = 0;
    float        animationDelta = 0.0f;

  private:
    bool m_isVisible = true;
    vec3 m_realSize;

  public:
    /// Change the position of the entity
    /// \param position The new position
    void setPosition(vec3 position);

    /// Change the rotation of the model
    /// \param rotation The new rotation of the model
    void setRotation(vec3 rotation);

    /// Change the size of the model
    /// \param size The new size of the model
    void setSize(vec3 size);

    /// Change the color of the model
    /// \param color The new color
    void setColor(vec3 color);

    /// Set object visibility. True means that the object is visible, false means invisible.
    void setVisible(bool visible)
    {
        // This is quite possible the most hacky code I've ever written.
        if (!m_isVisible && visible)
        {
            // Make invisible
            m_realSize = getSize();
            setSize(vec3{0.00001f});
        }
        else if (m_isVisible && !visible)
        {
            // Make visible
            setSize(m_realSize);
        }
    }

    /// Returns true when object is not hidden from view.
    bool isVisible()
    {
        return m_isVisible;
    }

    /// Get the current position of the entity
    /// \return A vec3 containing the position
    [[nodiscard]] vec3 getPosition();

    /// Get the current rotation of the model of the entity
    /// \return A vec3 containing the rotation
    [[nodiscard]] vec3 getRotation();

    [[nodiscard]] vec3 getSize();

    [[nodiscard]] vec3 getColor();

  private:

};
