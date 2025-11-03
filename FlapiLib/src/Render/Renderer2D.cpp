#include "pch.h"
#include "Renderer2D.h"

#include "Render/Shader.h"
#include "Render/VertexArray.h"
#include "Render/Buffer.h"

#include <glm/glm.hpp>          
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <glad/glad.h>

namespace FL
{

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TextureCoords;
		float TextureID;
		float TailingFactor;
	};

	struct Renderer2Ddata
	{
		//Const
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		const static uint32_t MaxTextureSlots = 32;

		//Shaders
		Ref<Shader> TextureShader;

		//QuadData

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;

		QuadVertex* QuadVertexBufferBase;
		QuadVertex* QuadVertexBufferPtr;

		uint32_t QuadIndexCount = 0;

		//Textures
		Ref<Texture2D> WhiteTexture;
		uint32_t WhiteTextureID = 0;
		Ref<Texture2D> TextureSlots[MaxTextureSlots];
		uint32_t TextureSlotIndex = 1; // 0 = white texture
	};

	static Renderer2Ddata s_Data;

	void Renderer2D::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		s_Data.TextureShader = Shader::Create("Assets/Shaders/shader2D.vert", "Assets/Shaders/shader2D.frag");

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		s_Data.QuadVertexArray = VertexArray::Create();
		s_Data.QuadVertexArray->Bind();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(nullptr, s_Data.MaxVertices * sizeof(QuadVertex));
		BufferLayout layout = {
			{ ShaderType::Float3, "a_Position" },
			{ ShaderType::Float4, "a_Color"},
			{ ShaderType::Float2, "a_TextureCoords"},
			{ ShaderType::Float , "a_TextureID"},
			{ ShaderType::Float , "a_TailingFactor"}
		};
		s_Data.QuadVertexBuffer->SetLayout(layout);
		s_Data.QuadVertexArray->SetVB(s_Data.QuadVertexBuffer);

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIB(quadIB);
		delete[] quadIndices;

		unsigned char WhiteTextureData[4] = { 255,255,255,255 };
		s_Data.WhiteTexture = Texture2D::Create(1, 1, WhiteTextureData);
		s_Data.WhiteTextureID = 0;
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexArray->Unbind();

		int samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader->Use();
		s_Data.TextureShader->setIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.TextureShader->Use();
		s_Data.TextureShader->setMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		BeginBatch();
	}

	void Renderer2D::EndScene()
	{
		EndBatch();
	}

	void Renderer2D::BeginBatch()
	{
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.QuadIndexCount = 0;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::EndBatch()
	{
		uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetBufferData(s_Data.QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuadInternal(position, size, color, s_Data.WhiteTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor)
	{
		DrawQuadInternal(position, size, glm::vec4(1.0f), texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
	{
		DrawQuadInternal(position, size, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		DrawQuadInternalTransform(transform, color, s_Data.WhiteTexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		DrawQuadInternalTransform(transform, color, s_Data.WhiteTexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotatedQuadInternal(position, rotation, size, color, s_Data.WhiteTexture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, Ref<Texture2D> texture, float tilingFactor)
	{
		DrawRotatedQuadInternal(position, rotation, size, glm::vec4(1.0f), texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
	{
		DrawRotatedQuadInternal(position, rotation, size, color, texture, tilingFactor);
	}

	void Renderer2D::DrawQuadInternal(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
		{
			EndBatch();
			BeginBatch();
		}

		float textureIndex = 0.0f;
		if (texture != nullptr && texture != s_Data.WhiteTexture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
			{
				if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) // new texture
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
		glm::vec3 positions[4] = {
			{ -0.5f, -0.5f, 0.0f }, // bottom left
			{  0.5f, -0.5f, 0.0f }, // bottom right
			{  0.5f,  0.5f, 0.0f }, // top right
			{ -0.5f,  0.5f, 0.0f }  // top left
		};

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
			s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Statistic.NumOfQuads++;
	}

	void Renderer2D::DrawQuadInternalTransform(const glm::mat4& transform, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
		{
			EndBatch();
			BeginBatch();
		}

		float textureIndex = 0.0f;
		if (texture != nullptr && texture != s_Data.WhiteTexture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
			{
				if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) // new texture
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
		glm::vec3 positions[4] = {
			{ -0.5f, -0.5f, 0.0f }, // bottom left
			{  0.5f, -0.5f, 0.0f }, // bottom right
			{  0.5f,  0.5f, 0.0f }, // top right
			{ -0.5f,  0.5f, 0.0f }  // top left
		};

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
			s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Statistic.NumOfQuads++;
	}

	void Renderer2D::DrawRotatedQuadInternal(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color, Ref<Texture2D> texture, float tilingFactor)
	{
		if (s_Data.QuadIndexCount >= s_Data.MaxIndices || s_Data.TextureSlotIndex >= s_Data.MaxTextureSlots)
		{
			EndBatch();
			BeginBatch();
		}

		float textureIndex = 0.0f;
		if (texture != nullptr && texture != s_Data.WhiteTexture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) // start at 1, skip white
			{
				if (s_Data.TextureSlots[i] && s_Data.TextureSlots[i]->GetRendererID() == texture->GetRendererID())
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) // new texture
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

		glm::vec2 texCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
		glm::vec3 positions[4] = {
			{ -0.5f, -0.5f, 0.0f }, // bottom left
			{  0.5f, -0.5f, 0.0f }, // bottom right
			{  0.5f,  0.5f, 0.0f }, // top right
			{ -0.5f,  0.5f, 0.0f }  // top left
		};

		for (int i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * glm::vec4(positions[i], 1.0f);
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TextureCoords = texCoords[i];
			s_Data.QuadVertexBufferPtr->TextureID = textureIndex;
			s_Data.QuadVertexBufferPtr->TailingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Statistic.NumOfQuads++;
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Statistic.DrawCalls++;
	}
	
	void Renderer2D::DrawIndexed(Ref<VertexArray> VA, uint32_t count)
	{
		VA->Bind();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
		VA->Unbind();
	}

}