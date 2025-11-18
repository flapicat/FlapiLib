#pragma once
#include "ImGuiMainUI.h"

#include <ImGui/imgui.h>
#include "Render/Renderer2D.h"
#include "Core/App.h"

namespace FL
{
	Entity UIDrawningData::s_PopupSelectedEntity{};
	Entity UIDrawningData::s_SelectedEntity{};
	bool   UIDrawningData::s_HasSelectedEntity = false;
	bool   UIDrawningData::s_IsViewPortFocus = false;

	void SetUpImGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 0.95f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.09f, 0.95f);
		colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 0.6f);

		colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.18f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.27f, 1.00f);

		colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);

		colors[ImGuiCol_Button] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);

		colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.14f, 0.15f, 1.00f);

		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);

		colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.06f, 0.06f, 0.07f, 1.00f);

		colors[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.13f, 1.00f);

		style.WindowRounding = 4.0f;
		style.FrameRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.TabRounding = 3.0f;
		style.PopupRounding = 4.0f;
		style.ScrollbarRounding = 8.0f;

		style.FramePadding = ImVec2(8, 6);
		style.ItemSpacing = ImVec2(10, 6);
		style.ItemInnerSpacing = ImVec2(8, 6);

		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = 1.0f;
	}

	void DrawMenuBar()
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

	void DrawStats(float FPS)
	{
		auto& stats = Renderer2D::s_Statistic;
		ImGui::SetNextWindowSize(ImVec2(240, 140), ImGuiCond_FirstUseEver);
		ImGui::Begin("Stats");

		ImGui::Text("FPS: %.1f", FPS);
		ImGui::Separator();
		ImGui::Text("Draw Calls: %u", stats.DrawCalls);
		ImGui::Text("Quads: %u", stats.NumOfQuads);
		ImGui::Text("Vertices: %u", stats.GetQuadVertices());
		ImGui::Text("Indices: %u", stats.GetQuadIndices());

		ImGui::End();

		Renderer2D::s_Statistic.Reset();
	}

	void DrawViewport(const Ref<FrameBuffer> framebuffer, Scene& scene)
	{
		auto window = App::Get().GetWindow().GetNativeWindow();
		ImGui::Begin("Viewport");
		if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
		{
			s_UIData.s_IsViewPortFocus = true;
		}
		else
		{
			s_UIData.s_IsViewPortFocus = false;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (viewportPanelSize.x != framebuffer->GetWidth() || viewportPanelSize.y != framebuffer->GetHeight())
		{
			framebuffer->OnResize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			scene.UpdatePrimaryCameraAspect((float)framebuffer->GetWidth() / (float)framebuffer->GetHeight());
		}

		ImGui::Image((ImTextureID)(uintptr_t)framebuffer->getColorTexture(),
			ImVec2((float)framebuffer->GetWidth(), (float)framebuffer->GetHeight()),
			ImVec2(0, 1), ImVec2(1, 0));

		ImGui::PopStyleVar();
		ImGui::End();
	}
}