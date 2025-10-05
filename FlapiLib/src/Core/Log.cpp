#include "pch.h"
#include "Log.h"

#include"spdlog/sinks/stdout_color_sinks.h"

namespace FL
{
	Ref<spdlog::logger> Log::s_Logger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_Logger = spdlog::stdout_color_mt("Nexus");
		s_Logger->set_level(spdlog::level::trace);
	}
}