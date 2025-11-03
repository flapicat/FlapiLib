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
        Font(std::string_view path, uint32_t pixelSize);
        ~Font();

        const std::map<char, Character>& GetCharacters() const { return m_Characters; }
        uint32_t GetSize() const { return m_PixelSize; }
    private:
        std::map<char, Character> m_Characters;
        uint32_t m_PixelSize;
	};

}