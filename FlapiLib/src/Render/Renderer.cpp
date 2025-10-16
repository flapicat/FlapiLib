#include "pch.h"
#include "Renderer.h"

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

namespace FL
{
	static struct RenderData
	{
		static const uint32_t MaxVertices = 100000;
		static const uint32_t MaxIndices = 300000;
		static const uint32_t MaxTextureSlots = 8;

		Ref<VertexArray> VertexArray;
		Ref<Shader> Shader;

		uint32_t VertexCount = 0;
		uint32_t IndexCount = 0;

		std::vector<float> VertexBufferCPU;
		std::vector<uint32_t> IndexBufferCPU;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
	};

	static RenderData s_Data;
	static RenderStats s_Stats;

	void Renderer::ClearColor(glm::vec4 color)
	{
		glClearColor(0.1, 0.1, 0.1, 0.1);
	}

	void Renderer::ClearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::Init()
	{
		glEnable(GL_DEPTH_TEST);

		s_Data.VertexArray = VertexArray::Create();
		Ref<VertexBuffer> VB = VertexBuffer::Create(nullptr, s_Data.MaxVertices * sizeof(float) * 10);
		BufferLayout layout = {
			{ FL::ShaderType::Float3, "a_Pos" },
			{ FL::ShaderType::Float4, "a_Color" },
			{ FL::ShaderType::Float2, "a_TextureCoords" },
			{ FL::ShaderType::Float, "a_TexIndex" }
		};
		VB->SetLayout(layout);
		s_Data.VertexArray->SetVB(VB);

		Ref<IndexBuffer> IB = IndexBuffer::Create(nullptr, s_Data.MaxIndices);
		s_Data.VertexArray->SetIB(IB);

		s_Data.Shader = Shader::Create("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");
		int samplers[s_Data.MaxTextureSlots];
		for (int i = 0; i < s_Data.MaxTextureSlots; i++) samplers[i] = i;
		
		s_Data.Shader->Use();
		s_Data.Shader->setIntArray("u_textures", samplers, s_Data.MaxTextureSlots);

		s_Data.VertexBufferCPU.reserve(s_Data.MaxVertices * 10);
		s_Data.IndexBufferCPU.reserve(s_Data.MaxIndices);

		s_Data.TextureSlots[0] = FL::Texture2D::Create("Assets/Textures/white.png");
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		s_Stats.ResetStats();

		s_Data.Shader->Use();
		s_Data.Shader->setMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());
	
		StartBatch();
	}

	void Renderer::SubmitMesh(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, const Ref<Texture2D>& texture)
	{
		if (s_Data.VertexCount + vertices.size() / 10 >= s_Data.MaxVertices ||
			s_Data.IndexCount + indices.size() >= s_Data.MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
		}

		s_Data.VertexBufferCPU.insert(
			s_Data.VertexBufferCPU.end(),
			vertices.begin(),
			vertices.end()
		);

		uint32_t baseVertex = s_Data.VertexCount;
		for (auto idx : indices)
			s_Data.IndexBufferCPU.push_back(baseVertex + idx);

		s_Data.VertexCount += vertices.size() / 10;
		s_Data.IndexCount += indices.size();

		s_Stats.VertexCount += vertices.size() / 10; 
		s_Stats.IndexCount += indices.size();
	}

	void Renderer::EndScene()
	{
		Flush();
	}

	const RenderStats& Renderer::GetStats()
	{
		return s_Stats;
	}

	void Renderer::Flush()
	{
		if (s_Data.IndexCount == 0) return;

		s_Data.VertexArray->Bind();
		s_Data.VertexArray->GetVB()->SetBufferData(s_Data.VertexBufferCPU);
		s_Data.VertexArray->GetIB()->SetBufferData(s_Data.IndexBufferCPU);

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i])
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			else
			{
				LOG_WARN("TRYING TO BIND TEXTURE THAT DONT EXIST");
			}
		}

		glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);
		s_Stats.DrawCalls++;
	}

	void Renderer::StartBatch()
	{
		s_Data.VertexCount = 0;
		s_Data.IndexCount = 0;
		s_Data.VertexBufferCPU.clear();
		s_Data.IndexBufferCPU.clear();
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer::NextBatch()
	{
		Flush();
		StartBatch();
	}
}