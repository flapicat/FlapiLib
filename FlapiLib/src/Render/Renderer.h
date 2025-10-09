#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace FL
{
	class Renderer
	{
	public:
		static void Init();
		static void ClearColor(glm::vec4 color);
		static void ClearBuffer();

		static void DrawFromVertexArray(const Ref<VertexArray>& VA);
	};

}