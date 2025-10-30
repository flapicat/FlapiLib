#include "Ball.h"
#include "FlapiLib.h"

Ball::Ball()
{
	m_Transform.Position = {0.0f,0.0f,0.0f};
	m_Transform.Size = { 0.1f,0.1f,0.0f };
}

Ball::~Ball()
{
}

void Ball::OnUpdate(FL::TimeStep ts)
{
	m_Transform.Position.x += m_velocity.x * m_BallSpeed * ts;
	m_Transform.Position.y += m_velocity.y * m_BallSpeed * ts;

	ProcessWindowCollision();
}

void Ball::ProcessWindowCollision()
{
	static bool collision = false;
	auto& window = FL::App::Get().GetWindow();
	float aspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
	if (m_Transform.Position.y > 1.0f - m_Transform.Size.y/2)
	{
		m_Transform.Position.y = 1.0f - m_Transform.Size.y / 2;
		m_velocity.y *= -1;
		collision = true;
	}
	if (m_Transform.Position.y < -1.0f + m_Transform.Size.y/2)
	{
		m_Transform.Position.y = -1.0f + m_Transform.Size.y / 2;
		m_velocity.y *= -1;
		collision = true;
	}
	if (m_Transform.Position.x > aspectRatio - m_Transform.Size.x/2)
	{
		m_Transform.Position.x = aspectRatio - m_Transform.Size.x / 2;
		m_velocity.x *= -1;
		collision = true;
	}
	if (m_Transform.Position.x < -aspectRatio + m_Transform.Size.x/2)
	{
		m_Transform.Position.x = -aspectRatio + m_Transform.Size.x / 2;
		m_velocity.x *= -1;
		collision = true;
	}

	if (collision)
	{
		FL::SoundPlayer::PlaySound(FL::AssetManager::GetAssets().GetSound("pong"));
	}

	collision = false;
}
