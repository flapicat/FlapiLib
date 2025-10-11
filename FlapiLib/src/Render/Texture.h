#pragma once

namespace FL
{
	class Texture2D
	{
	public:
		Texture2D(const std::string& filePath);
		~Texture2D();

		void Bind(uint32_t slot = 0);
		void UnBind();
		uint32_t GetTextureBindSlot() const { return m_BindSlot; }

		static Ref<Texture2D> Create(const std::string& filePath);
	private:
		uint32_t m_Texture;
		uint32_t m_BindSlot = 0;
		int32_t m_Width, m_Height, m_Format;
	};

}