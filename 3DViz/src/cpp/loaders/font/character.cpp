#include "character.h"


Character::Character() { }

Character::Character(GLuint texture, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance) :
        texture(texture),
        size(size),
        bearing(bearing),
        advance(advance) { }
