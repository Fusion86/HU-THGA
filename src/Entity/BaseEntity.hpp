#pragma once

#include <memory>
#include <OpenGL/OpenGLBuffer.hpp>
#include <ResourceLoading/Texture.hpp>
#include <string>

/// \brief
/// This class is an abstract base class for entities in the game
/// \details
/// An entity needs at least an ID (so it is traceable), buffer and a texture.
class BaseEntity
{
    const std::string m_id;

    std::shared_ptr<OpenGLBuffer> m_buffer;

    Texture m_texture;

  public:
    /// Create a BaseEntity
    /// \param id Entity ID of the baseEntity
    /// \param buffer GLBuffer of the model to render
    /// \param texture Texture to put on the model
    BaseEntity(std::string id, std::shared_ptr<OpenGLBuffer> buffer, Texture texture);

    /// Get the entity ID
    /// \return The id
    const std::string& getId();

    /// Get the OpenGL buffer of the entity's model
    /// \return The buffer
    std::shared_ptr<OpenGLBuffer> getBuffer();

    /// Get the texture that is put on the model
    /// \return The texture
    Texture getTexture();

    /// Replace the texture that is put on the model
    /// \param texture The new texture
    virtual void changeTexture(Texture texture);
};
