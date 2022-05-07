#include "camera.h"

const float PI = glm::pi<float>();

Camera::Camera() = default;

Camera::Camera(float x, float y, float z) : position(x, y, z) {}

void Camera::rotateHorizontally(float angle) {
    horizontalAngle += angle;

    horizontalAngle = std::fmod(horizontalAngle, PI * 2);
}

void Camera::rotateVertically(float angle) {
    verticalAngle += angle;

    if (verticalAngle > PI / 2) verticalAngle = PI / 2;
    else if (verticalAngle < -PI / 2) verticalAngle = -PI / 2;
}

void Camera::updateDirection() {
    float projection = std::cos(verticalAngle);
    forward = glm::vec3(projection * std::sin(horizontalAngle), std::sin(verticalAngle), -projection * std::cos(horizontalAngle));

    right = glm::vec3(std::sin(horizontalAngle + PI / 2), 0, -std::cos(horizontalAngle + PI / 2));

    projection = std::cos(verticalAngle + PI / 2);
    up = glm::vec3(projection * std::sin(horizontalAngle), std::sin(verticalAngle + PI / 2), -projection * std::cos(horizontalAngle));
}