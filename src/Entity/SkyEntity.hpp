#pragma once

#include <Entity/BaseEntity.hpp>

/// \brief
/// This class is for a skybox entity
/// \details
/// You can use this entity for example for a 3D sky around the camera.
class SkyEntity final : public BaseEntity
{
    float m_rotation = 0.0f;
  public:

    /// Use the constructor of baseEntity
    using BaseEntity::BaseEntity;

    /// Set the current Rotation of the skybox
    /// \param rotation A vec3 containing the new rotation in Euler Angles
    void setRotation(float rotation);

    /// Get the current rotation of the skybox
    /// \return A vec3 containing the rotation in Euler Angles
    float getRotation();
};