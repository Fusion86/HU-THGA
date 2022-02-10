#pragma once

#include "../Misc/Logger.hpp"

#include <iostream>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using std::string;

/// \brief
/// This class serves as a shader handler for GLSL (OpenGL shader language)
/// \details
/// You can use this class to bind a GLSL shader and render with it.
/// It is also possible to send GLSL uniform values to the shader.
class OpenGLShader final
{
  public:
    /// A vertex shader is a shader that connects all vertices into a model. It does also do camera calculations.
    /// A fragment shader is a shader that gives color to the models. It does also do lighting calculations.
    /// vertexPath: file path for the vertex shader
    /// fragmentPath: file path for the fragment shader
    OpenGLShader(const string& vertexPath, const string& fragmentPath);

    ~OpenGLShader();

    /// Bind shader program
    void bind();

    /// Unbind shader program
    void unbind();

    /// Upload a uniform variable to the GLSL shader. You need to give the name of the uniform and
    /// also give the value itself. The value can be one of the following types:
    /// int, float, double, boolean, vec2, vec3, vec4, mat3, mat4
    template <class T>
    void uploadUniform(const string& name, const T& data)
    {
        GLint loc = m_getUniLoc(name);
        m_uploadUniform(loc, data);
    }

  private:
    GLuint                  m_program;
    std::map<string, GLint> m_uniformMap;
    string                  m_vertexPath;
    string                  m_fragmentPath;
    string                  m_name;

    /// Creates the shader program from the shaders source code
    void m_createProgram(const GLchar* vShaderCode, const GLchar* fShaderCode);

    /// Finds the uniform location within the shader
    GLint m_getUniLoc(const string& uniformName)
    {
        auto it = m_uniformMap.find(uniformName);
        if (it == m_uniformMap.end())
        { // Add location to the map
            GLint loc = glGetUniformLocation(m_program, uniformName.c_str());
            if (loc == -1)
            {
                Logger::throwWarning("Uniform " + uniformName + " not found in shader " + m_name);
            }
            m_uniformMap.insert(std::make_pair(uniformName, loc));
            return loc;
        }
        else
        {
            return it->second; // Return existing location
        }
    }

    // Overloading the upload functions
    inline void m_uploadUniform(const GLint& loc, const GLint& data)
    {
        glUniform1i(loc, data);
    }

    inline void m_uploadUniform(const GLint& loc, const GLfloat& data)
    {
        glUniform1f(loc, data);
    }

    inline void m_uploadUniform(const GLint& loc, const GLdouble& data)
    {
        glUniform1d(loc, data);
    }

    inline void m_uploadUniform(const GLint& loc, const GLboolean& data)
    {
        glUniform1i(loc, data);
    }

    inline void m_uploadUniform(const GLint& loc, const vec2& data)
    {
        glUniform2f(loc, data.x, data.y);
    }

    inline void m_uploadUniform(const GLint& loc, const vec3& data)
    {
        glUniform3f(loc, data.x, data.y, data.z);
    }

    inline void m_uploadUniform(const GLint& loc, const vec4& data)
    {
        glUniform4f(loc, data.x, data.y, data.z, data.w);
    }

    inline void m_uploadUniform(const GLint& loc, const mat3& data)
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(data));
    }

    inline void m_uploadUniform(const GLint& loc, const mat4& data)
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(data));
    }
};
