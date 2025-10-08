#include "FlapiLib.h"
#include "Core/EntryPoint.h"
#include "pch.h"

#include <glad/glad.h>

class ExampleLayer : public FL::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

	}
	~ExampleLayer()
	{

	}

	virtual void OnAttach() override
	{
		float vertices[9] = {
			0.0,0.5,0.0,
			-0.5,-0.5,0.0,
			0.5,-0.5,0.0
		};
		uint32_t indices[3] =
		{
			0,1,2
		};

		m_VertexArray->Create();

		Ref<FL::VertexBuffer>VB = FL::VertexBuffer::Create(vertices, sizeof(vertices));
		Ref<FL::IndexBuffer>IB = FL::IndexBuffer::Create(indices, 3);

		m_VertexArray->SetVB(VB);
		m_VertexArray->SetIB(IB);

		m_VertexArray->Unbind();
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate() override
	{
	}

	virtual void OnRender() override
	{
		glClearColor(0.1, 0.1, 0.1, 0.1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_VertexArray->Bind();
		glDrawElements(GL_TRIANGLES, m_VertexArray->GetIB()->GetCount(), GL_UNSIGNED_INT, 0);
		m_VertexArray->Unbind();
	}

	virtual void OnEvent(FL::Event& e) 
	{
	}

private:
	Ref<FL::VertexArray> m_VertexArray;
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