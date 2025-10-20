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
			Loaded = false;
			return;
		}
		else
		{
			Loaded = true;
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

	Texture2D::Texture2D(int width, int height, unsigned char* data)
		:m_Width(width), m_Height(height), m_Format(GL_RGBA)
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(
			GL_TEXTURE_2D,		// target
			0,					// level
			m_Format,           // internal format (GL_RGBA, GL_RGB, etc.)
			m_Width,			// width
			m_Height,			// height
			0,					// border
			m_Format,           // format
			GL_UNSIGNED_BYTE,	// type
			data				// pointer to pixels
		);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Texture);
	}

	void Texture2D::Bind(uint32_t slot)
	{
		if (m_Texture == 0) { LOG_WARN("Trying to bind invalid texture"); return; };
		m_BindSlot = slot;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}

	void Texture2D::UnBind()
	{
		glActiveTexture(GL_TEXTURE0 + m_BindSlot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& filePath)
	{
		return CreateRef<Texture2D>(filePath);
	}

	Ref<Texture2D> Texture2D::Create(int width, int height, unsigned char* data)
	{
		return CreateRef<Texture2D>(width, height, data);
	}
}