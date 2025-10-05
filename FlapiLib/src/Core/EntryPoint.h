#pragma once
#include "App.h"
#include "Log.h"

extern FL::App* FL::CreateApp();

int main(int argc, char** argv)
{
	FL::Log::init();
	auto app = FL::CreateApp();
	app->Run();
	delete app;
}