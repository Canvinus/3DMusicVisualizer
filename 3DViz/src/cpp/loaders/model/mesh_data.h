#pragma once

#include <vector>

/**
 * The loaded mesh data
 */
class MeshData {

public:

    /**
     * The vertices count of the mesh
     */
    int verticesCount;

    /**
     * The image index in the images array
     */
    int imageIndex;

    /**
     * The specular parameter (how much the specular lighting affects the object)
     */
    float specular;

    /**
     * The shininess parameter (how reflective the object is, opposite to roughness)
     */
    float shininess;

public:

    /**
     * Constructor
     * @param verticesCount the vertices count in the mesh
     * @param imageIndex the image index in the images array
     * @param specular the specular parameter
     * @param shininess the shininess parameter
     */
    MeshData(int verticesCount, int imageIndex, float specular, float shininess);

};
