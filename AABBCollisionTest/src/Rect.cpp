#include "Rect.h"

#include "FlapiLib.h"

Rect::Rect()
{
	m_Transform.Position = { 0.0f,0.0f,0.0f };
	m_Transform.Size = { 0.5f,0.5f,0.0f };
}

Rect::~Rect()
{
}

void Rect::OnUpdate(FL::TimeStep ts)
{
	if (m_Movable)
	{
		if (FL::Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_Transform.Position.y += m_RectSpeed * ts;
		}
		if (FL::Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_Transform.Position.y -= m_RectSpeed * ts;
		}

		if (FL::Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_Transform.Position.x -= m_RectSpeed * ts;
		}
		if (FL::Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_Transform.Position.x += m_RectSpeed * ts;
		}
		CheckWindowCollision();
	}
}

void Rect::OnWindowResize(FL::Camera camera)
{
}

void Rect::CheckWindowCollision()
{
	float aspectRatio = (float)FL::App::Get().GetWindow().GetWidth() / (float)FL::App::Get().GetWindow().GetHeight();
	static float WindowTopBound = 1.0f;
	static float WindowBottomBound = -1.0f;
	if (m_Transform.Position.y >= WindowTopBound - m_Transform.Size.y / 2)
	{
		m_Transform.Position.y = WindowTopBound - m_Transform.Size.y / 2;
	}
	if (m_Transform.Position.y <= WindowBottomBound + m_Transform.Size.y / 2)
	{
		m_Transform.Position.y = WindowBottomBound + m_Transform.Size.y / 2;
	}
	if (m_Transform.Position.x >= aspectRatio - m_Transform.Size.x / 2)
	{
		m_Transform.Position.x = aspectRatio - m_Transform.Size.x / 2;
	}
	if (m_Transform.Position.x <= -aspectRatio + m_Transform.Size.x / 2)
	{
		m_Transform.Position.x = -aspectRatio + m_Transform.Size.x / 2;
	}
}
