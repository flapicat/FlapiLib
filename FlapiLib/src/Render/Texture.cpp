#include "pch.h"
#include "Texture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace FL
{
	Texture2D::Texture2D(const std::string& filePath)
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_Format, 0);
		if (!data)
		{
			LOG_WARN("Failed to load texture from filepath: {0}", filePath);
		}

		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLenum format = (m_Format == 4) ? GL_RGBA : GL_RGB;
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void Texture2D::Bind(uint32_t slot)
	{
		m_BindSlot = slot;
		glBindTextureUnit(slot, m_Texture);
	}

	void Texture2D::UnBind()
	{
		glBindTextureUnit(m_BindSlot, 0);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filePath)
	{
		return CreateRef<Texture2D>(filePath);
	}
}