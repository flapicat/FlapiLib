#include "Player.h"

Rect::Rect(Rects playerNum)
	:m_RectNum(playerNum)
{
	m_Transform.Position = { 0.05f,0.0f,0.0f };
	m_Transform.Size = { 0.05f,0.5f,0.0f };
}

Rect::~Rect()
{
}

void Rect::OnUpdate(FL::TimeStep ts)
{
	if (m_RectNum == Rect1)
	{
		if (FL::Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_Transform.Position.y += m_RectSpeed * ts;
		}
		if (FL::Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_Transform.Position.y -= m_RectSpeed * ts;
		}
	}

	if (m_RectNum == Rect2)
	{
		if (FL::Input::IsKeyPressed(GLFW_KEY_UP))
		{
			m_Transform.Position.y += m_RectSpeed * ts;
		}
		if (FL::Input::IsKeyPressed(GLFW_KEY_DOWN))
		{
			m_Transform.Position.y -= m_RectSpeed * ts;
		}
	}

	if (FL::Input::IsKeyPressed(GLFW_KEY_A))
	{
		m_Transform.Position.x -= m_RectSpeed * ts;
	}
	if (FL::Input::IsKeyPressed(GLFW_KEY_D))
	{
		m_Transform.Position.x += m_RectSpeed * ts;
	}

	static float WindowTopBound = 1.0f;
	static float WindowBottomBound = -1.0f;
	if (m_Transform.Position.y >= WindowTopBound - m_Transform.Size.y/2)
	{
		m_Transform.Position.y = WindowTopBound - m_Transform.Size.y / 2;
	}
	if (m_Transform.Position.y <= WindowBottomBound + m_Transform.Size.y / 2)
	{
		m_Transform.Position.y = WindowBottomBound + m_Transform.Size.y / 2;
	}
}

void Rect::OnWindowResize(FL::Camera camera)
{
	if (m_RectNum == Rect1)
	{
		m_Transform.Position.x = -camera.GetAspectRatio() + 0.5f;
	}
	if (m_RectNum == Rect2)
	{
		m_Transform.Position.x = camera.GetAspectRatio() - 0.5f;
	}
}
