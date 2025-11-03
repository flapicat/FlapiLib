#include "pch.h"

#include "Font.h"
#include <glad/glad.h>

#include "Render/Texture.h"

namespace FL
{
	Font::Font(std::string_view path, uint32_t pixelSize)
		: m_PixelSize(pixelSize)
	{
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            LOG_ERROR("ERROR::FREETYPE: Could not init FreeType");
            return;
        }

        FT_Face face;
        if (FT_New_Face(ft, path.data(), 0, &face)) {
            LOG_ERROR("ERROR::FREETYPE: Failed to load font: ");
            FT_Done_FreeType(ft);
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, pixelSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

           uint32_t texture;
           glGenTextures(1, &texture);
           glBindTexture(GL_TEXTURE_2D, texture);
           glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
               face->glyph->bitmap.width,
               face->glyph->bitmap.rows,
               0, 
               GL_RED, 
               GL_UNSIGNED_BYTE,
               face->glyph->bitmap.buffer);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            Character ch = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (uint32_t)face->glyph->advance.x
            };
            m_Characters.insert(std::pair<char, Character>(c, ch));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
	}

	Font::~Font()
	{
	}
}