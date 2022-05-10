#version 400 core

in vec3 v_color; // The color that is passed from the vertex shader and interpolated

out vec4 color;  // The fragment color

void main() {
    color = vec4(v_color, 1.0);  // Set the fragment color
}
