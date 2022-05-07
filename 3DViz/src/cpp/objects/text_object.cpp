#include "text_object.h"

#include "../loaders/font/load_font.h"

TextObject::TextObject(std::string fontPath) : fontPath(std::move(fontPath)) {}

void TextObject::setText(std::string _text, float _x, float _y, float _scale) {
    text = std::move(_text);
    x = _x;
    y = _y;
    scale = _scale;
}

void TextObject::initialize() {
    loadFont(characters, fontPath, 48);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    // a_vertex
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
}


void TextObject::draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(vao);

    glActiveTexture(GL_TEXTURE0);

    for (unsigned char c : text) {
        Character ch = characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Update the vertex buffer
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindTexture(GL_TEXTURE_2D, ch.texture);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance to the next character
        // The advance unit is 1/64 pixels so it is possible to use bitshift instead of division (2^6 = 64)
        x += (ch.advance >> 6) * scale;
    }

    glDisable(GL_BLEND);
}
