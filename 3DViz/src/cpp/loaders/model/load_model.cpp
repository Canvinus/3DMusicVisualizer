#include "load_model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <unordered_map>

#include "../../utils/exception.h"
#include "../image/load_image.h"

Image createImageForColor(const aiColor3D& color) {
    std::vector<byte> bytes;

    bytes.push_back((byte) (color.r * 255));
    bytes.push_back((byte) (color.g * 255));
    bytes.push_back((byte) (color.b * 255));
    bytes.push_back(255);

    return Image(1, 1, bytes);
}

ModelData loadModel(const std::string& path) {
    std::cout << "Loading the model: " << path << std::endl;

    Assimp::Importer importer;

    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

    const aiScene* scene = importer.ReadFile(path,
                                             aiProcess_Triangulate
                                             | aiProcess_SortByPType
                                             | aiProcess_PreTransformVertices);

    if (!scene) {
        std::string message = "loadModel: the model loading completed with errors:\n";
        message += importer.GetErrorString();
        throw Exception(message);
    }

    std::string directory = path.substr(0, path.find_last_of('/'));

    std::vector<float> vertices;
    std::vector<Image> images;
    std::vector<MeshData> meshes;

    std::unordered_map<int, int> materialToImage;

    for (int k = 0; k < scene->mNumMeshes; k++) {
        aiMesh* mesh = scene->mMeshes[k];

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        if (materialToImage.find(mesh->mMaterialIndex) == materialToImage.end()) {
            float opacity = 1;
            material->Get(AI_MATKEY_OPACITY, opacity);

            // Skip transparent meshes
            if (opacity != 1) continue;

            aiString materialName;
            material->Get(AI_MATKEY_NAME, materialName);

            std::cout << "Loading the material: " << materialName.C_Str() << std::endl;

            aiColor3D color(1, 1, 1);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

            Image image = createImageForColor(color);

            aiString texturePath;

            if (material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), texturePath) == AI_SUCCESS) {
                std::string imagePath;

                if (texturePath.C_Str()[0] == '/') imagePath = directory + texturePath.C_Str();
                else imagePath = directory + "/" + texturePath.C_Str();

                std::cout << "Loading the texture: " << imagePath << std::endl;

                try {
                    image = loadImage(imagePath, true);
                }
                catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
            else {
                std::cout << "The material doesn't have a texture (the material color will be used)" << std::endl;
            }

            images.push_back(image);

            materialToImage[mesh->mMaterialIndex] = images.size() - 1;
        }

        aiColor3D specular(0, 0, 0);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

        float shininess = 0;
        material->Get(AI_MATKEY_SHININESS, shininess);

        if (shininess < 1) shininess = 1;

        meshes.emplace_back(mesh->mNumFaces * 3, materialToImage[mesh->mMaterialIndex], specular.r, shininess);

        for (int i = 0; i < mesh->mNumFaces; i++) {
            aiFace& face = mesh->mFaces[i];

            for (int j = 0; j < 3; j++) {
                unsigned int index = face.mIndices[j];

                vertices.push_back(mesh->mVertices[index].x);
                vertices.push_back(mesh->mVertices[index].y);
                vertices.push_back(mesh->mVertices[index].z);

                vertices.push_back(mesh->mNormals[index].x);
                vertices.push_back(mesh->mNormals[index].y);
                vertices.push_back(mesh->mNormals[index].z);

                if (mesh->HasTextureCoords(0)) {
                    vertices.push_back(mesh->mTextureCoords[0][index].x);
                    vertices.push_back(mesh->mTextureCoords[0][index].y);
                }
                else {
                    vertices.push_back(0);
                    vertices.push_back(0);
                }
            }
        }
    }

    return ModelData(vertices, images, meshes);
}
