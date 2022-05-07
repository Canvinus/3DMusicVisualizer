#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

/**
 * The base class for 3D objects
 */
class Object3D {

public:

    /**
     * The vertex array object
     */
    GLuint vao = 0;

    /**
     * The position of the 3D object
     */
    glm::vec3 position = glm::vec3(0, 0, 0);

    /**
     * The rotation of the 3D object
     */
    glm::vec3 rotation = glm::vec3(0, 0, 0);

    /**
     * The scale of the 3D object
     */
    glm::vec3 scale = glm::vec3(1, 1, 1);

public:

    /**
     * Constructor
     */
    Object3D();

    /**
     * Constructor
     * @param position the position of the 3D object
     */
    explicit Object3D(glm::vec3 position);

    /**
     * Constructor
     * @param position the position of the 3D object
     * @param rotation the rotation of the 3D object
     */
    Object3D(glm::vec3 position, glm::vec3 rotation);

    /**
     * Constructor
     * @param position the position of the 3D object
     * @param rotation the rotation of the 3D object
     * @param scale the scale of the 3D object
     */
    Object3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

    /**
     * Initializes the 3D object in OpenGL
     */
    virtual void initialize() = 0;

    /**
     * Updates the 3D object
     * @param dt the elapsed time since the last frame in seconds
     */
    virtual void update(float dt);

    /**
     * Draws the 3D object
     */
    virtual void draw() = 0;

    /**
     * Returns the model matrix for the 3D object
     * @return the model matrix for the 3D object
     */
    virtual glm::mat4 getModelMatrix() const;

    /**
     * Returns the normal matrix for the 3D object<br>
     * You can read about the normal matrix here:
     * https://webglfundamentals.org/webgl/lessons/webgl-3d-lighting-directional.html
     * @return the normal matrix for the 3D object
     */
    virtual glm::mat4 getNormalMatrix() const;

};
