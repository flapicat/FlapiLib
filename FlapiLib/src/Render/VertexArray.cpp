#include "pch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace FL
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RenderID);
		glBindVertexArray(m_RenderID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RenderID);
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_RenderID);
	}
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::SetVB(const Ref<VertexBuffer>& VB)
	{
		m_VertexBuffer = VB;
		m_VertexBuffer->Bind();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void VertexArray::SetIB(const Ref<IndexBuffer>& IB)
	{
		m_IndexBuffer = IB;
		m_IndexBuffer->Bind();
	}

	Ref<VertexArray> VertexArray::Create()
	{
		return CreateRef<VertexArray>();
	}
}