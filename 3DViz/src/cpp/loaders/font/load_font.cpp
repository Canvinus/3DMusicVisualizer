#include "load_font.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "../../utils/exception.h"

void loadFont(std::map<char, Character>& characters, std::string& path, int height) {
    FT_Library ft;

    if (FT_Init_FreeType(&ft)) throw Exception("loadFont: the FreeType Library wasn't initialized");

    FT_Face face;

    if (FT_New_Face(ft, path.c_str(), 0, &face)) throw Exception("loadFont: the font wasn't loaded");

    FT_Set_Pixel_Sizes(face, 0, height);

    // Disable the byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        // Load the character
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) throw Exception("loadFont: The character wasn't loaded");

        // Create the texture
        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character(
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (unsigned int) face->glyph->advance.x
        );
        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
