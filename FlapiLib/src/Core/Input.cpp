#include "pch.h"
#include "Input.h"

#include "App.h"
#include <GLFW/glfw3.h>

namespace FL
{
    std::unordered_map<uint32_t, bool> Input::s_KeyState;
    std::unordered_map<uint32_t, bool> Input::s_MouseButtonState;

	bool Input::IsKeyPressed(uint32_t keycode)
	{
		auto window = App::Get().GetWindow().GetNativeWindow();
		return glfwGetKey(window, keycode) == GLFW_PRESS;
	}

	bool Input::OnKeyPressed(uint32_t keycode)
	{
        bool currentlyPressed = IsKeyPressed(keycode);
        bool& wasPressed = s_KeyState[keycode];

        if (currentlyPressed && !wasPressed)
        {
            wasPressed = true;
            return true;
        }
        else if (!currentlyPressed)
        {
            wasPressed = false;
        }

        return false;
	}

    bool Input::IsMouseButtonPressed(uint32_t keycode)
    {
        auto window = App::Get().GetWindow().GetNativeWindow();
        return glfwGetMouseButton(window, keycode) == GLFW_PRESS;
    }

    bool Input::OnMouseButtonPressed(uint32_t keycode)
    {
        bool currentlyPressed = IsMouseButtonPressed(keycode);
        bool& wasPressed = s_MouseButtonState[keycode];

        if (currentlyPressed && !wasPressed)
        {
            wasPressed = true;
            return true;
        }
        else if (!currentlyPressed)
        {
            wasPressed = false;
        }

        return false;
    }
}