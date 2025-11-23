#include "FlapiLib.h"
#include "pch.h"
#include <imgui.h>

namespace FL
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep ts) override;
		virtual void OnRender() override;
		virtual void OnEvent(Event& e);
		virtual void OnImGuiRender() override;
	private:
		void OnKeyPressed(const KeyPressedEvent& event);
	private:
		float m_fps = 0.0f;
		Scene m_Scene;
		Ref<FrameBuffer> m_FBO;
	};
}