#include "TerrainEntityRenderer.hpp"

#include <GL/glew.h>
#include <Entity/TerrainEntity.hpp>
#include <OpenGL/OpenGLBuffer.hpp>

void TerrainEntityRenderer::setMatrices(mat4 viewMatrix, mat4 projectionMatrix)
{
    m_viewMatrix       = viewMatrix;
    m_projectionMatrix = projectionMatrix;
}

void TerrainEntityRenderer::bind()
{
    m_shader->bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    m_shader->uploadUniform("u_viewMatrix", m_viewMatrix);
    m_shader->uploadUniform("u_projectionMatrix", m_projectionMatrix);
}

void TerrainEntityRenderer::render(const BaseEntity& baseEntity)
{
    // Cast to 3D entity
    auto entity = dynamic_cast<const TerrainEntity&>(baseEntity);

    // Model matrix
    mat4 modelMatrix = mat4(1.0f);
    modelMatrix      = glm::translate(modelMatrix, entity.getPosition());
    modelMatrix      = glm::scale(modelMatrix, entity.getSize());
    m_shader->uploadUniform("u_modelMatrix", modelMatrix);

    // Uniforms
    m_shader->uploadUniform("u_uvRepeatR", entity.getBlendUvRepeatR());
    m_shader->uploadUniform("u_uvRepeatG", entity.getBlendUvRepeatG());
    m_shader->uploadUniform("u_uvRepeatB", entity.getBlendUvRepeatB());
    m_shader->uploadUniform("u_blendmapped", entity.isBlendmapped());
    m_shader->uploadUniform("u_color", entity.getColor());

    // Bind
    glBindVertexArray(entity.getBuffer()->getVAO());
    m_shader->uploadUniform("u_sampler_diffuse", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, entity.getTexture());
    m_shader->uploadUniform("u_sampler_blendMapR", 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, entity.getBlendR());
    m_shader->uploadUniform("u_sampler_blendMapG", 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, entity.getBlendG());
    m_shader->uploadUniform("u_sampler_blendMapB", 3);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, entity.getBlendB());

    // Render
    glDrawArrays(GL_TRIANGLES, 0, entity.getBuffer()->getVertexCount());

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void TerrainEntityRenderer::unbind()
{
    glDisable(GL_DEPTH_TEST);

    m_shader->unbind();
}