#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

/**
 * The camera
 */
class Camera {

public:

    /**
     * The camera position
     */
    glm::vec3 position = glm::vec3(0, 0, 0);

    /**
     * The forward direction vector
     */
    glm::vec3 forward;

    /**
     * The right direction vector
     */
    glm::vec3 right;

    /**
     * The up direction vector
     */
    glm::vec3 up;

private:

    float horizontalAngle = 0;

    float verticalAngle = 0;

public:

    /**
     * Constructor
     */
    Camera();

    /**
     * Constructor
     * @param x the camera x coordinate
     * @param y the camera y coordinate
     * @param z the camera z coordinate
     */
    Camera(float x, float y, float z);

    /**
     * Rotates the camera in the horizontal plane
     * @param angle the rotation angle
     */
    void rotateHorizontally(float angle);

    /**
     * Rotates the camera in the vertical plane
     * @param angle the rotation angle
     */
    void rotateVertically(float angle);

    /**
     * Updates the direction vectors
     */
    void updateDirection();

};
