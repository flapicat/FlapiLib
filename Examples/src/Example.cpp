#include "FlapiLib.h"
#include "Core/EntryPoint.h"
#include "pch.h"

#include <glad/glad.h>

class ExampleLayer : public FL::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera((float)1600/(float)900)
	{

	}

	~ExampleLayer()
	{

	}

	virtual void OnAttach() override
	{
		std::vector<float> vertices =
		{
			// Positions        // Colors (RGBA)         //Texture UV
			-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f,1.0f,  // top-left
			 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f,1.0f,  // top-right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f,0.0f,  // bottom-left
			 0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f,0.0f   // bottom-right
		};

		std::vector<uint32_t> indices = 
		{
			0,1,2,2,3,1 
		};

		m_Shader = FL::Shader::Create("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");

		m_VertexArray = FL::VertexArray::Create();
		m_VertexArray->Bind();

		Ref<FL::VertexBuffer>VB = FL::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
		FL::BufferLayout layout = {
			{ FL::ShaderType::Float3, "a_Pos" },
			{ FL::ShaderType::Float4, "a_Color" },
			{ FL::ShaderType::Float2, "a_TextureCoords" }
		};
		VB->SetLayout(layout);
		m_VertexArray->SetVB(VB);

		Ref<FL::IndexBuffer>IB = FL::IndexBuffer::Create(indices.data(), indices.size());
		m_VertexArray->SetIB(IB);

		m_VertexArray->Unbind();

		m_Texture = FL::Texture2D::Create("Assets/Textures/container.png");
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate(FL::TimeStep ts) override
	{
	}

	virtual void OnRender() override
	{
		m_Shader->setMat4("u_viewProjectionMatrix", m_Camera.GetViewProjectionMatrix());

		FL::Renderer::ClearColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
		FL::Renderer::ClearBuffer();

		m_Texture->Bind();
		m_Shader->Use();
		m_Shader->setInt("u_texture", m_Texture->GetTextureBindSlot());
		FL::Renderer::DrawFromVertexArray(m_VertexArray);
	}

	virtual void OnEvent(FL::Event& e) 
	{
	}

private:
	Ref<FL::VertexArray> m_VertexArray;
	Ref<FL::Shader> m_Shader;
	FL::OrthoCamera m_Camera;
	Ref<FL::Texture2D>m_Texture;
};

class ExampleApp : public FL::App
{
public:
	ExampleApp()
	{
		PushLayer(new ExampleLayer());
	}
	~ExampleApp()
	{
	}
private:
};

FL::App* FL::CreateApp()
{
	return new ExampleApp();
}