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
		std::vector<float> vertices = 
		{
			-0.5,0.5,0.0, //lu
			0.5,0.5,0.0, //ru
			-0.5,-0.5,0.0,//ld
			0.5,-0.5,0.0//rd
		};

		std::vector<uint32_t> indices = 
		{
			0,1,2,2,3,1 
		};

		m_VertexArray = FL::VertexArray::Create();
		m_VertexArray->Bind();

		Ref<FL::VertexBuffer>VB = FL::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(float));
		Ref<FL::IndexBuffer>IB = FL::IndexBuffer::Create(indices.data(), indices.size());

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