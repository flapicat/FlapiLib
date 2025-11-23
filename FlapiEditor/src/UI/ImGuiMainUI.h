#pragma once
#include "Core/Core.h"
#include "Render/FrameBuffer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace FL
{
	void SetUpImGuiStyle();
	void DrawMenuBar(Scene& scene);
	void DrawStats(float FPS);
	void DrawViewport(const Ref<FrameBuffer> framebuffer, Scene& scene);

	void LoadScene(Scene& scene);
	void SaveScene(Scene& scene);
	void SaveAsScene(Scene& scene);
	void AddScene(Scene& scene);

	struct UIDrawningData
	{
		static Entity	s_PopupSelectedEntity;
		static Entity	s_SelectedEntity	 ;
		static bool		s_HasSelectedEntity	 ;
		static bool		s_IsViewPortFocus	 ;
	};

	inline static UIDrawningData s_UIData;
}