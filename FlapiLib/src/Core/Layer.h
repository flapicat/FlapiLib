#pragma once
#include "pch.h"

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
	private:
		std::string m_LayerDebugName;
	};

}