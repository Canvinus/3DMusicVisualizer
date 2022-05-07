#include "load_text.h"

#include <fstream>

#include "../../utils/exception.h"

std::string loadText(const std::string& path) {
    std::ifstream stream(path);

    if (!stream.is_open()) throw Exception("loadText: the file \"" + path + "\" doesn't exist");

    std::string text;

    const int size = 1024;
    char buffer[size];

    while (stream) {
        stream.read(buffer, size);

        text.append(buffer, stream.gcount());
    }

    return text;
}
