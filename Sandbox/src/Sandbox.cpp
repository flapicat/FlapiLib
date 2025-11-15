#include "FlapiLib.h"
#include "Core/EntryPoint.h"
#include "pch.h"

#include <ImGui/imgui.h>

class EditorLayer : public FL::Layer
{
public:
	EditorLayer()
		:FL::Layer("Example")
	{
	}

	~EditorLayer()
	{
	}

	virtual void OnAttach() override
	{
		auto& window = FL::App::Get().GetWindow();

		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/checkerboard.png", "checkerboard");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/container.png", "container");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/tilemap.png", "tilemap");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/awesomeface.png", "awesomeface");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Model,	"Assets/objects/backpack/backpack.obj", "backpack");
		
		class CameraControllerScript : public FL::ScriptableEntity
		{
		public:
			CameraControllerScript() = default;
			void OnCreate() override
			{

			}

			void OnUpdate(FL::TimeStep ts) override
			{
				auto& cam = GetComponent<FL::CameraComponent>();
				auto& transform = GetComponent<FL::TransformComponent>();
				float velocity = cam.moveSpeed * ts;
				if (cam.type == FL::CameraTypes::Orthographic)
				{
					if (FL::Input::IsKeyPressed(GLFW_KEY_W))
						transform.Position += cam.up * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_S))
						transform.Position -= cam.up * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_A))
						transform.Position -= cam.right * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_D))
						transform.Position += cam.right * velocity;
				}

				if (cam.type == FL::CameraTypes::Perspective)
				{
					if (FL::Input::IsKeyPressed(GLFW_KEY_W))
						transform.Position += cam.front * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_S))
						transform.Position -= cam.front * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_E))
						transform.Position += cam.up * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_Q))
						transform.Position -= cam.up * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_A))
						transform.Position -= cam.right * velocity;
					if (FL::Input::IsKeyPressed(GLFW_KEY_D))
						transform.Position += cam.right * velocity;
				}
			}
		};

		m_Scene.OnInit();
		{
			FL::Entity cameraEntity = m_Scene.CreateEntity("MainCamera");
			cameraEntity.AddComponent<FL::TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f));
			cameraEntity.AddComponent<FL::CameraComponent>();
			cameraEntity.GetComponent<FL::CameraComponent>().primary = true;
			cameraEntity.GetComponent<FL::CameraComponent>().type = FL::CameraTypes::Orthographic;
			cameraEntity.AddComponent<FL::NativeScriptingComponent>();
			cameraEntity.GetComponent<FL::NativeScriptingComponent>().Bind<CameraControllerScript>();
		}

		m_FBO = FL::FrameBuffer::Create(window.GetWidth(), window.GetHeight());
	}
	
	virtual void OnDetach() override
	{
		m_Scene.OnDestroy();
	}

	virtual void OnUpdate(FL::TimeStep ts) override
	{
		m_fps = 1.0f / ts.GetSeconds();
		m_Scene.OnUpdate(ts);
	}

	virtual void OnRender() override
	{
		m_FBO->bind();
		FL::Renderer::ClearColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
		FL::Renderer::ClearBuffer();

		m_Scene.OnRender();
		m_FBO->unbind();
	}

	virtual void OnEvent(FL::Event& e)
	{
		m_Scene.OnEvent(e);
		FL::EventHandler handler(e);
		handler.Handle<FL::WindowResizeEvent>([&](const FL::WindowResizeEvent& ev) { m_FBO->OnResize(ev.GetWidth(), ev.GetHeight()); });
	}

	virtual void OnImGuiRender() override
	{
		ImGui::ShowDemoWindow();
		auto& stats = FL::Renderer2D::s_Statistic;

		ImGui::Begin("Viewport");
		ImGui::Image((ImTextureID)(uintptr_t)m_FBO->getColorTexture(), ImVec2(m_FBO->GetWidth(), m_FBO->GetHeight()), ImVec2(0, 1),	ImVec2(1, 0));
		ImGui::End();

		ImGui::Begin("Stats");
		ImGui::Text("FPS: %.1f", m_fps);
		ImGui::Text("Draw Calls: %u", stats.DrawCalls);
		ImGui::Text("Num Of Quads: %u", stats.NumOfQuads);
		ImGui::Text("Quad Vertices: %u", stats.GetQuadVertices());
		ImGui::Text("Quad Indices: %u", stats.GetQuadIndices());
		ImGui::End();

		FL::Renderer2D::s_Statistic.Reset();
	}

private:
	float m_fps = 0.0f; 
	FL::Scene m_Scene;
	Ref<FL::FrameBuffer> m_FBO;
};

class FlapiEditor : public FL::App
{
public:
	FlapiEditor()
	{
		PushLayer(new EditorLayer());
	}
	~FlapiEditor()
	{
	}
private:
};

FL::App* FL::CreateApp()
{
	return new FlapiEditor();
}
