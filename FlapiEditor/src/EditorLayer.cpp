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
		if (m_IsViewPortFocus)
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

		DrawViewport();
		DrawStats();
		DrawPropertiesPanel();
		DrawHierarhyPanel();

		ImGui::ShowDemoWindow();
	}

	void EditorLayer::SetUpImGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// Base colors (dark subtle)
		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 0.95f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.09f, 0.95f);
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 0.6f);

		// Headers / categories
		colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.27f, 1.00f);

		// Title bars
		colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);

		// Buttons and frames
		colors[ImGuiCol_Button] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);

		// Scrollbar
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);

		// Selection
		colors[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);

		// General rounding / sizing
		style.WindowRounding = 4.0f;
		style.FrameRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.TabRounding = 3.0f;
		style.PopupRounding = 4.0f;
		style.ScrollbarRounding = 8.0f;

		// Spacing tuned like an editor
		style.FramePadding = ImVec2(8, 6);
		style.ItemSpacing = ImVec2(10, 6);
		style.ItemInnerSpacing = ImVec2(8, 6);

		// Larger default font scale a touch for readability
		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = 1.0f; // set to >1.0f bigger font by default
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
		ImGui::SetNextWindowSize(ImVec2(240, 140), ImGuiCond_FirstUseEver);
		ImGui::Begin("Stats");

		ImGui::Text("FPS: %.1f", m_fps);
		ImGui::Separator();
		ImGui::Text("Draw Calls: %u", stats.DrawCalls);
		ImGui::Text("Quads: %u", stats.NumOfQuads);
		ImGui::Text("Vertices: %u", stats.GetQuadVertices());
		ImGui::Text("Indices: %u", stats.GetQuadIndices());

		ImGui::End();

		Renderer2D::s_Statistic.Reset();
	}

	void EditorLayer::DrawViewport()
	{
		auto window = App::Get().GetWindow().GetNativeWindow();
		ImGui::Begin("Viewport");
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
		{
			m_IsViewPortFocus = true;
		}
		else
		{
			m_IsViewPortFocus = false;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (viewportPanelSize.x != m_FBO->GetWidth() || viewportPanelSize.y != m_FBO->GetHeight())
		{
			m_FBO->OnResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
		}

		m_Scene.UpdatePrimaryCameraAspect((float)m_FBO->GetWidth() / (float)m_FBO->GetHeight());

		ImGui::Image((ImTextureID)(uintptr_t)m_FBO->getColorTexture(),
			ImVec2((float)m_FBO->GetWidth(), (float)m_FBO->GetHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::PopStyleVar();
		ImGui::End();
	}

	void EditorLayer::DrawPropertiesPanel()
	{
		ImGui::SetNextWindowSize(ImVec2(360, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Properties");

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 8));

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.18f, 0.18f, 0.19f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.20f, 0.20f, 0.21f, 1.0f));

		if (m_HasSelectedEntity)
		{
			DrawTagComponent();
			DrawTransformComponent();

			if (m_SelectedEntity.HasComponent<SpriteComponent2D>())
			{
				DrawSprite2DComponent();
			}

			if (m_SelectedEntity.HasComponent<CameraComponent>())
			{
				DrawCameraComponent();
			}
		}
		else
		{
			ImGui::TextDisabled("No entity selected");
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void EditorLayer::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float dragSpeed)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, 120.0f);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));

		// Calculate sizes
		float fullWidth = ImGui::GetContentRegionAvail().x;
		float buttonWidth = 28.0f; 
		float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		float fieldWidth = (fullWidth - (buttonWidth * 3) - (spacing * 4)) / 3.0f;

		float lineHeight = ImGui::GetFrameHeight();
		ImVec2 axisButtonSize = ImVec2(buttonWidth, lineHeight);

		// --- X axis (red)
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.78f, 0.18f, 0.18f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.86f, 0.28f, 0.28f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.72f, 0.12f, 0.12f, 1.0f));
		if (ImGui::Button("X", axisButtonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(fieldWidth);
		ImGui::DragFloat("##XVal", &values.x, dragSpeed, -FLT_MAX, FLT_MAX, "%.3f");
		ImGui::PopItemWidth();

		// --- Y axis (green)
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.72f, 0.26f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.34f, 0.82f, 0.34f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.62f, 0.20f, 1.0f));
		if (ImGui::Button("Y", axisButtonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(fieldWidth);
		ImGui::DragFloat("##YVal", &values.y, dragSpeed, -FLT_MAX, FLT_MAX, "%.3f");
		ImGui::PopItemWidth();

		// --- Z axis (blue)
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.38f, 0.88f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.48f, 0.98f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.14f, 0.28f, 0.78f, 1.0f));
		if (ImGui::Button("Z", axisButtonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(fieldWidth);
		ImGui::DragFloat("##ZVal", &values.z, dragSpeed, -FLT_MAX, FLT_MAX, "%.3f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void EditorLayer::DrawHierarhyPanel()
	{
		ImGui::SetNextWindowSize(ImVec2(260, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy");

		ImGui::TextDisabled("Current Scene:");
		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.18f, 0.18f, 0.19f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.20f, 0.20f, 0.21f, 1.0f));

		m_Scene.ForEachEntity([this](Entity entity) {
			auto& tag = entity.GetComponent<TagComponent>();
			bool isSelected = (m_HasSelectedEntity && m_SelectedEntity == entity);

			if (ImGui::Selectable(tag.Tag.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
			{
				m_SelectedEntity = entity;
				m_HasSelectedEntity = true;
			}
			});

		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::DrawTagComponent()
	{
		if (ImGui::CollapsingHeader("Tag", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& tag = m_SelectedEntity.GetComponent<TagComponent>();
			char buff[256] = {};
			strncpy(buff, tag.Tag.c_str(), sizeof(buff) - 1);

			ImGui::Columns(2, nullptr, false);

			ImGui::Text("Name");
			ImGui::NextColumn();

			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::InputText("##TagEdit", buff, sizeof(buff)))
				tag.Tag = buff;

			ImGui::Columns(1);
		}
	}

	void EditorLayer::DrawTransformComponent()
	{
		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& transform = m_SelectedEntity.GetComponent<TransformComponent>();

			DrawVec3Control("Position", transform.Position, 0.0f);
			DrawVec3Control("Rotation", transform.Rotation, 0.0f, 0.1f);
			DrawVec3Control("Scale", transform.Scale, 1.0f);
		}
	}

	void EditorLayer::DrawSprite2DComponent()
	{
		if (ImGui::CollapsingHeader("Sprite2D", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& sprite2D = m_SelectedEntity.GetComponent<SpriteComponent2D>();

			ImGui::Spacing();

			// --- Color ---
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnWidth(0, 120.0f);
			ImGui::Text("Color");
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::ColorEdit3("##SpriteColor", glm::value_ptr(sprite2D.Color), ImGuiColorEditFlags_NoInputs);
			ImGui::PopItemWidth();
			ImGui::Columns(1);
			ImGui::Separator();

			// --- Texture Picker ---
			ImGui::Text("Texture");
			ImGui::BeginChild("TexturePicker", ImVec2(0, 150), true);

			auto textures = AssetManager::GetAssets().GetTextures();

			float padding = 4.0f;       
			float thumbnailSize = 64.0f;
			int colIndex = 0;
			int columns = ImGui::GetContentRegionAvail().x / (thumbnailSize + padding) - 1;
			if (columns <= 1) columns = 1;

			if (ImGui::BeginTable("TextureTable", columns, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_SizingStretchSame))
			{
				ImGui::TableNextRow();

				// "No Texture" option
				ImGui::TableNextColumn();
				bool isSelected = (sprite2D.Texture == nullptr);
				if (isSelected) ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.1f, 1.0f));

				ImTextureID nullTexID = 0;
				if (ImGui::ImageButton(nullTexID, ImVec2(thumbnailSize, thumbnailSize)))
					sprite2D.Texture = nullptr;

				if (isSelected) ImGui::PopStyleColor();

				colIndex = 1;
				for (auto& tex : textures)
				{
					if (colIndex >= columns)
					{
						ImGui::TableNextRow();
						colIndex = 0;
					}

					ImGui::TableNextColumn();

					ImTextureID texID = (ImTextureID)(uintptr_t)tex.second->GetRendererID();
					bool isSelected = (sprite2D.Texture && (*sprite2D.Texture.get() == *tex.second.get()));

					if (isSelected) ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.1f, 1.0f));

					if (ImGui::ImageButton(texID, ImVec2(thumbnailSize, thumbnailSize), ImVec2(0, 1), ImVec2(1, 0)))
						sprite2D.Texture = tex.second;

					if (isSelected) ImGui::PopStyleColor();

					colIndex++;
				}

				ImGui::EndTable();
			}

			ImGui::EndChild();
		}
	}

	void EditorLayer::DrawCameraComponent()
	{
		if (ImGui::CollapsingHeader("CameraComponent", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto& transform = m_SelectedEntity.GetComponent<TransformComponent>();
			auto& cam = m_SelectedEntity.GetComponent<CameraComponent>();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
			ImGui::Columns(2, nullptr, false);
			ImGui::SetColumnWidth(0, 120.0f);

			ImGui::Text("Type");
			ImGui::NextColumn();

			int selectedIndex = (cam.type == CameraTypes::Orthographic) ? 0 : 1;
			const char* items[] = { "Orthographic", "Perspective" };
			if (ImGui::Combo("##CameraType", &selectedIndex, items, IM_ARRAYSIZE(items)))
			{
				cam.type = (selectedIndex == 0) ? CameraTypes::Orthographic : CameraTypes::Perspective;
				if (cam.type == CameraTypes::Orthographic)
					transform.Position.z = 0.0f;
				cam.OnTypeChange();
			}
			ImGui::NextColumn();

			ImGui::Separator();

			if (cam.type == CameraTypes::Orthographic)
			{
				ImGui::Text("Zoom");
				ImGui::NextColumn();
				ImGui::DragFloat("##Zoom", &cam.Zoom, 0.1f, 0.1f, 10.0f);
				ImGui::NextColumn();
			}

			if (cam.type == CameraTypes::Perspective)
			{
				ImGui::Text("FOV");
				ImGui::NextColumn();
				ImGui::DragFloat("##FOV", &cam.fov, 1, 30, 110);
				ImGui::NextColumn();
			}

			ImGui::Text("Near Plane");
			ImGui::NextColumn();
			ImGui::DragFloat("##Near", &cam.nearPlane, 0.01f, -1.0f, cam.farPlane - 0.01f);
			ImGui::NextColumn();

			ImGui::Text("Far Plane");
			ImGui::NextColumn();
			ImGui::DragFloat("##Far", &cam.farPlane, 0.01f, cam.nearPlane + 0.01f, 10000.0f);
			ImGui::NextColumn();

			ImGui::Text("Primary");
			ImGui::NextColumn();
			ImGui::Checkbox("##Primary", &cam.primary);
			ImGui::NextColumn();

			ImGui::Separator();

			ImGui::Columns(1);
			if (ImGui::CollapsingHeader("Debug Info"))
			{
				float fullWidth = ImGui::GetContentRegionAvail().x;
				float indent = ImGui::GetStyle().IndentSpacing;
				fullWidth += indent;

				ImGui::Text("Aspect Ratio: %.3f", cam.aspectRatio);
				ImGui::Text("Euler Angles: Yaw %.1f, Pitch %.1f", cam.yaw, cam.pitch);

				ImGui::Text("Direction Vectors:");
				ImGui::BeginDisabled();
				ImGui::PushItemWidth(fullWidth);

				ImGui::Text("Front"); ImGui::SameLine();
				ImGui::PushItemWidth(-1);
				ImGui::InputFloat3("##Front", glm::value_ptr(cam.front));
				ImGui::PopItemWidth();

				ImGui::Text("Up"); ImGui::SameLine();
				ImGui::PushItemWidth(-1);
				ImGui::InputFloat3("##Up", glm::value_ptr(cam.up));
				ImGui::PopItemWidth();

				ImGui::Text("Right"); ImGui::SameLine();
				ImGui::PushItemWidth(-1);
				ImGui::InputFloat3("##Right", glm::value_ptr(cam.right));
				ImGui::PopItemWidth();

				ImGui::PopItemWidth();
				ImGui::EndDisabled();

				ImGui::Text("Matrices:");
				ImGui::BeginDisabled();
				ImGui::PushItemWidth(fullWidth);
				for (int i = 0; i < 4; ++i)
				{
					ImGui::Text("Proj Row %d", i); ImGui::SameLine();
					ImGui::PushItemWidth(-1);
					ImGui::InputFloat4(("Proj##" + std::to_string(i)).c_str(), &cam.projectionMatrix[i][0]);
					ImGui::PopItemWidth();
				}
				ImGui::Separator();
				for (int i = 0; i < 4; ++i)
				{
					ImGui::Text("View Row %d", i); ImGui::SameLine();
					ImGui::PushItemWidth(-1);
					ImGui::InputFloat4(("View##" + std::to_string(i)).c_str(), &cam.viewMatrix[i][0]);
					ImGui::PopItemWidth();
				}
				ImGui::Separator();
				for (int i = 0; i < 4; ++i)
				{
					ImGui::Text("VP Row %d", i); ImGui::SameLine();
					ImGui::PushItemWidth(-1);
					ImGui::InputFloat4(("VP##" + std::to_string(i)).c_str(), &cam.viewProjectionMatrix[i][0]);
					ImGui::PopItemWidth();
				}
				ImGui::PopItemWidth();
				ImGui::EndDisabled();
			}
			ImGui::Columns(1);
			ImGui::PopStyleVar();
		}
	}
}