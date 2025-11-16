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

		void OnKeyEvent(int keycode);

		void SetUpImGuiStyle();
		void DrawMenuBar();
		void DrawStats();
		void DrawViewport();
		void DrawPropertiesPanel();
		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float dragSpeed = 0.01f);
		void DrawHierarhyPanel();
		void DrawTagComponent();
		void DrawTransformComponent();
		void DrawSprite2DComponent();
		void DrawCameraComponent();
	private:
		float m_fps = 0.0f;
		Scene m_Scene;
		Ref< FrameBuffer> m_FBO;
		Entity m_SelectedEntity;
		bool m_IsViewPortFocus = false;
		bool m_HasSelectedEntity = false;
	};
}