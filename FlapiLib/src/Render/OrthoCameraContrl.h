#pragma once
#include "OrthoCamera.h"

#include "Core/TimeStep.h"
#include <Event/MouseEvent.h>

namespace FL
{
	class OrthoCameraContrl : public OrthoCamera
	{
	public:
		OrthoCameraContrl(float aspectRatio);
		~OrthoCameraContrl();

		void OnUpdate(TimeStep ts);
		void OnMouseScrolled(const MouseScrollEvent& e);
	private:
		float m_CameraSpeed = 5.0f; 
		float m_ZoomSpeed = 0.1f;
		float m_Zoom = 1.0f;
	};

}

