#version 400 core

uniform sampler2D u_texture; // The character texture
uniform vec3 u_textColor; // The text color

in vec2 v_textureCoordinates; // The texture coordinates

out vec4 color; // The fragment color

void main() {
    // Set the fragment color based on the text color and the character opacity
    color = vec4(u_textColor, texture(u_texture, v_textureCoordinates).r);
}