#pragma once
#include "Camera.h"

#include "Core/TimeStep.h"
#include <Event/MouseEvent.h>

namespace FL
{
	class CameraController : public Camera
	{
	public:
		CameraController(float aspectRatio, CameraType type);
		~CameraController();

		void OnUpdate(TimeStep ts);
		void OnMouseScrolled(const MouseScrollEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
	private:
		float m_CameraSpeed = 5.0f;
		float m_ZoomSpeed = 0.1f;
		float m_Zoom = 1.0f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		bool m_FirstMouse = true;
	};

}