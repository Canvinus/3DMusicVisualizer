#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "object3d.h"
#include "../loaders/model/mesh_data.h"

/**
 * The textured model that is loaded from the file<br>
 * Models with colored materials are supported too
 * because colors are converted to textures<br>
 * See the libraries/CMakeLists.txt file for supported formats<br>
 * The importers with the commented lines are enabled
 */
class TexturedModel : public Object3D {

public:

    static bool setUniforms;

    /**
     * The specular uniform location to set data for each mesh
     */
    static GLuint u_specular;

    /**
     * The shininess uniform location to set data for each mesh
     */
    static GLuint u_shininess;

public:

    /**
     * The path to the model
     */
    std::string modelPath;

    /**
     * The model textures
     */
    std::vector<GLuint> textures;

    /**
     * The model meshes
     */
    std::vector<MeshData> meshes;

public:

    /**
     * Constructor
     * @param modelPath the path to the model
     */
    explicit TexturedModel(std::string modelPath);

    void initialize() override;

    void draw() override;

};
