#include "Player.h"

Player::Player(PlayerNum playerNum)
	:m_PLayerNum(playerNum)
{
}

Player::~Player()
{
}

void Player::OnUpdate(FL::TimeStep ts)
{
	if (m_PLayerNum == Player1)
	{
		if (FL::Input::IsKeyPressed(GLFW_KEY_W))
		{
			m_Transform.Position.y += m_playerSpeed * ts;
		}
		if (FL::Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_Transform.Position.y -= m_playerSpeed * ts;
		}
	}

	if (m_PLayerNum == Player2)
	{
		if (FL::Input::IsKeyPressed(GLFW_KEY_UP))
		{
			m_Transform.Position.y += m_playerSpeed * ts;
		}
		if (FL::Input::IsKeyPressed(GLFW_KEY_DOWN))
		{
			m_Transform.Position.y -= m_playerSpeed * ts;
		}
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

void Player::OnWindowResize(FL::Camera camera)
{
	if (m_PLayerNum == Player1)
	{
		m_Transform.Position.x = -camera.GetAspectRatio() + 0.5f;
	}
	if (m_PLayerNum == Player2)
	{
		m_Transform.Position.x = camera.GetAspectRatio() - 0.5f;
	}
}
