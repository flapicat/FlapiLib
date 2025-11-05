#pragma once
#include "Camera.h"
#include "Texture.h"

#include "Render/VertexArray.h"
#include "Core/Font.h"

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

		static void DrawQuadInternal(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture);
		static void DrawQuadInternalTransform(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2D> texture);
		static void DrawRotatedQuadInternal(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture);
		
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D> texture);
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture);

		static void DrawTextWIndow(Ref<Font> font, const std::string& text, const glm::vec3& position, float scale, const glm::vec3& color, bool center = false);

		static void DrawPointInternal(const glm::vec3& Position, const glm::vec4& color, float pointSize);
		static void DrawPoint(const glm::vec3& Position, const glm::vec4& color, float pointSize = 1.0f);
		static void DrawPoint(const glm::vec2& Position, const glm::vec4& color, float pointSize = 1.0f);

		static void DrawLineInternal(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float thickness);
		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color, float thickness = 1.0f);
		static void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color, float thickness = 1.0f);
	};

}