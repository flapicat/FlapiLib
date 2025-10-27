#pragma once
#include <glm/glm.hpp>

#include "Core/TimeStep.h"


enum WindowCollisionSide
{
	None,
	LeftSide,
	RightSide,
	TopSide,
	BottomSide
};

class Ball
{
public:
	struct Transform
	{
		glm::vec3 Position = { 0.0f,0.0f,0.0f };
		glm::vec3 Size = { 1.0f,1.0f,0.0f };
	};
public:
	Ball();
	~Ball();

	void OnUpdate(FL::TimeStep ts);

	Transform& GetTransform() { return m_Transform; }
	void SetTransform(const Transform& transform) { m_Transform = transform; }

	WindowCollisionSide IsBallCollidingWindow();
private:
	Transform m_Transform;
	glm::vec3 m_velocity = {1.0f,1.0f, 0.0f};
	float m_BallSpeed = 1.0f;
};

