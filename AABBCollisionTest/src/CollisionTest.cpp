#include "Core/EntryPoint.h"
#include "Layer.h"

class Pong : public FL::App
{
public:
	Pong()
	{
		PushLayer(new Layer());
	}
	~Pong()
	{
	}
private:
};

FL::App* FL::CreateApp()
{
	return new Pong();
}