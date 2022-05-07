#pragma once

#include <map>
#include <string>

#include "character.h"

/**
 * Loads the font from the file<br>
 * The characters are uploaded to the OpenGL textures
 * @param characters the map where the information about the loaded characters is stored
 * @param path the path to the file
 * @param height the height of the font
 */
void loadFont(std::map<char, Character>& characters, std::string& path, int height);
