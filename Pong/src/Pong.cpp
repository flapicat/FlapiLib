#include "Core/EntryPoint.h"
#include "PongLayer.h"

class Pong : public FL::App
{
public:
	Pong()
	{
		PushLayer(new PongLayer());
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