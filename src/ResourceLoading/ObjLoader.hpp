#pragma once

#include <map>
#include <iostream>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using std::map;
using std::string;
using std::vector;

/// \brief
/// This struct serves as a collection of all loaded .obj data
struct Obj
{
    vector<vec3> vertices;
    vector<vec2> uvCoords;
    vector<vec3> normals;
};

/// \brief
/// This class serves as a loader/manager of .obj file data
/// \details
/// With this class you can load obj files and receive the loaded data.
/// It makes use of caching, by storing the loaded data into an std::map
class ObjLoader final
{
  private:
  public:
    ObjLoader() = delete;

    ~ObjLoader() = delete;

    /// Loads a .obj file
    /// fileName: .obj filename/path
    static const Obj& loadObjFile(const string& fileName);

  private:
    /// Loads a .obj file
    /// fileName: .obj filename/path
    static std::shared_ptr<Obj> m_loadObjFile(const string& fileName);

    static inline map<string, std::shared_ptr<Obj>> m_objMap = {};
};
