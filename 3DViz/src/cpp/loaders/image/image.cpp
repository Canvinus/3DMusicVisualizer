#include "image.h"

Image::Image() = default;

Image::Image(int width, int height, std::vector<byte> bytes) : width(width), height(height), bytes(std::move(bytes)) { }
