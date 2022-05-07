#pragma once

#include <vector>

#include "../image/image.h"
#include "mesh_data.h"

/**
 * The loaded model data
 */
class ModelData {

public:

    /**
     * The vertices of the model
     */
    std::vector<float> vertices;

    /**
     * The images of the model textures
     */
    std::vector<Image> images;

    /**
     * The meshes of the model
     */
    std::vector<MeshData> meshes;

public:

    /**
     * Constructor
     * @param vertices the vertices of the model
     * @param images the images of the model textures
     * @param meshes the meshes of the model
     */
    ModelData(std::vector<float> vertices, std::vector<Image> images, std::vector<MeshData> meshes);

};
