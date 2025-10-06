#pragma once

namespace FL
{
	class VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		void Bind();
		void UnBind();
	private:
		uint32_t m_RenderID;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		void Bind();
		void UnBind();
	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
	};


}