#include "load_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../../utils/exception.h"

Image loadImage(const std::string& path, bool flip) {
    int channels = 4;

    int width;
    int height;

    stbi_set_flip_vertically_on_load(flip);

    byte* data = stbi_load(path.data(), &width, &height, nullptr, channels);

    if (!data) {
        std::string message = "loadImage: the image loading completed with errors:\n";
        message += stbi_failure_reason();
        throw Exception(message);
    }

    std::vector<byte> bytes(data, data + width * height * channels);

    stbi_image_free(data);

    Image image(width, height, bytes);

    return image;
}
