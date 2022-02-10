#include "TextureLoader.hpp"

#include <SFML/Graphics/Image.hpp>
#include <Misc/Logger.hpp>

Texture TextureLoader::loadTexture(const string& fileName, bool mipmap, bool aniso)
{
    auto it = m_textureMap.find(fileName);
    if (it == m_textureMap.end()) // Not in map (yet)
    {
        m_textureMap.insert(std::make_pair(fileName, m_loadTexture(fileName, mipmap, aniso))); // Create
        return loadTexture(fileName, mipmap, aniso);                                           // Return
    }
    else
    {
        return it->second; // Cache texture
    }
}

Texture TextureLoader::loadCubemap(const std::vector<string>& fileNames)
{
    return m_loadCubemap(fileNames);
}

Texture TextureLoader::m_loadTexture(const string& fileName, bool mipmap, bool aniso)
{
    // Load actual texture file
    sf::Image surface;

    if (!surface.loadFromFile("./Assets/Textures/" + fileName))
    {
        Logger::throwError("Could not load texture file: " + fileName);
    }

    // Generate OpenGL texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Load SFML texturedata into OpenGL texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface.getSize().x, surface.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 surface.getPixelsPtr());

    // Mipmapping or not
    if (mipmap)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    // Anisotropic filtering
    if (aniso)
    {
        float temp;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &temp);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, temp);
    }

    // Repeat dimensions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

    // Logging
    Logger::throwInfo("Loaded PNG texture: " + fileName);

    // Return new texture
    return tex;
}

GLuint TextureLoader::m_loadCubemap(const std::vector<string>& fileNames)
{
    // Generate OpenGL texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    // Add the faces images to the texture buffer(textureID)
    for (GLuint i = 0; i < fileNames.size(); i++)
    {
        // Load SFML surface
        sf::Image surface;
        if (!surface.loadFromFile("./Assets/Textures/" + fileNames[i]))
        {
            Logger::throwError("Skybox textures could not be loaded!");
        }

        // Load SFML texturedata into OpenGL texture
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, surface.getSize().x, surface.getSize().y, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, surface.getPixelsPtr());

        // Logging
        Logger::throwInfo("Loaded cubemap texture: " + fileNames[i]);
    }

    // OpenGL magic
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Return new texture
    return textureId;
}