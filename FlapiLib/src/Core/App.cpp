#include "pch.h"
#include "App.h"

namespace FL
{
	App* App::s_Instace = nullptr;

	App::App()
	{
		s_Instace = this;
		WindowInitData data(1600, 900, "Example");
		m_Window.reset(new Window(data));
		m_Window->SetEventCallback([this](Event& e) { OnEvent(e); });

		m_ImGuiLayer = new ImGuiLayer();
		PushLayer(m_ImGuiLayer);
	}

	App::~App()
	{
	}

	void App::Run()
	{
		while (m_Running)
		{
			for (auto& layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			for (auto& layer : m_LayerStack)
			{
				layer->OnRender();
			}

			m_ImGuiLayer->Begin();
			for (auto& layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void App::OnEvent(Event& event)
	{
		EventHandler hander(event);
		hander.Handle<WindowCloseEvent>([this](const WindowCloseEvent& ev) {OnWindowCloseEvent(ev); });

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
			(*it)->OnEvent(event);
	}

	void App::Close()
	{
		m_Running = false;
	}

	void App::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void App::PopLayer(Layer* layer)
	{
		layer->OnDetach();
		m_LayerStack.PopLayer(layer);
	}

	void App::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		m_Running = false;
	}
}