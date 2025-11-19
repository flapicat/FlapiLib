#include "EditorLayer.h"

#include <imgui.h>

#include "UI/ImGuiMainUI.h"
#include "UI/HierarhyPanelUI.h"
#include "UI/PropertiesPanelUI.h"

namespace FL
{
	EditorLayer::EditorLayer()
		:Layer("Example")
	{
	}

	EditorLayer::~EditorLayer()
	{
	}

	void EditorLayer::OnAttach()
	{
		SetUpImGuiStyle();
		auto& window = App::Get().GetWindow();

		AssetManager::LoadAssetFromFile(AssetType::Texture, "Assets/Textures/checkerboard.png", "checkerboard");
		AssetManager::LoadAssetFromFile(AssetType::Texture, "Assets/Textures/container.png", "container");
		AssetManager::LoadAssetFromFile(AssetType::Texture, "Assets/Textures/tilemap.png", "tilemap");
		AssetManager::LoadAssetFromFile(AssetType::Texture, "Assets/Textures/awesomeface.png", "awesomeface");
		AssetManager::LoadAssetFromFile(AssetType::Model, "Assets/objects/backpack/backpack.obj", "backpack");

		class CameraControllerScript : public  ScriptableEntity
		{
		public:
			CameraControllerScript() = default;
			void OnCreate() override
			{

			}

			void OnUpdate(TimeStep ts) override
			{
				auto& cam = GetComponent<CameraComponent>();
				auto& transform = GetComponent<TransformComponent>();
				float velocity = cam.moveSpeed * ts;
				if (cam.type == CameraTypes::Orthographic)
				{
					if (Input::IsKeyPressed(GLFW_KEY_W))
						transform.Position += cam.up * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_S))
						transform.Position -= cam.up * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_A))
						transform.Position -= cam.right * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_D))
						transform.Position += cam.right * velocity;
				}

				if (cam.type == CameraTypes::Perspective)
				{
					if (Input::IsKeyPressed(GLFW_KEY_W))
						transform.Position += cam.front * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_S))
						transform.Position -= cam.front * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_E))
						transform.Position += cam.up * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_Q))
						transform.Position -= cam.up * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_A))
						transform.Position -= cam.right * velocity;
					if (Input::IsKeyPressed(GLFW_KEY_D))
						transform.Position += cam.right * velocity;
				}
			}
		};

		m_Scene.OnInit();
		{
			//Entity cameraEntity = m_Scene.CreateEntity("MainCamera");
			//cameraEntity.AddComponent< CameraComponent>();
			//cameraEntity.GetComponent< CameraComponent>().primary = true;
			//cameraEntity.GetComponent< CameraComponent>().type = CameraTypes::Orthographic;
			//cameraEntity.GetComponent< CameraComponent>().Zoom = 2.0f;
			//cameraEntity.AddComponent< NativeScriptingComponent>();
			//cameraEntity.GetComponent< NativeScriptingComponent>().Bind<CameraControllerScript>();
		}

		m_FBO = FrameBuffer::Create(window.GetWidth(), window.GetHeight());
	}

	void EditorLayer::OnDetach()
	{

		m_Scene.OnDestroy();
	}

	void EditorLayer::OnUpdate(TimeStep ts)
	{
		m_fps = 1.0f / ts.GetSeconds();
		m_Scene.OnUpdate(ts);
	}

	void EditorLayer::OnRender()
	{
		m_FBO->bind();
		Renderer::ClearColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
		Renderer::ClearBuffer();

		m_Scene.OnRender();
		m_FBO->unbind();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		if (s_UIData.s_IsViewPortFocus)
		{
			m_Scene.OnEvent(e);
		}
		EventHandler handler(e);
	}

	void EditorLayer::OnImGuiRender()
	{
		// DOCKSPACE
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

		static bool dockspaceOpen = true;
		static bool optFullscreen = true;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;

		if (optFullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Dockspace Window", &dockspaceOpen, windowFlags);
		ImGui::PopStyleVar(2);

		ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0), dockspaceFlags);

		DrawMenuBar();

		ImGui::End();

		DrawStats(m_fps);
		DrawViewport(m_FBO, m_Scene);
		DrawHierarhyPanel(m_Scene);
		DrawPropertiesPanel();

		ImGui::ShowDemoWindow();
	}
}