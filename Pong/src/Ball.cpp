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

	auto colliding = IsBallCollidingWindow();
	if (colliding == TopSide)
	{
		m_velocity.y *= -1;
	}
	if (colliding == BottomSide)
	{
		m_velocity.y *= -1;
	}
	if (colliding == RightSide)
	{
		m_velocity.x *= -1;
	}
	if (colliding == LeftSide)
	{
		m_velocity.x *= -1;
	}
}

WindowCollisionSide Ball::IsBallCollidingWindow()
{
	auto& window = FL::App::Get().GetWindow();
	float aspectRatio = (float)window.GetWidth() / (float)window.GetHeight();
	if (m_Transform.Position.y > 1.0f - m_Transform.Size.y/2)
	{
		return TopSide;
	}
	if (m_Transform.Position.y < -1.0f + m_Transform.Size.y/2)
	{
		return BottomSide;
	}
	if (m_Transform.Position.x > aspectRatio - m_Transform.Size.x/2)
	{
		return RightSide;
	}
	if (m_Transform.Position.x < -aspectRatio + m_Transform.Size.x/2)
	{
		return LeftSide;
	}
	return None;
}
