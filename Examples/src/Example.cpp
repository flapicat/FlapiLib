#include "FlapiLib.h"
#include "Core/EntryPoint.h"
#include "pch.h"

#include <glad/glad.h>

class ExampleLayer : public FL::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera((float)1600/(float)900, FL::CameraType::Perspective)
	{
	}

	~ExampleLayer()
	{
	}

	virtual void OnAttach() override
	{
		m_Camera.SetPosition(glm::vec3(0.0,0.0,0.0));
		if (m_Camera.GetType() == FL::CameraType::Perspective){	cursorEnable = false;}

		m_vertices = {
			// Front face
			-0.5f, -0.5f,  0.5f, 1,1,1,1, 0.0f,0.0f,  1.0f, // bottom-left
			 0.5f, -0.5f,  0.5f, 1,1,1,1, 1.0f,0.0f,  1.0f, // bottom-right
			 0.5f,  0.5f,  0.5f, 1,1,1,1, 1.0f,1.0f,  1.0f, // top-right
			-0.5f,  0.5f,  0.5f, 1,1,1,1, 0.0f,1.0f,  1.0f, // top-left
													  
			// Back face							  
			-0.5f, -0.5f, -0.5f, 1,1,1,1, 1.0f,0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f, 1,1,1,1, 0.0f,0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f, 1,1,1,1, 0.0f,1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f, 1,1,1,1, 1.0f,1.0f,  1.0f,
													  
			// Left face							  
			-0.5f, -0.5f, -0.5f, 1,1,1,1, 0.0f,0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, 1,1,1,1, 1.0f,0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f, 1,1,1,1, 1.0f,1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f, 1,1,1,1, 0.0f,1.0f,  1.0f,
													  
			// Right face							  
			 0.5f, -0.5f, -0.5f, 1,1,1,1, 1.0f,0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f, 1,1,1,1, 0.0f,0.0f,  1.0f,
			 0.5f,  0.5f,  0.5f, 1,1,1,1, 0.0f,1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f, 1,1,1,1, 1.0f,1.0f,  1.0f,
													  
			 // Top face							  
			 -0.5f, 0.5f,  0.5f, 1,1,1,1, 0.0f,0.0f,  1.0f,
			  0.5f, 0.5f,  0.5f, 1,1,1,1, 1.0f,0.0f,  1.0f,
			  0.5f, 0.5f, -0.5f, 1,1,1,1, 1.0f,1.0f,  1.0f,
			 -0.5f, 0.5f, -0.5f, 1,1,1,1, 0.0f,1.0f,  1.0f,
													  
			 // Bottom face							  
			 -0.5f, -0.5f,  0.5f, 1,1,1,1, 0.0f,0.0f, 1.0f,
			  0.5f, -0.5f,  0.5f, 1,1,1,1, 1.0f,0.0f, 1.0f,
			  0.5f, -0.5f, -0.5f, 1,1,1,1, 1.0f,1.0f, 1.0f,
			 -0.5f, -0.5f, -0.5f, 1,1,1,1, 0.0f,1.0f, 1.0f
		};

		m_indices = {
			0, 1, 2,  2, 3, 0,       // front
			4, 5, 6,  6, 7, 4,       // back
			8, 9,10, 10,11, 8,       // left
		    12,13,14, 14,15,12,       // right
		    16,17,18, 18,19,16,       // top
		    20,21,22, 22,23,20        // bottom
		};

		// Load texture
		m_ContainerTexture = FL::Texture2D::Create("Assets/Textures/container.png");
	}

	virtual void OnDetach() override
	{
	}

	virtual void OnUpdate(FL::TimeStep ts) override
	{
		auto window = FL::App::Get().GetWindow().GetNativeWindow();
		m_Camera.OnUpdate(ts);
		if (FL::Input::OnKeyPressed(GLFW_KEY_ESCAPE))
		{
			if (cursorEnable)
			{
				cursorEnable = !cursorEnable;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else
			{
				cursorEnable = !cursorEnable;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
	}

	virtual void OnRender() override
	{
		FL::Renderer::ClearColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
		FL::Renderer::ClearBuffer();

		FL::Renderer::BeginScene(m_Camera);

		for (int x = 0; x < 10; x++)
		{
			for (int y = 0; y < 10; y++)
			{
				for (int z = 0; z < 10; z++)
				{
					FL::Renderer::SubmitMesh(m_vertices, m_indices, m_ContainerTexture);
				}
			}
		}

		FL::Renderer::EndScene();
	}

	virtual void OnEvent(FL::Event& e) 
	{
		FL::EventHandler handler(e);
		handler.Handle<FL::MouseScrollEvent>([this](const FL::MouseScrollEvent& ev) {m_Camera.OnMouseScrolled(ev); });
		if(!cursorEnable)handler.Handle<FL::MouseMovedEvent>([this](const FL::MouseMovedEvent& ev) {m_Camera.OnMouseMoved(ev); });
	}

private:
	bool cursorEnable = false;
	FL::CameraController m_Camera;
	std::vector<float> m_vertices;
	std::vector<uint32_t> m_indices;
	Ref<FL::Texture2D> m_ContainerTexture;
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

