#pragma once
#include "Window.h"
#include "LayerStack.h"

#include "Event/AppEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace FL
{
	class App
	{
	public:
		App();
		~App();

		void Run();
		void Close();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		inline static App& Get() { return *s_Instace; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		void OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		static App* s_Instace;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
	};

	App* CreateApp();
}