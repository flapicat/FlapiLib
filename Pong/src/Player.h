#pragma once
#include "FlapiLib.h"
#include <glm/glm.hpp>

#include <Core/TimeStep.h>

#include "Transform.h"

class Rect
{
public:
	enum Rects
	{
		Rect1,
		Rect2
	};
public:
	Rect(Rects playerNum);
	~Rect();

	void OnUpdate(FL::TimeStep ts);
	
	Transform& GetTransform() { return m_Transform; }
	void SetTransform(const Transform& transform) { m_Transform = transform; }

	void OnWindowResize(FL::Camera camera);
private:
	Rects m_RectNum = Rect1;
	Transform m_Transform;
	float m_RectSpeed = 2.0f;
};

