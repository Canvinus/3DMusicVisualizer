#version 400 core

layout (location = 0) in vec2 a_position; // The coordinates of the viewport corner

uniform vec3 u_forward; // The forward direction of the camera
uniform vec3 u_clipPlaneRight; // The vector from the center to the right edge of the near clip plane
uniform vec3 u_clipPlaneUp; // The vector from the center to the upper edge of the near clip plane

out vec3 v_textureCoordinates; // The texture coordinates

void main() {
    v_textureCoordinates = u_forward + a_position.x * u_clipPlaneRight + a_position.y * u_clipPlaneUp; // Calculate the ray for the viewport corner
    v_textureCoordinates.z *= -1; // Inverse the z coordinate because the cubemap texture uses the left-handed coordinate system
    gl_Position = vec4(a_position, 0.0, 1.0);  // Set the viewport corner position in the clip space
}
