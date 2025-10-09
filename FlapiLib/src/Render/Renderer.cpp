#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>

namespace FL
{

	void Renderer::Init()
	{

	}

	void Renderer::ClearColor(glm::vec4 color)
	{
		glClearColor(0.1,0.1,0.1,0.1);
	}

	void Renderer::ClearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::DrawFromVertexArray(const Ref<VertexArray>& VA)
	{
		VA->Bind();
		glDrawElements(GL_TRIANGLES, VA->GetIB()->GetCount(), GL_UNSIGNED_INT, 0);
		VA->Unbind();
	}
}