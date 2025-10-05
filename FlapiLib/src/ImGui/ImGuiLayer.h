#pragma once
#include "Core/Layer.h"

namespace FL
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvent = block; }
	private:
		float m_Time = 0.0f;
		bool m_BlockEvent = true;
	};

}