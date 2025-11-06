#include "Core/EntryPoint.h"
#include "Layer.h"

class Sandbox : public FL::App
{
public:
	Sandbox()
	{
		PushLayer(new Layer());
	}
	~Sandbox()
	{
	}
private:
};

FL::App* FL::CreateApp()
{
	return new Sandbox();
}