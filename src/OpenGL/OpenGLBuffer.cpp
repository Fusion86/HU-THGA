#include "OpenGLBuffer.hpp"

OpenGLBuffer::OpenGLBuffer(float data[], unsigned int dataCount, ShapeType type)
{
    // Create buffers
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    // Bind buffers
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Allocate buffer memory
    glBufferData(GL_ARRAY_BUFFER, dataCount * sizeof(float), &data[0], GL_STATIC_DRAW);

    // Create vertex pointers
    if (type == ShapeType::SHAPE_2D)
    {
        m_vertexCount = dataCount / 4;
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else if (type == ShapeType::SHAPE_3D)
    {
        m_vertexCount = dataCount / 8;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    else if (type == ShapeType::SHAPE_CUBE)
    {
        m_vertexCount = dataCount;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
    }

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

OpenGLBuffer::~OpenGLBuffer()
{
    // TODO: Fix error here
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

const GLuint OpenGLBuffer::getVAO() const
{
    return m_vao;
}

const int OpenGLBuffer::getVertexCount() const
{
    return m_vertexCount;
}