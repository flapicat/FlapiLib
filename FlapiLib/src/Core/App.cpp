#include "pch.h"
#include "App.h"

namespace FL
{
	App* App::s_Instace = nullptr;

	App::App()
	{
		s_Instace = this;
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
		}
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
}