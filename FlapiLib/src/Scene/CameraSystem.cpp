#include <pch.h>
#include "CameraSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


namespace FL {

    void CameraSystem::OnUpdate(entt::registry& registry, TimeStep ts) {
        auto view = registry.view<TransformComponent, CameraComponent>();

        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& cam = view.get<CameraComponent>(entity);

            glm::vec3 front;
            front.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
            front.y = sin(glm::radians(cam.pitch));
            front.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
            cam.front = glm::normalize(front);
            cam.right = glm::normalize(glm::cross(cam.front, cam.up));

			RecalculateProjectionViewMatrixOfCam(transform, cam);
        }
    }

    void CameraSystem::OnEvent(entt::registry& registry, Event& e)
    {
        FL::EventHandler handler(e);
        auto view = registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			auto& cam = view.get<CameraComponent>(entity);
            handler.Handle<FL::MouseMovedEvent>([&](const FL::MouseMovedEvent& ev) { OnMouseMoved(cam, ev); });
            handler.Handle<FL::MouseScrollEvent>([&](const FL::MouseScrollEvent& ev) { OnMouseScrolled(cam,ev); });
            handler.Handle<FL::WindowResizeEvent>([&](const FL::WindowResizeEvent& ev) { OnWindowResize(transform,cam,ev); });
        }
    }

	void CameraSystem::RecalculateProjectionViewMatrixOfCam(TransformComponent& transform, CameraComponent& camComponent)
	{
		if (camComponent.type == CameraTypes::Orthographic)
		{
			camComponent.projectionMatrix = glm::ortho(-camComponent.aspectRatio * camComponent.Zoom, camComponent.aspectRatio * camComponent.Zoom, -1.0f * camComponent.Zoom, 1.0f * camComponent.Zoom, camComponent.nearPlane, camComponent.farPlane);
		}

		if (camComponent.type == CameraTypes::Perspective)
		{
			camComponent.projectionMatrix = glm::perspective(glm::radians(camComponent.fov), camComponent.aspectRatio, camComponent.nearPlane, camComponent.farPlane);
		}

		camComponent.viewMatrix = glm::lookAt(transform.Position, transform.Position + camComponent.front,camComponent.up);
		camComponent.viewProjectionMatrix = camComponent.projectionMatrix * camComponent.viewMatrix;

	}

	void CameraSystem::OnMouseScrolled(CameraComponent& camComponent, const MouseScrollEvent& e)
	{
		if (camComponent.ScrollEnabled)
		{
			if (camComponent.type == CameraTypes::Orthographic)
			{
				camComponent.Zoom = glm::clamp(camComponent.Zoom - e.GetYoffset() * camComponent.zoomSpeed, 0.25f, 4.0f);

				float orthoLeft = -camComponent.zoomSpeed * camComponent.Zoom;
				float orthoRight = camComponent.zoomSpeed * camComponent.Zoom;
				float orthoBottom = -1.0f * camComponent.Zoom;
				float orthoTop = 1.0f * camComponent.Zoom;

				camComponent.projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, camComponent.nearPlane, camComponent.farPlane);
				camComponent.viewProjectionMatrix = camComponent.projectionMatrix * camComponent.viewMatrix;
				LOG_ERROR("Scroll: Zoom: {0}", camComponent.Zoom);
			}

			if (camComponent.type == CameraTypes::Perspective)
			{
				camComponent.fov = glm::clamp(camComponent.fov - e.GetYoffset() * camComponent.zoomSpeed, 15.0f, 90.0f);

				camComponent.projectionMatrix = glm::perspective(glm::radians(camComponent.fov), camComponent.zoomSpeed, camComponent.nearPlane, camComponent.farPlane);
				camComponent.viewProjectionMatrix = camComponent.projectionMatrix * camComponent.viewMatrix;
			}
		}
	}

	void CameraSystem::OnMouseMoved(CameraComponent& camComponent, const MouseMovedEvent& e)
	{
		if (camComponent.type == CameraTypes::Perspective)
		{
			float xpos = e.GetxPos();
			float ypos = e.GetyPos();

			if (camComponent.FirstMouse)
			{
				camComponent.LastX = xpos;
				camComponent.LastY = ypos;
				camComponent.FirstMouse = false;
			}

			float xOffset = xpos - camComponent.LastX;
			float yOffset = camComponent.LastY - ypos;
			camComponent.LastX = xpos;
			camComponent.LastY = ypos;

			const float sensitivity = 0.1f;
			xOffset *= sensitivity;
			yOffset *= sensitivity;

			camComponent.yaw += xOffset;
			camComponent.pitch += yOffset;

			camComponent.pitch = glm::clamp(camComponent.pitch, -89.0f, 89.0f);

			glm::vec3 front;
			front.x = cos(glm::radians(camComponent.yaw)) * cos(glm::radians(camComponent.pitch));
			front.y = sin(glm::radians(camComponent.pitch));
			front.z = sin(glm::radians(camComponent.yaw)) * cos(glm::radians(camComponent.pitch));
			camComponent.front = glm::normalize(front);
			
			camComponent.right = glm::normalize(glm::cross(camComponent.front, camComponent.up));
		}
	}

	void CameraSystem::OnWindowResize(TransformComponent& transform, CameraComponent& camComponent, const WindowResizeEvent& e)
	{
		camComponent.aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		RecalculateProjectionViewMatrixOfCam(transform, camComponent);
	}

}
