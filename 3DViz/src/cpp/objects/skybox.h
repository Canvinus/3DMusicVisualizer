#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "object3d.h"

/**
 * The skybox
 */
class Skybox : public Object3D {

public:

    /**
     * The cubemap texture
     */
    GLuint texture = 0;

    /**
     * The paths to the skybox texture images
     */
    std::vector<std::string> imagePaths;

public:

    /**
     * Constructor
     * @param rightPath,
     * leftPath,
     * upPath,
     * downPath,
     * frontPath,
     * backPath the paths to the skybox texture images
     */
    Skybox(
            std::string rightPath,
            std::string leftPath,
            std::string upPath,
            std::string downPath,
            std::string frontPath,
            std::string backPath
    );

    void initialize() override;

    void draw() override;
};
