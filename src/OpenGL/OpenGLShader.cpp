#include "OpenGLShader.hpp"

#include <fstream>
#include <sstream>

OpenGLShader::OpenGLShader(const string& vertexPath, const string& fragmentPath)
{
    m_name = vertexPath.substr(0, vertexPath.size() - 5);
    string        vertexCode;
    string        fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    m_vertexPath   = vertexPath;
    m_fragmentPath = fragmentPath;

    // Open the shader text files
    try
    {
        vShaderFile.open("./Assets/Shaders/" + m_vertexPath);
        fShaderFile.open("./Assets/Shaders/" + m_fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        Logger::throwError("Shader text files could not be opened!");
    }

    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    m_createProgram(vShaderCode, fShaderCode);
}

OpenGLShader::~OpenGLShader()
{
    unbind();
    glDeleteProgram(m_program);
}

void OpenGLShader::m_createProgram(const GLchar* vShaderCode, const GLchar* fShaderCode)
{
    // Compile the shaders
    GLuint vertex, fragment;
    GLint  success;
    GLchar infoLog[512]; // For errors

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Errors vertex shader
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        Logger::throwError("Shader error at " + m_vertexPath + ": " + infoLog);
    }

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Errors fragment shader
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        Logger::throwError("Shader error at " + m_fragmentPath + ": " + infoLog);
    }

    // Shader program
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);
    glLinkProgram(m_program);

    // Linking errors
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        Logger::throwError("Shader error at " + m_fragmentPath.substr(0, m_fragmentPath.size() - 5) + ": " + infoLog);
    }

    // Delete the no longer needed shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    Logger::throwInfo("Loaded vertex shader: " + m_vertexPath);
    Logger::throwInfo("Loaded fragment shader: " + m_fragmentPath);
}

void OpenGLShader::bind()
{
    glUseProgram(m_program);
}

void OpenGLShader::unbind()
{
    glUseProgram(0);
}