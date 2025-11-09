#include "FlapiLib.h"
#include "Core/EntryPoint.h"
#include "pch.h"

#include <ImGui/imgui.h>

class Layer : public FL::Layer
{
public:
	Layer()
		:FL::Layer("Example")
	{
	}

	~Layer()
	{
	}

	virtual void OnAttach() override
	{
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/checkerboard.png", "checkerboard");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/container.png", "container");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/tilemap.png", "tilemap");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Texture, "Assets/Textures/awesomeface.png", "awesomeface");
		FL::AssetManager::LoadAssetFromFile(FL::AssetType::Model, "Assets/objects/backpack/backpack.obj", "backpack");

		m_Scene.OnInit();
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
		FL::Renderer::ClearColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
		FL::Renderer::ClearBuffer();

		m_Scene.OnRender();
	}

	virtual void OnEvent(FL::Event& e)
	{
		m_Scene.OnEvent(e);
		FL::EventHandler handler(e);
	}

	virtual void OnImGuiRender() override
	{
		auto& stats = FL::Renderer2D::s_Statistic;

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
};

class Sandbox : public FL::App
{
public:
	Sandbox()
	{
		PushLayer(new Layer());
	}
	~Sandbox()
	{
	}
private:
};

FL::App* FL::CreateApp()
{
	return new Sandbox();
}