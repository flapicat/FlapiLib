#include "pch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace FL
{
	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		glGenBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RenderID);
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return CreateRef<VertexBuffer>(vertices, size);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}

	void VertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		glGenBuffers(1, &m_RenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RenderID);
	
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return CreateRef<IndexBuffer>(indices, count);
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}

	void IndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}