#pragma once
#include "pch.h"
#include "Event/Event.h"

#include <GLFW/glfw3.h>

namespace FL
{
	struct WindowInitData
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;

		WindowInitData(uint32_t width, uint32_t height, const std::string& title)
			:Width(width), Height(height), Title(title)
		{
		}
	};
	class Window
	{
		struct WindowProps
		{
			uint32_t Width;
			uint32_t Height;
			std::string Title;
			bool VSync;
			std::function<void(Event&)> EvCallback;
		};
	public:
		Window(const WindowInitData& data);
		~Window();
		void CreateWin(const WindowInitData& data);

		void OnUpdate();

		unsigned int GetWidth() const { return m_WindowProps.Width; }
		unsigned int GetHeight() const { return m_WindowProps.Height; }

		void SetEventCallback(const std::function<void(Event&)>& callback) { m_WindowProps.EvCallback = callback; }

		void SetVSync(bool enabled);
		bool IsVSync() const { return m_WindowProps.VSync; };

		GLFWwindow* GetNativeWindow() const { return m_Window; }
	private:
		GLFWwindow* m_Window;

		WindowProps m_WindowProps;
	};

}