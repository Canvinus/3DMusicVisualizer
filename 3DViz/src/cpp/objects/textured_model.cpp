#include "textured_model.h"

#include "../loaders/model/load_model.h"

bool TexturedModel::setUniforms = true;

GLuint TexturedModel::u_specular = 0;

GLuint TexturedModel::u_shininess = 0;

TexturedModel::TexturedModel(std::string modelPath) : modelPath(std::move(modelPath)) {}

void TexturedModel::initialize() {
    ModelData model = loadModel(modelPath);

    // The vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // The vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(GLfloat), model.vertices.data(), GL_STATIC_DRAW);

    // The a_position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) 0);

    // The a_normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));

    // The a_textureCoordinates attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));

    for (Image& image : model.images) {
        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bytes.data());

        textures.push_back(texture);
    }

    meshes = model.meshes;
}

void TexturedModel::draw() {
    glBindVertexArray(vao);

    int offset = 0;

    for (const MeshData& mesh : meshes) {
        if (setUniforms) {
            glUniform1f(u_specular, mesh.specular);
            glUniform1f(u_shininess, mesh.shininess);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[mesh.imageIndex]);

        glDrawArrays(GL_TRIANGLES, offset, mesh.verticesCount);

        offset += mesh.verticesCount;
    }
}
