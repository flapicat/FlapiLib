#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Camera.h"
#include "Texture.h"

namespace FL
{
	class Renderer
	{
	public:
		static void Init();
		static void ClearColor(glm::vec4 color);
		static void ClearBuffer();

		static void SubmitMesh(const std::vector<float>& vertices,const std::vector<uint32_t>& indices,	const Ref<Texture2D>& texture);

		static void BeginScene(const Camera& camera);
		static void EndScene();
	private:
		static void Flush();
		static void StartBatch();
		static void NextBatch();
	};

}