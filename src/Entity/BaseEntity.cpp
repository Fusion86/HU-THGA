#include "BaseEntity.hpp"

#include <ResourceLoading/Texture.hpp>
#include <OpenGL/OpenGLBuffer.hpp>

BaseEntity::BaseEntity(std::string id, std::shared_ptr<OpenGLBuffer> buffer, Texture texture)
    : m_id(std::move(id)), m_buffer(std::move(buffer)), m_texture(texture)
{
}
const std::string& BaseEntity::getId()
{
    return m_id;
}

std::shared_ptr<OpenGLBuffer> BaseEntity::getBuffer()
{
    return m_buffer;
}

Texture BaseEntity::getTexture()
{
    return m_texture;
}

void BaseEntity::changeTexture(Texture texture)
{
    m_texture = texture;
}
