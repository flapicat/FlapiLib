#include "EditorLayer.h"

#include <imgui.h>

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
				auto& cam = GetComponent< CameraComponent>();
				auto& transform = GetComponent< TransformComponent>();
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
			Entity cameraEntity = m_Scene.CreateEntity("MainCamera");
			cameraEntity.AddComponent< TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f));
			cameraEntity.AddComponent< CameraComponent>();
			cameraEntity.GetComponent< CameraComponent>().primary = true;
			cameraEntity.GetComponent< CameraComponent>().type = CameraTypes::Orthographic;
			cameraEntity.GetComponent< CameraComponent>().Zoom = 2.0f;
			cameraEntity.AddComponent< NativeScriptingComponent>();
			cameraEntity.GetComponent< NativeScriptingComponent>().Bind<CameraControllerScript>();
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
		m_Scene.OnEvent(e);
		EventHandler handler(e);
	}

	void EditorLayer::OnImGuiRender()
	{
		// DOCKSPACE
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		static bool dockspaceOpen = true;
		static bool optFullscreen = true;
		static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;

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

		// -----------------------------------------------------------
		// Begin DockSpace window
		// -----------------------------------------------------------
		ImGui::Begin("Dockspace Window", &dockspaceOpen, windowFlags);
		ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0), dockspaceFlags);
		DrawMenuBar();
		ImGui::End();

		DrawViewport();
		DrawStats(); 
		DrawPropertiesPanel();
		DrawHierarhyPanel();

		ImGui::ShowDemoWindow();
	}

	void EditorLayer::DrawMenuBar()
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) { App::Get().Close(); }

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
		}

	void EditorLayer::DrawStats()
		{
			auto& stats = Renderer2D::s_Statistic;
			ImGui::Begin("Stats");
			ImGui::Text("FPS: %.1f", m_fps);
			ImGui::Text("Draw Calls: %u", stats.DrawCalls);
			ImGui::Text("Num Of Quads: %u", stats.NumOfQuads);
			ImGui::Text("Quad Vertices: %u", stats.GetQuadVertices());
			ImGui::Text("Quad Indices: %u", stats.GetQuadIndices());
			ImGui::End();
			Renderer2D::s_Statistic.Reset();
		}

	void EditorLayer::DrawViewport()
		{
			ImGui::Begin("Viewport");
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (viewportPanelSize.x != m_FBO->GetWidth() || viewportPanelSize.y != m_FBO->GetHeight())
			{
				m_FBO->OnResize(viewportPanelSize.x, viewportPanelSize.y);
			}
			m_Scene.UpdatePrimaryCameraAspect((float)m_FBO->GetWidth() / (float)m_FBO->GetHeight());
			ImGui::Image((ImTextureID)(uintptr_t)m_FBO->getColorTexture(), ImVec2(m_FBO->GetWidth(), m_FBO->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
		}

	void EditorLayer::DrawPropertiesPanel()
	{
		ImGui::Begin("Properties"); 
		//TagComponent
		if (m_HasSelectedEntity)
		{
			//TagComponent
			ImGui::Text("Tag");
			ImGui::Separator();
			auto& tag = m_SelectedEntity.GetComponent<TagComponent>();
			char buff[256];

			strncpy(buff, tag.Tag.c_str(), sizeof(buff));
			buff[sizeof(buff) - 1] = '\0';

			if (ImGui::InputText("Tag", buff, sizeof(buff)))
			{
				tag.Tag = buff;
			}

			ImGui::Separator();
			ImGui::NewLine();
			ImGui::Text("Transform");
			ImGui::Separator();
			auto& transform = m_SelectedEntity.GetComponent<TransformComponent>();
			static float f = 0;
			ImGui::DragFloat("asd", &f, 0.01f);
			ImGui::InputFloat3("Position", glm::value_ptr(transform.Position));
			ImGui::InputFloat3("Rotation", glm::value_ptr(transform.Rotation));
			ImGui::InputFloat3("Scale", glm::value_ptr(transform.Scale));
			ImGui::Separator();
		}
		ImGui::End();
	}

	void EditorLayer::DrawHierarhyPanel()
	{
		ImGui::Begin("Hierarhy");
		ImGui::Text("Curren Scene:");
		m_Scene.ForEachEntity([this](Entity entity) {
			auto& tag = entity.GetComponent<TagComponent>();
			bool isSelected = (m_HasSelectedEntity && m_SelectedEntity == entity);

			if (ImGui::Selectable(tag.Tag.c_str(), isSelected))
			{
				m_SelectedEntity = entity;
				m_HasSelectedEntity = true;
			}
			});
		ImGui::End();
	}
}