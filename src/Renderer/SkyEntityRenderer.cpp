#include "SkyEntityRenderer.hpp"

#include <GL/glew.h>
#include <Entity/SkyEntity.hpp>
#include <OpenGL/OpenGLBuffer.hpp>

void SkyEntityRenderer::setMatrices(mat4 viewMatrix, mat4 projectionMatrix)
{
    m_viewMatrix       = viewMatrix;
    m_projectionMatrix = projectionMatrix;
}

void SkyEntityRenderer::bind()
{
    m_shader->bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_shader->uploadUniform("u_viewMatrix", mat4(mat3(m_viewMatrix)));
    m_shader->uploadUniform("u_projectionMatrix", m_projectionMatrix);
}

void SkyEntityRenderer::render(const BaseEntity& baseEntity)
{
    // Cast to 3D entity
    auto entity = dynamic_cast<const SkyEntity&>(baseEntity);

    // Model matrix
    mat4 rotationMatrix = mat4(1.0f);
    rotationMatrix      = glm::rotate(rotationMatrix, glm::radians(entity.getRotation()), vec3(0.0f, 1.0f, 0.0f));
    m_shader->uploadUniform("u_rotationMatrix", rotationMatrix);

    // Bind
    m_shader->uploadUniform("u_sampler", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, entity.getTexture());

    // Render
    glBindVertexArray(entity.getBuffer()->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, entity.getBuffer()->getVertexCount());

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindVertexArray(0);
}

void SkyEntityRenderer::unbind()
{
    glDisable(GL_DEPTH_TEST);

    m_shader->unbind();
}