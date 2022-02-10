#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;

#include <OpenGL/OpenGLShader.hpp>
#include <Entity/BaseEntity.hpp>

/// \brief
/// This class serves as an abstract base renderer
/// \details
/// Every renderer is different, but they all need at least one thing: a shader program.
class BaseRenderer
{
  public:
    /// vertexShaderFile: file path for the vertex shader
    /// fragmentShaderFile: file path for the fragment shader
    BaseRenderer(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
        : m_shader(new OpenGLShader(vertexShaderFile, fragmentShaderFile))
    {
    }

    /// Pure virtual bind function every renderer must have. Can also possibly need matrices for camera
    virtual void bind() = 0;

    /// Pure virtual render function, always needing a base entity
    virtual void render(const BaseEntity& baseEntity) = 0;

    /// Pure virtual unbind function every renderer must have
    virtual void unbind() = 0;

  protected:
    std::shared_ptr<OpenGLShader> m_shader = nullptr;
};