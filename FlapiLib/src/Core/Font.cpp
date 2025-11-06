#include "pch.h"
#include "Font.h"
#include <glad/glad.h>
#include "Render/Texture.h"

namespace FL
{
    Ref<Font> Font::Create(const FT_Library& FTlib, std::string_view path)
    {
        return CreateRef<Font>(FTlib, path);
    }

    Font::Font(const FT_Library& FTlib, std::string_view path)
    {
        FT_Face face;
        if (FT_New_Face(FTlib, path.data(), 0, &face)) {
            LOG_ERROR("ERROR::FREETYPE: Failed to load font: {0}", path);
            FT_Done_FreeType(FTlib);
            return;
        }

        FT_Set_Pixel_Sizes(face, 0, m_PixelSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        m_Ascent = (float)face->size->metrics.ascender / 64.0f;
        m_Descent = (float)face->size->metrics.descender / 64.0f;
        m_LineHeight = (float)face->size->metrics.height / 64.0f;

        float maxBearingY = 0.0f;

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

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character ch = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (uint32_t)face->glyph->advance.x
            };
            m_Characters.insert({ c, ch });

            if (ch.Bearing.y > maxBearingY)
                maxBearingY = (float)ch.Bearing.y;
        }

        m_TopOffset = m_Ascent - maxBearingY;

        FT_Done_Face(face);
    }

    Font::~Font()
    {
    }

    glm::vec2 Font::GetTextCenterOffset(const std::string& text, const glm::vec3& scale)
    {
        float width = 0.0f;
        float maxHeight = 0.0f;

        for (auto c : text)
        {
            auto it = m_Characters.find(c);
            if (it == m_Characters.end()) continue;

            const Character& ch = it->second;
            width += (ch.Advance >> 6) * scale.x;
            float height = ch.Size.y * scale.y;
            if (height > maxHeight)
                maxHeight = height;
        }

        return { width * 0.5, maxHeight * 0.5};
    }

}
