#pragma once

namespace FL
{
	class FrameBuffer
	{
	public:
		static Ref<FrameBuffer> Create(uint32_t width, uint32_t height);
		FrameBuffer(uint32_t width, uint32_t height);
		~FrameBuffer();

		void bind();
		void unbind();

		uint32_t getColorTexture() const { return m_ColorTexture; }
		uint32_t getDepthRBO() const { return m_depthRBO; }
		uint32_t GetID() const { return m_RendererID; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		void OnResize(uint32_t width, uint32_t height);
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorTexture = 0;
		uint32_t m_depthRBO = 0;
		uint32_t m_Width, m_Height;
	};
}