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

		m_Scene.OnInit(); 
		m_FBO = FrameBuffer::Create(window.GetWidth(), window.GetHeight());
		std::string scene = "Assets/Scenes/Example.flapi";
		m_Scene.LoadScene(scene);
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
		handler.Handle<FL::KeyPressedEvent>([&](const FL::KeyPressedEvent& ev) { OnKeyPressed(ev); });
	}

	void EditorLayer::OnKeyPressed(const KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return;

		bool control = Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Input::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || Input::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT);

		//ShortCuts
		switch (e.GetKeyCode())
		{
			case GLFW_KEY_N:
			{
				if (control)
				{
					AddScene(m_Scene);
				}
				break;
			}
			case GLFW_KEY_L:
			{
				if (control)
				{
					LoadScene(m_Scene);
				}
				break;
			}
			case GLFW_KEY_S:
			{
				if (control)
				{
					if (shift)
					{
						SaveAsScene(m_Scene);
					}
					else
					{
						SaveScene(m_Scene);
					}
				}
				break;
			}
		}
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

		DrawMenuBar(m_Scene);

		ImGui::End();

		DrawStats(m_fps);
		DrawViewport(m_FBO, m_Scene);
		DrawHierarhyPanel(m_Scene);
		DrawPropertiesPanel();

		ImGui::ShowDemoWindow();
	}
}