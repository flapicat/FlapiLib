#pragma once
#include "Camera.h"
#include "Texture.h"

#include "Render/VertexArray.h"

namespace FL
{
	class Renderer2D
	{
	public:
		struct Statistic
		{
			uint32_t DrawCalls = 0;
			uint32_t NumOfQuads = 0;

			void Reset() { DrawCalls = 0; NumOfQuads = 0; }

			uint32_t GetQuadVertices() const { return NumOfQuads * 4; }
			uint32_t GetQuadIndices() const { return NumOfQuads * 6; }
		};
		inline static Statistic s_Statistic;

	public:
		static void Init();
		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void DrawIndexed(Ref<VertexArray> VA, uint32_t count);

		static void BeginBatch();
		static void EndBatch();
		static void Flush();

		static void DrawQuadInternal(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
		static void DrawQuadInternalTransform(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);
		static void DrawRotatedQuadInternal(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor = 1);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor = 1);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor = 1);

	};

}