#pragma once
#include <glm/glm.hpp>

#include "Core/TimeStep.h"

#include "Transform.h"

class Ball
{
public:
	Ball();
	~Ball();

	void OnUpdate(FL::TimeStep ts);

	Transform& GetTransform() { return m_Transform; }
	void SetTransform(const Transform& transform) { m_Transform = transform; }

	void ProcessWindowCollision();

	glm::vec3& GetVelocity() { return m_velocity; };
private:
	Transform m_Transform;
	glm::vec3 m_velocity = {1.0f,1.0f, 0.0f};
	float m_BallSpeed = 1.0f;
};

