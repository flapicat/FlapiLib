#pragma once
#include "Camera.h"

namespace FL
{
	class ProjectionCamera : public Camera
	{
	public:
		ProjectionCamera(float aspectRatio);
	protected:
		float m_Fov = 60.0f;
	};

}