#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"

/**
 * The information about the loaded character
 */
class Character {

public:

    /**
     * The character texture
     */
    GLuint texture = 0;

    /**
     * The size of the character
     */
    glm::ivec2 size = glm::ivec2(0, 0);

    /**
     * The offset from the baseline to the left/top of the character
     */
    glm::ivec2 bearing = glm::ivec2(0, 0);

    /**
     *  The offset to advance to the next character
     */
    unsigned int advance = 0;

public:

    /**
     * Constructor
     */
    Character();

    /**
     * Constructor
     * @param texture the character texture
     * @param size the size of the character
     * @param bearing the offset from the baseline to the left/top of the character
     * @param advance the offset to advance to the next character
     */
    Character(GLuint texture, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance);

};
