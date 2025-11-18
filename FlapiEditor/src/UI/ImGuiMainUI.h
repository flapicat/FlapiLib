#pragma once
#include "Core/Core.h"
#include "Render/FrameBuffer.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace FL
{
	void SetUpImGuiStyle();
	void DrawMenuBar();
	void DrawStats(float FPS);
	void DrawViewport(const Ref<FrameBuffer> framebuffer, Scene& scene);

	struct UIDrawningData
	{
		static Entity	s_PopupSelectedEntity;
		static Entity	s_SelectedEntity	 ;
		static bool		s_HasSelectedEntity	 ;
		static bool		s_IsViewPortFocus	 ;
	};

	inline static UIDrawningData s_UIData;
}