#pragma once
#include "pch.h"
#include "Event/Event.h"

namespace FL
{
	class Layer
	{
	public:
		Layer(const std::string& LayerName);
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}
		virtual void OnEvent(Event& e) {}
		virtual void OnImGuiRender() {};
	private:
		std::string m_LayerDebugName;
	};

}