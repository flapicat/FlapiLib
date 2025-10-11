#include "pch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace FL
{
	static GLenum GetGLTypeFromShaderElementType(ShaderType type)
	{
		switch (type)
		{
		case FL::ShaderType::Bool:		return GL_BOOL;
		case FL::ShaderType::Int:		return GL_INT;
		case FL::ShaderType::Int2:		return GL_INT;
		case FL::ShaderType::Int3:		return GL_INT;
		case FL::ShaderType::Int4:		return GL_INT;
		case FL::ShaderType::Float:		return GL_FLOAT;
		case FL::ShaderType::Float2:	return GL_FLOAT;
		case FL::ShaderType::Float3:	return GL_FLOAT;
		case FL::ShaderType::Float4:	return GL_FLOAT;
		case FL::ShaderType::Mat2:		return GL_FLOAT;
		case FL::ShaderType::Mat3:		return GL_FLOAT;
		case FL::ShaderType::Mat4:		return GL_FLOAT;
		}
	}

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
		auto& layout = m_VertexBuffer->GetLayout();

		uint32_t index = 0;
		for (auto& element : layout.GetElements())
		{
			glVertexAttribPointer(index,
				GetLayoutElementCount(element.Type),
				GetGLTypeFromShaderElementType(element.Type),
				element.Normalized, 
				layout.GetStride(),
				(void*)element.Offset);
			glEnableVertexAttribArray(index);
			index++;
		}
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