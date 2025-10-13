#pragma once
#include <glm/glm.hpp>

namespace FL
{
	enum CameraType
	{
		Orthographic, Perspective
	};

	class Camera
	{
	public:
		Camera(float aspectRatio)
			:m_AspectRatio(aspectRatio)
		{
		}

		const glm::mat4& GetViewProjectionMatrix() { return m_ProjectionViewMatrix; }
		const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
		const glm::vec3& GetPosition() { return m_Pos; }
		CameraType GetType() { return m_type; };
		void SetPosition(const glm::vec3& pos) { m_Pos = pos; }
	protected:
		CameraType m_type;
		glm::vec3 m_Pos = { 0.0f,0.0f,0.0f };
		float m_AspectRatio;
		float m_Near = -1, m_Far = 1;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionViewMatrix;

		glm::vec3 m_Up = { 0.0f,1.0f,0.0f };
		glm::vec3 m_Front = { 0.0f,0.0f,-1.0f };
		glm::vec3 m_Right = { 1.0f,0.0f,0.0f };
	};
}