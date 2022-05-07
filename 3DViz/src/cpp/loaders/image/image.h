#pragma once

#include <vector>

typedef unsigned char byte;

/**
 * The loaded image
 */
class Image {

public:

    /**
     * The image width
     */
    int width = 0;

    /**
     * The image height
     */
    int height = 0;

    /**
     * The image data in the RGBA format
     */
    std::vector<byte> bytes;

public:

    /**
     * Constructor
     */
    Image();

    /**
     * Constructor
     * @param width The image width
     * @param height The image height
     * @param bytes The image data in the RGBA format
     */
    Image(int width, int height, std::vector<byte> bytes);

};
