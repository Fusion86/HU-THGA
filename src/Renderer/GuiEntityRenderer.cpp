#include "GuiEntityRenderer.hpp"

#include <GL/glew.h>
#include <Entity/GuiEntity.hpp>
#include <OpenGL/OpenGLBuffer.hpp>

void GuiEntityRenderer::bind()
{
    m_shader->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GuiEntityRenderer::render(const BaseEntity& baseEntity)
{
    // Cast to 2D entity
    auto& entity = (GuiEntity&)baseEntity;

    // Uniforms
    m_shader->uploadUniform("u_pos", entity.getPosition());
    m_shader->uploadUniform("u_size", entity.getSize());
    m_shader->uploadUniform("u_color", entity.getColor());

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

void GuiEntityRenderer::unbind()
{
    glDisable(GL_BLEND);

    m_shader->unbind();
}