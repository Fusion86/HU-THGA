#include "GameEntityRenderer.hpp"
#include <Entity/GameEntity.hpp>
#include <Misc/Globals.hpp>
#include <OpenGL/OpenGLBuffer.hpp>

void GameEntityRenderer::setMatrices(mat4 viewMatrix, mat4 projectionMatrix)
{
    m_viewMatrix       = viewMatrix;
    m_projectionMatrix = projectionMatrix;
}

void GameEntityRenderer::update(GameEntity& entity, float delta)
{
    if (entity.animationDelta >= Globals::ANIMATION_DELTA)
    {
        entity.animationDelta = 0.0f;

        if (entity.currentFrame == (entity.totalFrames - 1))
        {
            entity.currentFrame = 0;
        }
        else
        {
            entity.currentFrame++;
        }
    }
    else
    {
        entity.animationDelta += delta;
    }
}

void GameEntityRenderer::bind()
{
    m_shader->bind();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader->uploadUniform("u_viewMatrix", m_viewMatrix);
    m_shader->uploadUniform("u_projectionMatrix", m_projectionMatrix);
}

void GameEntityRenderer::unbind()
{
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    m_shader->unbind();
}

void GameEntityRenderer::render(const BaseEntity& baseEntity)
{
    // Cast to 3D entity

    auto entity = (GameEntity&)baseEntity;

    // Animation updates
    float uvPart   = (1.0f / float(entity.totalFrames));
    float uvOffset = uvPart * float(entity.currentFrame);

    // Model matrix
    mat4 modelMatrix = mat4(1.0f);
    modelMatrix      = glm::translate(modelMatrix, entity.getPosition());
    modelMatrix      = glm::rotate(modelMatrix, glm::radians(entity.getRotation().x), vec3(1.0f, 0.0f, 0.0f));
    modelMatrix      = glm::rotate(modelMatrix, glm::radians(entity.getRotation().y), vec3(0.0f, 1.0f, 0.0f));
    modelMatrix      = glm::rotate(modelMatrix, glm::radians(entity.getRotation().z), vec3(0.0f, 0.0f, 1.0f));
    modelMatrix      = glm::scale(modelMatrix, entity.getSize());
    m_shader->uploadUniform("u_modelMatrix", modelMatrix);
    m_shader->uploadUniform("u_color", entity.getColor());
    m_shader->uploadUniform("u_uvPart", uvPart);
    m_shader->uploadUniform("u_uvOffset", uvOffset);

    // Bind
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, entity.getTexture());

    // Render
    glBindVertexArray(entity.getBuffer()->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, entity.getBuffer()->getVertexCount());

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}