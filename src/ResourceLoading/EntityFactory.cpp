#include "EntityFactory.hpp"
#include <ResourceLoading/TextureLoader.hpp>
#include <ResourceLoading/ObjLoader.hpp>
#include <Misc/Logger.hpp>
#include <sstream>
#include <OpenGL/OpenGLBuffer.hpp>

std::shared_ptr<SkyEntity> EntityFactory::createSkyEntity(STRING id, STRING right, STRING left, STRING top,
                                                          STRING bottom, STRING back, STRING front)
{
    // Skybox vertices
    float skybox_data[] = {-1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                           1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                           -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                           -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                           1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                           -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                           -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                           1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                           -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                           1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

    // Create new entity
    // Mipmaps	= false because we are using pixel graphics. When set to true graphics become blurry.
    // Ansio	= false because it is not needed.
    auto pBuffer =
        std::make_shared<OpenGLBuffer>(skybox_data, sizeof(skybox_data) / sizeof(float), ShapeType::SHAPE_CUBE);
    auto entity =
        std::make_shared<SkyEntity>(id, pBuffer, TextureLoader::loadCubemap({right, left, top, bottom, back, front}));
    // Return entity
    return entity;
}

std::shared_ptr<TerrainEntity> EntityFactory::createTerrainEntity(STRING id, vec3 position, vec3 size,
                                                                  STRING textureFile, STRING blendR, STRING blendG,
                                                                  STRING blendB, vec2 uvRepeatR, vec2 uvRepeatG,
                                                                  vec2 uvRepeatB, STRING objFile, bool blendmapping)
{
    // Load textures
    auto texture = TextureLoader::loadTexture(textureFile, true, true);

    // Load .obj file
    auto          obj = ObjLoader::loadObjFile(objFile);
    vector<float> data;

    // Merge data into 1 array
    for (unsigned int i = 0; i < obj.vertices.size(); i++)
    {
        data.push_back(obj.vertices[i].x);
        data.push_back(obj.vertices[i].y);
        data.push_back(obj.vertices[i].z);

        data.push_back(obj.uvCoords[i].x);
        data.push_back(obj.uvCoords[i].y);

        data.push_back(obj.normals[i].x);
        data.push_back(obj.normals[i].y);
        data.push_back(obj.normals[i].z);
    }

    auto buffer = std::make_shared<OpenGLBuffer>(&data[0], (unsigned int)data.size(), ShapeType::SHAPE_3D);

    auto entity = std::make_shared<TerrainEntity>(id, buffer, texture);
    // Create new entity

    entity->setPosition(position);
    entity->setSize(size);

    if (blendmapping)
    {
        auto blendTexR = TextureLoader::loadTexture(blendR, true, true);
        auto blendTexG = TextureLoader::loadTexture(blendG, true, true);
        auto blendTexB = TextureLoader::loadTexture(blendB, true, true);
        entity->setBlendTextures(blendTexR, blendTexG, blendTexB);
        entity->setBlendUvRepeats(uvRepeatR, uvRepeatG, uvRepeatB);
    }

    // Return entity
    return entity;
}

std::shared_ptr<GameEntity> EntityFactory::createGameEntity(STRING id, vec3 position, vec3 rotation, vec3 size,
                                                            STRING texFile, STRING objFile)
{
    // Load .obj file
    auto          obj = ObjLoader::loadObjFile(objFile);
    vector<float> data;

    // Merge data into 1 array
    for (unsigned int i = 0; i < obj.vertices.size(); i++)
    {
        data.push_back(obj.vertices[i].x);
        data.push_back(obj.vertices[i].y);
        data.push_back(obj.vertices[i].z);

        data.push_back(obj.uvCoords[i].x);
        data.push_back(obj.uvCoords[i].y);

        data.push_back(obj.normals[i].x);
        data.push_back(obj.normals[i].y);
        data.push_back(obj.normals[i].z);
    }

    // Create new entity
    // Mipmaps	= false because we are using pixel graphics. When set to true graphics become blurry.
    // Ansio	= false because it is not needed.
    auto    buffer  = std::make_shared<OpenGLBuffer>(&data[0], (unsigned int)data.size(), ShapeType::SHAPE_3D);
    Texture texture = TextureLoader::loadTexture(texFile, false, false);
    auto    entity  = std::make_shared<GameEntity>(m_createUnitId(id), buffer, texture);
    entity->setPosition(position);
    entity->setRotation(rotation);
    entity->setSize(size);

    // Return entity
    return entity;
}

std::shared_ptr<GuiEntity> EntityFactory::createGuiEntity(STRING id, float x, float y, float w, float h, STRING texFile,
                                                          bool clickable, bool texfiltering)
{
    vector<float> data = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

                          1.0f, 1.0f, 1.0f, 1.0,  0.0f, 1.0f, 0.0f, 1.0,  0.0f, 0.0f, 0.0f, 0.0f};

    auto    buffer  = std::make_shared<OpenGLBuffer>(&data[0], (unsigned int)data.size(), ShapeType::SHAPE_2D);
    Texture texture = TextureLoader::loadTexture(texFile, texfiltering, texfiltering);
    auto    entity  = std::make_shared<GuiEntity>(id, buffer, texture);

    entity->setPosition(vec2(x, y));
    entity->setSize(vec2(w, h));
    entity->setClickable(clickable);

    // Return entity
    return entity;
}

std::string EntityFactory::m_createUnitId(std::string name)
{
    return name + std::to_string(m_unitIdCounter++);
}