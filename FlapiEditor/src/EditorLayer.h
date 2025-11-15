#include "FlapiLib.h"
#include "pch.h"

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

		void DrawMenuBar();
		void DrawStats();
		void DrawViewport();
		void DrawPropertiesPanel();
		void DrawHierarhyPanel();
	private:
		float m_fps = 0.0f;
		 Scene m_Scene;
		Ref< FrameBuffer> m_FBO;
		Entity m_SelectedEntity;
		bool m_HasSelectedEntity = false;
	};
}