#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "Camera.h"
#include "Texture.h"

namespace FL
{
	struct RenderStats
	{
		uint32_t DrawCalls = 0;
		uint32_t VertexCount = 0;
		uint32_t IndexCount = 0;

		void ResetStats()
		{
			DrawCalls = 0;
			VertexCount = 0;
			IndexCount = 0;
		}
	};

	class Renderer
	{
	public:
		static void Init();
		static void ClearColor(glm::vec4 color);
		static void ClearBuffer();

		static void SubmitMesh(const std::vector<float>& vertices,const std::vector<uint32_t>& indices,	const Ref<Texture2D>& texture, const glm::mat4& transform = glm::mat4(1.0f));

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static const RenderStats& GetStats();
	private:
		static void Flush();
		static void StartBatch();
		static void NextBatch();
	};

}