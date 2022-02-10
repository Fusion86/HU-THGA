#pragma warning(disable : 4996) // Disabling annoying warning

#include "ObjLoader.hpp"

#include <GL/glew.h>
#include <Misc/Logger.hpp>

const Obj& ObjLoader::loadObjFile(const string& fileName)
{
    // Check if mesh data was loaded already, if not, load data and store in std::map
begin:
    auto iterator = m_objMap.find(fileName); // Search for existing OBJs
    if (iterator == m_objMap.end())
    {
        m_objMap.insert(std::make_pair(fileName, m_loadObjFile(fileName))); // Insert new data
        goto begin;
    }
    else
    {
        return *iterator->second; // Return the corresponding OBJ parts
    }
}

std::shared_ptr<Obj> ObjLoader::m_loadObjFile(const string& fileName)
{
    // Declare variables
    auto         obj = std::make_shared<Obj>();
    vector<vec3> temmPositions;
    vector<vec2> temmUvs;
    vector<vec3> temmNormals;

    // Load .obj file
    FILE* file = fopen(("./Assets/Models/" + fileName).c_str(), "r");
    if (file == NULL)
    {
        Logger::throwError("Could not load OBJ file: " + fileName);
    }

    // Fill the vector with the data from the file
    while (true)
    {
        char lineHeader[128];
        int  res = fscanf(file, "%s", lineHeader);
        if (res == EOF) // End of file
        {
            break;
        }

        // File content
        if (strcmp(lineHeader, "v") == 0) // Vertices
        {
            vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temmPositions.push_back(vertex);
            continue;
        }
        else if (strcmp(lineHeader, "vt") == 0) // Uv coords
        {
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            temmUvs.push_back(uv);
            continue;
        }
        else if (strcmp(lineHeader, "vn") == 0) // Normals
        {
            vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temmNormals.push_back(normal);
            continue;
        }
        else if (strcmp(lineHeader, "f") == 0) // Faces (triangle data)
        {
            // Declare variables
            GLuint posIndex[3];
            GLuint uvIndex[3];
            GLuint normalIndex[3];
            int    matches;

            // Read face indices
            matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &posIndex[0], &uvIndex[0], &normalIndex[0],
                             &posIndex[1], &uvIndex[1], &normalIndex[1], &posIndex[2], &uvIndex[2], &normalIndex[2]);
            if (matches != 9)
            {
                Logger::throwError("Too many or not enough faces at file: " + fileName);
            }

            // Add vertices
            for (int i = 0; i < 3; i++)
            {
                obj->vertices.push_back(temmPositions[posIndex[i] - 1]);
                obj->uvCoords.push_back(temmUvs[uvIndex[i] - 1]);
                obj->normals.push_back(temmNormals[normalIndex[i] - 1]);
            }
        }
    }

    fclose(file);

    // Error checking
    if (obj->vertices.empty())
    {
        Logger::throwError("Incorrect or too little content at file: " + string(fileName + ".obj"));
    }

    // Logging
    Logger::throwInfo("Loaded OBJ model: " + fileName);

    // Return new OBJ
    return obj;
}