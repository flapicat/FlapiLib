#include "Core/EntryPoint.h"
#include "Layer.h"

class FlapiEditor : public FL::App
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

FL::App* FL::CreateApp()
{
	return new FlapiEditor();
}