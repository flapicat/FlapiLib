#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace FL
{
    struct Character {
        uint32_t TextureID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        uint32_t Advance;
    };

    class Font
    {
    public:
        static Ref<Font> Create(const FT_Library& FTlib, std::string_view path);
        Font(const FT_Library& FTlib, std::string_view path);
        ~Font();

        const std::map<char, Character>& GetCharacters() const { return m_Characters; }
        uint32_t GetSize() const { return m_PixelSize; }

        float GetAscent() const { return m_Ascent; }
        float GetDescent() const { return m_Descent; }
        float GetLineHeight() const { return m_LineHeight; }
        float GetTopOffset() const { return m_TopOffset; }

        glm::vec2 GetTextCenterOffset(const std::string& text, float scale);
    private:
        std::map<char, Character> m_Characters;
        uint32_t m_PixelSize = 128;

        float m_Ascent = 0.0f;
        float m_Descent = 0.0f;
        float m_LineHeight = 0.0f;
        float m_TopOffset = 0.0f;
    };
}
