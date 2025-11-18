#pragma once
#include <ImGui/imgui.h>
#include "HierarhyPanelUI.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"

#include "ImGuiMainUI.h"

namespace FL
{
	void DrawHierarhyPanel(Scene& scene)
	{
		ImGui::SetNextWindowSize(ImVec2(260, 400), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy");

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("HierarchyPanel_Empty");
		}

		if (ImGui::BeginPopup("HierarchyPanel_Empty"))
		{
			if (ImGui::MenuItem("Add")) { scene.CreateEntity(); }
			ImGui::EndPopup();
		}


		ImGui::TextDisabled("Current Scene:");
		ImGui::Separator();

		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.12f, 0.12f, 0.13f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.18f, 0.18f, 0.19f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.20f, 0.20f, 0.21f, 1.0f));

		Entity entityToDelete = Entity{};

        static const char* PopupName = "EntityContextMenu";

        scene.ForEachEntity([&scene](Entity entity) {
            auto& tag = entity.GetComponent<TagComponent>();
            bool isSelected = (s_UIData.s_HasSelectedEntity && s_UIData.s_SelectedEntity == entity);

            if (ImGui::Selectable(tag.Tag.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns))
            {
                s_UIData.s_SelectedEntity = entity;
                s_UIData.s_HasSelectedEntity = true;
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                s_UIData.s_PopupSelectedEntity = entity; 
                ImGui::OpenPopup(PopupName);             
            }
            });

        if (ImGui::BeginPopup(PopupName))
        {
            auto& entity = s_UIData.s_PopupSelectedEntity;
            auto& tag = entity.GetComponent<TagComponent>();

            ImGui::Text("%s Options", tag.Tag.c_str());
            ImGui::Separator();

            if (ImGui::MenuItem("Delete"))
            {
                scene.DeleteEntity(entity);
                if (s_UIData.s_SelectedEntity == entity)
                    s_UIData.s_HasSelectedEntity = false;
            }

            ImGui::EndPopup();
        }

		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}