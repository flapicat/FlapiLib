#include "pch.h"
#include "Input.h"

#include "App.h"
#include <GLFW/glfw3.h>

namespace FL
{
	bool Input::IsKeyPressed(uint32_t keycode)
	{
		auto window = App::Get().GetWindow().GetNativeWindow();
		return glfwGetKey(window, keycode) == GLFW_PRESS;
	}
}