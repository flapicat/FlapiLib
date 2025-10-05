#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace FL
{
	class Log
	{
	public:
		static void init();
		inline static Ref<spdlog::logger>& GetLogger() { return s_Logger; }
	private:
		static Ref<spdlog::logger> s_Logger;
	};

}

//CLIENT LOG MACROS
#define LOG_TRACE(...)     ::FL::Log::GetLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...)      ::FL::Log::GetLogger()->info(__VA_ARGS__);
#define LOG_WARN(...)      ::FL::Log::GetLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...)     ::FL::Log::GetLogger()->error(__VA_ARGS__);
#define LOG_FATAL(...)     ::FL::Log::GetLogger()->fatal(__VA_ARGS__);