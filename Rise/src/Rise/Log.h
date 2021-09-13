#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Rise
{
	class RISE_API Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger;}
		static std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger;}

	private:

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Core log macros
#define RS_CORE_FATAL(...) ::Rise::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define RS_CORE_ERROR(...) ::Rise::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RS_CORE_WARN(...)  ::Rise::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RS_CORE_INFO(...)  ::Rise::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RS_CORE_TRACE(...) ::Rise::Log::GetCoreLogger()->trace(__VA_ARGS__)

// Client log macros
#define RS_FATAL(...) ::Rise::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define RS_ERROR(...) ::Rise::Log::GetClientLogger()->error(__VA_ARGS__)
#define RS_WARN(...)  ::Rise::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RS_INFO(...)  ::Rise::Log::GetClientLogger()->info(__VA_ARGS__)
#define RS_TRACE(...) ::Rise::Log::GetClientLogger()->trace(__VA_ARGS__)
