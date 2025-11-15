#include "FlapiLib.h"
#include "Core/EntryPoint.h"

#include "EditorLayer.h"


namespace FL
{
	class FlapiEditor : public App
	{
	public:
		FlapiEditor()
		{
			PushLayer(new EditorLayer());
		}
		~FlapiEditor()
		{
		}
	private:
	};

	App* CreateApp()
	{
		return new FlapiEditor();
	}
}