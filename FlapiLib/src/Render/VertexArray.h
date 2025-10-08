#pragma once
#include "Buffer.h"

namespace FL
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

		void SetVB(const Ref<VertexBuffer>& VB);
		void SetIB(const Ref<IndexBuffer>& IB);
		const Ref<VertexBuffer>& GetVB() { return m_VertexBuffer; };
		const Ref<IndexBuffer>& GetIB() { return m_IndexBuffer; };

		static Ref<VertexArray> Create();
	private:
		uint32_t m_RenderID;
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}