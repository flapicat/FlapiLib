#pragma once

namespace FL
{
	class Input
	{
	public:
		static bool IsKeyPressed(uint32_t keycode);
		static bool OnKeyPressed(uint32_t keycode);
		static bool IsMouseButtonPressed(uint32_t keycode);
		static bool OnMouseButtonPressed(uint32_t keycode);
	private:
		static std::unordered_map<uint32_t, bool> s_KeyState;
		static std::unordered_map<uint32_t, bool> s_MouseButtonState;
	};
}

