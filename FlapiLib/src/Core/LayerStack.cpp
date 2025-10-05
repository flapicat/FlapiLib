#include "pch.h"
#include "LayerStack.h"

namespace FL
{
	LayerStack::LayerStack()
	{
	}
	LayerStack::~LayerStack()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetach();
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		m_Layers.erase(it);
	}
}