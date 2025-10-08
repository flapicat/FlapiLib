#pragma once

namespace FL
{
	class VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

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

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

		uint32_t GetCount() { return m_Count; }

		void Bind();
		void UnBind();
	private:
		uint32_t m_RenderID;
		uint32_t m_Count;
	};


}