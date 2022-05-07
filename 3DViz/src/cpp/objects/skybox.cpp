#include "skybox.h"

#include "../loaders/image/load_image.h"

// The coordinates of the viewport corners
const GLfloat vertices[] {
    -1, 1,
    -1, -1,
    1, 1,
    1, -1
};

Skybox::Skybox(
        std::string rightPath,
        std::string leftPath,
        std::string upPath,
        std::string downPath,
        std::string frontPath,
        std::string backPath
        ) {
    imagePaths.push_back(std::move(rightPath));
    imagePaths.push_back(std::move(leftPath));
    imagePaths.push_back(std::move(upPath));
    imagePaths.push_back(std::move(downPath));
    imagePaths.push_back(std::move(frontPath));
    imagePaths.push_back(std::move(backPath));
}

void Skybox::initialize() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    Image images[6];

    for (int i = 0; i < 6; i++) images[i] = loadImage(imagePaths[i], false);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; i++) {
        Image& image = images[i];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bytes.data());
    }
}

void Skybox::draw() {
    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
