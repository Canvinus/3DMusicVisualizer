#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>

#include "object3d.h"
#include "../loaders/model/mesh_data.h"
#include "../loaders/font/character.h"

/*
 * The 2D text<br>
 * The text rendering and the font loading are mostly copied from here:
 * https://learnopengl.com/In-Practice/Text-Rendering
 */
class TextObject : public Object3D {

private:

    std::string fontPath;

    std::string text;

    float x = 0;

    float y = 0;

    float scale = 1.0;

    std::map<char, Character> characters;

    GLuint vbo = 0;

public:

    /**
     * Constructor
     * @param fontPath the path ro the font file
     */
    explicit TextObject(std::string fontPath);

    /**
     * Sets the text and its properties
     * @param _text the text
     * @param _x the text x coordinate in pixels from the lower left corner of the window
     * @param _y the text y coordinate in pixels from the lower left corner of the window
     * @param _scale the text scale
     */
    void setText(std::string _text, float _x, float _y, float _scale);

    void initialize() override;

    void draw() override;

};
