#pragma once
#include "FlapiLib.h"
#include <glm/glm.hpp>

#include <Core/TimeStep.h>

#include "Transform.h"

class Rect
{
public:
	Rect();
	~Rect();

	void OnUpdate(FL::TimeStep ts);
	
	Transform& GetTransform() { return m_Transform; }
	void SetTransform(const Transform& transform) { m_Transform = transform; }

	void OnWindowResize(FL::Camera camera);

	void CheckWindowCollision();

public:
	bool m_Movable = false;
private:
	Transform m_Transform;
	float m_RectSpeed = 2.0f;
};

