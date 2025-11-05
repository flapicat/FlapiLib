#include "PongLayer.h"

#include "Collision.h"
#include "InitMap.h"

#include <flapiLib.h>

Layer::Layer()
	:FL::Layer("Example"), m_Camera(FL::CameraType::Orthographic, (float)1600 / (float)900, FL::CameraMovement::Static), m_Player1(Rect::Rects::Rect1), m_Player2(Rect::Rects::Rect2)
{
}

Layer::~Layer()
{
}

void Layer::OnAttach()
{
	FL::App::Get().GetWindow().CenterWindow();

	m_Camera.SetPosition(glm::vec3(0.0, 0.0, 0.0));
	m_Player1.GetTransform().Position.x = -m_Camera.GetAspectRatio() + 0.25f;
	m_Player2.GetTransform().Position.x = m_Camera.GetAspectRatio() - 0.25f;

	FL::AssetManager::LoadAssetFromFile(FL::AssetType::Font,	"Assets/Fonts/Orange-Kid.otf",	"Orange_kid");
	FL::AssetManager::LoadAssetFromFile(FL::AssetType::Sound,	"Assets/Sounds/pong.wav",		"pong"		);
}

void Layer::OnDetach()
{

}

void Layer::OnUpdate(FL::TimeStep ts)
{
	m_fps = 1.0f / ts.GetSeconds();
	m_Camera.OnUpdate(ts);
	m_Player1.OnUpdate(ts);
	m_Player2.OnUpdate(ts);

	m_Ball.OnUpdate(ts);
	auto p1Collision = AABBvsAABB(m_Player1.GetTransform(), m_Ball.GetTransform());
	auto p2Collision = AABBvsAABB(m_Player2.GetTransform(), m_Ball.GetTransform());
	if (p1Collision.x)
	{
		m_Ball.GetVelocity().x *= -1;
	}
	if (p1Collision.y)
	{
		m_Ball.GetVelocity().y *= -1;
	}
	if (p2Collision.x)
	{
		m_Ball.GetVelocity().x *= -1;
	}
	if (p2Collision.y)
	{
		m_Ball.GetVelocity().y *= -1;
	}
}

void Layer::OnRender()
{
	auto& window = FL::App::Get().GetWindow();

	FL::Renderer::ClearColor(glm::vec4(0.1, 0.1, 0.1, 0.1));
	FL::Renderer::ClearBuffer();

	FL::Renderer2D::BeginScene(m_Camera);
	DrawBoard();

	FL::Renderer2D::DrawQuad(m_Player1.GetTransform().Position, m_Player1.GetTransform().Size, glm::vec4(1.0, 1.0, 1.0, 1.0));
	FL::Renderer2D::DrawQuad(m_Player2.GetTransform().Position, m_Player1.GetTransform().Size, glm::vec4(1.0, 1.0, 1.0, 1.0));

	FL::Renderer2D::DrawQuad(m_Ball.GetTransform().Position, m_Ball.GetTransform().Size, glm::vec4(1.0, 1.0, 1.0, 1.0));

	static auto font = FL::AssetManager::GetAssets().GetFont("Orange_kid");
	auto points = m_Ball.GetPoints();

	FL::Renderer2D::DrawTextWIndow(font, std::to_string(points.first), { window.GetWidth() / 5, 0,0 }, 2.0f, glm::vec3(0.5, 0.5, 0.5));
	FL::Renderer2D::DrawTextWIndow(font, std::to_string(points.second), { window.GetWidth() - window.GetWidth() / 5, 0,0 }, 2.0f, glm::vec3(0.5, 0.5, 0.5));

	if (points.first >= 15)
	{
		DisplayEndScreen("P1 WIN");
		m_Ball.setBallVelocity(glm::vec3(0.0f));

	}
	else if (points.second >= 15)
	{
		DisplayEndScreen("P2 WIN");
		m_Ball.setBallVelocity(glm::vec3(0.0f));
	}
	FL::Renderer2D::EndScene();
}

void Layer::OnEvent(FL::Event& e)
{
	FL::EventHandler handler(e);
	handler.Handle<FL::MouseMovedEvent>([this](const FL::MouseMovedEvent& ev) {m_Camera.OnMouseMoved(ev); });
	handler.Handle<FL::MouseScrollEvent>([this](const FL::MouseScrollEvent& ev) {m_Camera.OnMouseScrolled(ev); });
	handler.Handle<FL::WindowResizeEvent>([this](const FL::WindowResizeEvent& ev) 
		{
			m_Camera.OnWindowResize(ev); 
			m_Player1.OnWindowResize(m_Camera);
			m_Player2.OnWindowResize(m_Camera);
		});
}

void Layer::OnImGuiRender()
{
#if DEBUG
	auto& stats = FL::Renderer2D::s_Statistic;

	ImGui::Begin("Stats");
	ImGui::Text("FPS: %.1f", m_fps);
	ImGui::Text("Draw Calls: %u", stats.DrawCalls);
	ImGui::Text("Num Of Quads: %u", stats.NumOfQuads);
	ImGui::Text("Quad Vertices: %u", stats.GetQuadVertices());
	ImGui::Text("Quad Indices: %u", stats.GetQuadIndices());
	ImGui::End();

	ImGui::Begin("Players");
	ImGui::Text("Player1 pos: x:%.1f y:%.1f", m_Player1.GetTransform().Position.x, m_Player1.GetTransform().Position.y);
	ImGui::Text("Player2 pos: x:%.1f y:%.1f", m_Player2.GetTransform().Position.x, m_Player2.GetTransform().Position.y);
	ImGui::End();

	FL::Renderer2D::s_Statistic.Reset();
#endif
}

void Layer::DisplayEndScreen(std::string_view playerWon)
{
	static auto& window = FL::App::Get().GetWindow();
	static auto font = FL::AssetManager::GetAssets().GetFont("Orange_kid");
	FL::Renderer2D::DrawTextWIndow(font, playerWon.data(), { window.GetWidth() / 2, window.GetHeight() / 2 - 100,0}, 2.0f, glm::vec3(0.7, 0.7, 0.7), true);
	FL::Renderer2D::DrawTextWIndow(font, "Press Space To Continue", { window.GetWidth() / 2,  window.GetHeight() / 2 + 50 ,0}, 1.0f, glm::vec3(0.7, 0.7, 0.7), true);
}

