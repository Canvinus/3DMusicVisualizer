#version 400 core

uniform samplerCube u_texture; // The skybox texture

in vec3 v_textureCoordinates; // The texture coordinates

out vec4 color; // The fragment color

void main() {
    color = texture(u_texture, v_textureCoordinates);
}
