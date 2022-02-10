#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <ResourceLoading/Texture.hpp>

using std::string;

/// \brief
/// This class serves as a loader/manager of texture file data
/// \details
/// With this class you can load texture files (eg. PNG JPEG DDS) and receive the loaded data.
/// It makes use of caching, by storing the loaded data into an std::map
class TextureLoader final
{
  public:
    TextureLoader() = delete;

    /// Loads a texture file
    /// fileName: texture filename/path
    /// mipmap: option to enable OpenGL mipmapping technique, so that textures look better at far range
    /// aniso: option to enable OpenGL anisotropic filtering, so that textures look better from certain angles
    static Texture loadTexture(const string& fileName, bool mipmap, bool aniso);

    /// Loads multiple texture files into a cubemap texture
    /// fileNames: texture filenames/paths
    static Texture loadCubemap(const std::vector<string>& fileNames);

  private:
    /// Loads a texture file
    /// fileName: texture filename/path
    /// mipmap: option to enable OpenGL mipmapping technique, so that textures look better at far range
    /// aniso: option to enable OpenGL anisotropic filtering, so that textures look better from certain angles
    static Texture m_loadTexture(const string& fileName, bool mipmap, bool aniso);

    /// Loads multiple texture files into a cubemap texture
    /// fileNames: texture filenames/paths
    static Texture m_loadCubemap(const std::vector<string>& fileNames);

    static inline std::map<string, Texture> m_textureMap = {};
};