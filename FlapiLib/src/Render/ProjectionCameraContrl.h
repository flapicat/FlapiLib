#pragma once
#include "ProjectionCamera.h"

#include "Core/TimeStep.h"
#include <Event/MouseEvent.h>

namespace FL
{
	class  ProjectionCameraContrl : public ProjectionCamera
	{
	public:
		ProjectionCameraContrl(float aspectRatio);
		~ProjectionCameraContrl();

		void OnUpdate(TimeStep ts);
		void OnMouseScrolled(const MouseScrollEvent& e);
		void OnMouseMoved(const MouseMovedEvent& e);
	private:
		float m_CameraSpeed = 2.0f;
		float m_ZoomSpeed = 1.0f;
		float m_Zoom = 1.0f;
		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		float m_LastX = 0.0f;
		float m_LastY = 0.0f;
		bool m_FirstMouse = true;
	};

}
