#pragma once
#include "App.h"

extern FL::App* FL::CreateApp();

int main(int argc, char** argv)
{
	auto app = FL::CreateApp();
	app->Run();
	delete app;
}