#include "CameraManager.hpp"

#include <glm/gtc/matrix_transform.hpp>

CameraManager::CameraManager(vec3 position, float fov, int windowWidth, int windowHeight, float nearZ, float farZ)
    : m_position(position), m_fov(fov), m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_nearZ(nearZ),
      m_farZ(farZ)
{
}

void CameraManager::update()
{
    m_viewMatrix = glm::lookAt(m_position, m_position + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
    m_projectionMatrix =
        glm::perspective(glm::radians(m_fov), float(m_windowWidth) / float(m_windowHeight), m_nearZ, m_farZ);
}

void CameraManager::move(vec3 translation)
{
    m_position += translation;
}

void CameraManager::setPosition(vec3 position)
{
    m_position = position;
}
