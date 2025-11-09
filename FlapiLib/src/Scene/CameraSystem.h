#pragma once
#include <entt/entt.hpp>

#include "Core/TimeStep.h"
#include "Core/Input.h"

#include "Event/Events.h"

#include "Components.h"

namespace FL {
    class CameraSystem {
    public:
        static void OnUpdate(entt::registry& registry, TimeStep ts);
        static void OnEvent(entt::registry& registry, Event& e);
        
        // Cam Helpers
        static void RecalculateProjectionViewMatrixOfCam(TransformComponent& transform, CameraComponent& camComponent);
        //Events
        static void OnMouseScrolled(CameraComponent& camComponent, const MouseScrollEvent& e);
        static void OnMouseMoved(CameraComponent& camComponent, const MouseMovedEvent& e);
        static void OnWindowResize(TransformComponent& transform,CameraComponent& camComponent, const WindowResizeEvent& e);
    };
}