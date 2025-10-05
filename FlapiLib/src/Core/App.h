#pragma once
#include "LayerStack.h"

#include "Event/AppEvent.h"

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

		inline static App* Get() { return s_Instace; }
	private:
		void OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		static App* s_Instace;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	App* CreateApp();
}