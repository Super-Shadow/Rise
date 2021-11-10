#pragma once
#include "Base.h"

#ifdef RS_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Rise::Log::Init();

	RS_PROFILE_BEGIN_SESSION("Startup", "RiseProfile-Startup.json");
	/*RS_CORE_WARN("Initialised Log!");
	auto a = 5;
	RS_INFO("Hello! Var={0}", a);*/
	const auto app = Rise::CreateApplication();
	RS_PROFILE_END_SESSION();

	RS_PROFILE_BEGIN_SESSION("Runtime", "RiseProfile-Runtime.json");
	app->Run();
	RS_PROFILE_END_SESSION();

	RS_PROFILE_BEGIN_SESSION("Shutdown", "RiseProfile-Shutdown.json");
	delete app;
	RS_PROFILE_END_SESSION();
}

#endif