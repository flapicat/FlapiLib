#include "pch.h"
#include "Window.h"

#include <glad/glad.h>
#include "Event/AppEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyboardEennt.h"

namespace FL
{
	Window::Window(const WindowInitData& data)
	{
		CreateWin(data);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::CreateWin(const WindowInitData& data)
	{
		m_WindowProps.Width = data.Width;
		m_WindowProps.Height = data.Height;
		m_WindowProps.Title = data.Title;

		if (!glfwInit())
		{
			LOG_ERROR("Failed to init GLFW!");
		}

		m_Window = glfwCreateWindow(m_WindowProps.Width,m_WindowProps.Height, m_WindowProps.Title.c_str(), nullptr, nullptr);
		if (!m_Window)
		{
			LOG_ERROR("Failed to initialize GLFW window!");
		}

		glfwSetWindowUserPointer(m_Window, this);
		glfwMakeContextCurrent(m_Window);
		glfwSwapInterval(true);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
		{
			LOG_ERROR("Failed to initialize Glad!");
		}

		LOG_INFO("OpenGL info:");
		LOG_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		LOG_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		LOG_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			win->m_WindowProps.EvCallback(event);
			});


		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			Window* win = (Window*)glfwGetWindowUserPointer(window);
			MouseScrollEvent event(xoffset, yoffset);
			win->m_WindowProps.EvCallback(event);
			});
	}

	void Window::OnUpdate()
	{
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}

	void Window::SetVSync(bool enabled)
	{
		m_WindowProps.VSync = enabled;
		glfwSwapInterval(enabled);
	}
}