#pragma once

#include <string>

#include "image.h"

/**
 * Loads the image from the file<br>
 * See the stb_image library for supported formats<br>
 * The image is always loaded in the RGBA format
 * @param path the path to the file
 * @param flip whether the image is flipped vertically
 * @return the image from the file
 */
Image loadImage(const std::string& path, bool flip);
