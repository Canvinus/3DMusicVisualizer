#include "model_data.h"

ModelData::ModelData(std::vector<float> vertices, std::vector<Image> images, std::vector<MeshData> meshes) :
        vertices(std::move(vertices)),
        images(std::move(images)),
        meshes(std::move(meshes)) { }
