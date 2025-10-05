#include "FlapiLib.h"
#include "Core/EntryPoint.h"
#include "pch.h"

class ExampleLayer : public FL::Layer
{
public:
	ExampleLayer()
		:Layer("Example")
	{

	}
	~ExampleLayer()
	{

	}

	virtual void OnAttach() override
	{
		std::cout << "OnAttach\n";
	}
	virtual void OnDetach() override
	{
		std::cout << "OnDetach\n";
	}
	virtual void OnUpdate() override
	{
	}
	virtual void OnRender() override
	{
	}
};

class ExampleApp : public FL::App
{
public:
	ExampleApp()
	{
		PushLayer(new ExampleLayer());
	}
	~ExampleApp()
	{
	}
private:
};

FL::App* FL::CreateApp()
{
	return new ExampleApp();
}