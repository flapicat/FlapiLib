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

		glGenVertexArrays(1, &VA);
		glBindVertexArray(VA);

		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
		glGenBuffers(1, &EB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
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

		glBindVertexArray(VA);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	virtual void OnEvent(FL::Event& e) 
	{
	}

private:
	uint32_t VA, VB, EB;
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