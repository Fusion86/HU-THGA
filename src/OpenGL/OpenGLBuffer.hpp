#pragma once

#include <GL/glew.h>

enum class ShapeType
{
    SHAPE_2D,
    SHAPE_3D,
    SHAPE_CUBE
};

/// \brief
/// This class serves as the vertex buffer class for entities
/// \details
/// An OpenGL buffer is needed for an entity to render.
class OpenGLBuffer final
{
  public:
    /// OpenGL vertex buffering works as follows: You have a VBO (vertex buffer object)
    /// in which you store all of the data needed for rendering:
    /// position vertex (x,y,z) - UV/texture coordinate (x,y) - normal vector (x,y,z)
    /// data: a list of vertex data (positions, uv coordinates, normal vectors)
    /// dataCount: amount of position vertices
    OpenGLBuffer(float data[], unsigned int dataCount, ShapeType type);

    ~OpenGLBuffer();

    [[nodiscard]] const GLuint getVAO() const;

    [[nodiscard]] const int getVertexCount() const;

  private:
    int m_vertexCount;

    GLuint m_vao;
    GLuint m_vbo;
};
