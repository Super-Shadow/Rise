#pragma once

#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define RS_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define RS_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define RS_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define RS_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define RS_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

// DLL support
#ifdef RS_PLATFORM_WINDOWS
	#if RS_DYNAMIC_LINK
		#ifdef RS_BUILD_DLL
			#define RISE_API __declspec(dllexport)
		#else
			#define RISE_API __declspec(dllimport)
		#endif
	#else
		#define RISE_API
	#endif
#else
	#error Rise only supports Windows!
#endif // End of DLL support

#ifdef RS_DEBUG
	#define RS_ENABLE_ASSERTS
#endif

#ifndef _MSC_VER
	#define abstract = 0 // Abstract doesn't exist on other compilers!
#endif

#ifdef RS_ENABLE_ASSERTS
	#if defined(RS_PLATFORM_WINDOWS)
		#ifdef _MSC_VER
			#define RS_DEBUGBREAK __debugbreak()
		#else
			#error "Compiler doesn't support debugbreak yet!"
		#endif
	#elif defined(RS_PLATFORM_LINUX)
		#include <signal.h>
		#define RS_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
		#define HZ_DEBUGBREAK()
	#endif
	#define RS_ASSERT(x, ...) { if(!(x)) { RS_ERROR("Assertion Failed: {0}", __VA_ARGS__); RS_DEBUGBREAK; } } static_assert(true, "Require semi-colon")
	#define RS_CORE_ASSERT(x, ...) { if(!(x)) { RS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RS_DEBUGBREAK; } } static_assert(true, "Require semi-colon")
#else
	#define RS_ASSERT(x, ...) static_assert(true, "Require semi-colon")
	#define RS_CORE_ASSERT(x, ...) static_assert(true, "Require semi-colon")
#endif

#define BIT(x) (1 << (x))

#define RS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) // NOLINT(modernize-avoid-bind, bugprone-macro-parentheses)


namespace Rise
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
