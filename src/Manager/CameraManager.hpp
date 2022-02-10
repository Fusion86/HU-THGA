#pragma once

#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

/// \brief
/// This class manages the 3D camera (in the form of 4x4 matrices)
/// \details
/// You can manipulate the position the camera and calculate the new matrices.
class CameraManager final
{
    mat4  m_viewMatrix;
    mat4  m_projectionMatrix;
    vec3  m_position;
    int   m_windowWidth;
    int   m_windowHeight;
    float m_fov;
    float m_nearZ;
    float m_farZ;

  public:
    /// Create a cameramanager
    /// \param position the 3D camera position in the world
    /// \param fov the field of view of the camera (60-90 is default in most games)
    /// \param windowWidth needed to calculate the window aspect ratio
    /// \param windowHeight needed to calculate the window aspect ratio
    /// \param nearZ the nearest z position the camera should be seeing
    /// \param farZ the furthest z position the camera should be seeing
    CameraManager(vec3 position, float fov, int windowWidth, int windowHeight, float nearZ, float farZ);

    /// Updates the camera matrices (aka new camera movement/angle etc.)
    void update();

    /// Changes the camera position as follows: position + translation
    void move(vec3 translation);

    /// Change the camera position
    /// \param position A vec3 containing the new camera position
    void setPosition(vec3 position);

    /// Returns the view matrix (used to transform 3D positions according to where camera is looking at)
    inline const mat4& getViewMatrix()
    {
        return m_viewMatrix;
    }

    /// Returns the projection matrix (used to transform the camera view into perspective)
    inline const mat4& getProjectionMatrix()
    {
        return m_projectionMatrix;
    }

    /// Get the current camera position
    /// \return
    inline const vec3& getPosition()
    {
        return m_position;
    }
};
