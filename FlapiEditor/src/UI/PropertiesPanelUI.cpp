#pragma once
#include "PropertiesPanelUI.h"
#include <ImGui/imgui.h>
#include "Scene/Components.h"
#include "Core/AssetManager.h"

#include "ImGuiMainUI.h"
#include "../NativeScripts/Scripts.h"

namespace FL
{
	void DrawPropertiesPanel()
	{
		ImGui::SetNextWindowSize(ImVec2(360, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Properties");

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 8));

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.18f, 0.18f, 0.19f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.20f, 0.20f, 0.21f, 1.0f));

		if (s_UIData.s_HasSelectedEntity)
		{
			if (s_UIData.s_SelectedEntity.HasComponent<TagComponent>())
			{
				DrawComponent<TagComponent>("Tag", DrawTagComponent, false);
			}

			if (s_UIData.s_SelectedEntity.HasComponent<TransformComponent>())
			{
				DrawComponent<TransformComponent>("Transform", DrawTransformComponent, false);
			}

			if (s_UIData.s_SelectedEntity.HasComponent<SpriteComponent2D>())
			{
				DrawComponent<SpriteComponent2D>("Sprite2D", DrawSprite2DComponent);
			}

			if (s_UIData.s_SelectedEntity.HasComponent<CameraComponent>())
			{
				DrawComponent<CameraComponent>("Camera", DrawCameraComponent);
			}

			if (s_UIData.s_SelectedEntity.HasComponent<NativeScriptingComponent>())
			{
				DrawComponent<NativeScriptingComponent>("Native Script", DrawNativeScriptingComponent);
			}

			ImGui::Separator();
			float panelWidth = ImGui::GetContentRegionAvail().x;
			float buttonWidth = 150.0f;
			ImVec2 buttonSize(buttonWidth, 30.0f);
			ImGui::SetCursorPosX((panelWidth - buttonWidth) * 0.5f);

			static const char* PopupName = "AddComponent";

			if (ImGui::Button("Add Component", buttonSize))
			{
				ImGui::OpenPopup(PopupName);
			}

			if (ImGui::BeginPopup(PopupName))
			{
				ImGui::Text("Components");
				ImGui::Separator();

				if (!s_UIData.s_SelectedEntity.HasComponent<TagComponent>())
				{
					if (ImGui::MenuItem("Tag"))
					{
						//ITS NOT EVEN POSSIBLE
					}
				}
				if (!s_UIData.s_SelectedEntity.HasComponent<TransformComponent>())
				{
					if (ImGui::MenuItem("Transform"))
					{
						//ITS NOT EVEN POSSIBLE
					}
				}

				if (!s_UIData.s_SelectedEntity.HasComponent<SpriteComponent2D>())
				{
					if (ImGui::MenuItem("Sprite 2D Renderer"))
					{
						s_UIData.s_SelectedEntity.AddComponent<SpriteComponent2D>();
					}
				}

				if (!s_UIData.s_SelectedEntity.HasComponent<CameraComponent>())
				{
					if (ImGui::MenuItem("Camera"))
					{
						s_UIData.s_SelectedEntity.AddComponent<CameraComponent>();
					}
				}

				if (!s_UIData.s_SelectedEntity.HasComponent<NativeScriptingComponent>())
				{
					if (ImGui::MenuItem("Native Script"))
					{
						s_UIData.s_SelectedEntity.AddComponent<NativeScriptingComponent>();
					}
				}

				ImGui::EndPopup();
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

	void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float dragSpeed)
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

	template<typename T>
	void DrawComponent(const std::string& name, std::function<void()>func, bool removeEnable)
	{
		bool removeComponent = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGui::PushID(name.c_str());

		bool open = ImGui::CollapsingHeader(name.c_str(), flags);

		float lineStartX = ImGui::GetItemRectMin().x;
		float lineEndX = ImGui::GetItemRectMax().x;

		float buttonSize = 25.0f;

		float buttonX = lineEndX - buttonSize;
		float buttonY = ImGui::GetItemRectMin().y;

		ImGui::SetCursorScreenPos(ImVec2(buttonX, buttonY));

		std::string popupName = "ComponentSettings" + name;

		if (ImGui::Button("+", ImVec2(buttonSize, buttonSize)))
			ImGui::OpenPopup(popupName.c_str());

		if (ImGui::BeginPopup(popupName.c_str()))
		{
			if (ImGui::MenuItem("Remove component",0,false, removeEnable))
			{
				removeComponent = true;
			}
			ImGui::EndPopup();
		}

		if (open)
			func();

		ImGui::PopID();

		if (s_UIData.s_SelectedEntity && removeComponent)
		{
			if (s_UIData.s_SelectedEntity.HasComponent<T>())
				s_UIData.s_SelectedEntity.RemoveComponent<T>();
		}
	}

	void DrawTagComponent()
	{
		auto& tag = s_UIData.s_SelectedEntity.GetComponent<TagComponent>();
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

	void DrawTransformComponent()
	{
		auto& transform = s_UIData.s_SelectedEntity.GetComponent<TransformComponent>();

		DrawVec3Control("Position", transform.Position, 0.0f);
		DrawVec3Control("Rotation", transform.Rotation, 0.0f, 0.1f);
		DrawVec3Control("Scale", transform.Scale, 1.0f);
	}

	void DrawSprite2DComponent()
	{
		auto& sprite2D = s_UIData.s_SelectedEntity.GetComponent<SpriteComponent2D>();

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
				{
					sprite2D.TextureName = tex.first;
					sprite2D.Texture = tex.second;
				}
				if (isSelected) ImGui::PopStyleColor();

				colIndex++;
			}

			ImGui::EndTable();
		}

		ImGui::EndChild();
		
	}

	void DrawCameraComponent()
	{
		auto& transform = s_UIData.s_SelectedEntity.GetComponent<TransformComponent>();
		auto& cam = s_UIData.s_SelectedEntity.GetComponent<CameraComponent>();

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

	void DrawNativeScriptingComponent()
	{
		auto& nsc = s_UIData.s_SelectedEntity.GetComponent<NativeScriptingComponent>();

		static int selectedIndex = 0;

		if (nsc.Instance)
		{
			ImGui::Text("Script Active: true");
		}
		else
		{
			ImGui::Text("Script Active: false");
		}

		if (ImGui::Combo("Script Type", &selectedIndex, [](void* data, int idx, const char** out_text) {
			auto& names = *static_cast<std::vector<std::string>*>(data);
			*out_text = names[idx].c_str();
			return true;
			}, &scriptNames, (int)scriptNames.size()))
		{
			if (nsc.DestroyScript)
				nsc.DestroyScript();

			const std::string& name = scriptNames[selectedIndex];
			if (name == "None")
				nsc.Bind<DummyScript>();
			if (name == "CameraControllerScript")
				nsc.Bind<CameraControllerScript>();
		}
	}
}