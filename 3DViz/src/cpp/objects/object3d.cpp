#include "object3d.h"

#include <glm/ext.hpp>

Object3D::Object3D() = default;

Object3D::Object3D(glm::vec3 position) : position(position) { }

Object3D::Object3D(glm::vec3 position, glm::vec3 rotation) : position(position), rotation(rotation) { }

Object3D::Object3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : position(position), rotation(rotation), scale(scale) { }

void Object3D::update(float dt) { }

glm::mat4 Object3D::getModelMatrix() const {
    glm::mat4 matrix = glm::identity<glm::mat4>();
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, rotation.x, glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, rotation.y, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, rotation.z, glm::vec3(0, 0, 1));
    matrix = glm::scale(matrix, scale);

    return matrix;
}

glm::mat4 Object3D::getNormalMatrix() const {
    return glm::transpose(glm::inverse(getModelMatrix()));
}
