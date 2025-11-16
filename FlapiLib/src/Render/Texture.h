#pragma once
#include <string>
#include "Core/Core.h"

#include <glad/glad.h>

namespace FL
{
	class Texture2D
	{
	public:
		static Ref<Texture2D> Create(const std::string& filePath);
		static Ref<Texture2D> Create(int width, int height, unsigned char* data);

		Texture2D(const std::string& filePath);
		Texture2D(int width, int height, unsigned char* data);
		~Texture2D();

		void Bind(uint32_t slot = 0);
		void UnBind();
		uint32_t GetTextureBindSlot() const { return m_BindSlot; }

		uint32_t GetRendererID() { return m_Texture; }


		bool operator==(Texture2D& texture)
		{
			return m_Texture == texture.m_Texture;
		}
	public:
		bool Loaded = false;
	private:
		uint32_t m_Texture;
		uint32_t m_BindSlot = 0;
		int32_t m_Width, m_Height, m_Format;
	};

}