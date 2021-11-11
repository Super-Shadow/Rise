#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <iomanip>

#include <thread>
#include <utility>

namespace Rise
{
	struct ProfileResult
	{
		std::string Name;
		std::chrono::duration<double, std::micro> Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;

		explicit InstrumentationSession(std::string name) : Name(std::move(name)) {}
	};

	class Instrumentor
	{
	public:
		Instrumentor() : m_CurrentSession(nullptr) {}

		/* Explicitly disallow copying. */
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor& operator= (const Instrumentor&) = delete;

		Instrumentor(Instrumentor&&) = delete;
		Instrumentor& operator=(Instrumentor&&) = delete;
		~Instrumentor() = default;
		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard guard(m_Mutex);
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger())
				{ // Edge case: BeginSession() might be before Log::Init()
					RS_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}

			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = CreateScope<InstrumentationSession>(name);
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger())
				{ // Edge case: BeginSession() might be before Log::Init()
					RS_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard guard(m_Mutex);
			InternalEndSession();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	private:
		friend class InstrumentationTimer; // Allow this class access to private to use WriteProfile

		void WriteProfile(const ProfileResult& result)
		{
			std::ostringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ", {";
			json << R"("cat":"function",)";
			json << R"("dur":)" << result.ElapsedTime.count() << ',';
			json << R"("name":")" << result.Name << R"(",)";
			json << R"("ph":"X",)";
			json << R"("pid":0,)";
			json << R"("tid":)" << result.ThreadID << ",";
			json << R"("ts":)" << result.Start.count();
			json << '}';
			
			std::lock_guard guard(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		void WriteHeader()
		{
			m_OutputStream << R"({"otherData": {},"traceEvents":[{})";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		// Note: you must already own lock on m_Mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_CurrentSession) 
			{
				WriteFooter();
				m_OutputStream.close();
				m_CurrentSession.reset();
				//m_CurrentSession = nullptr;
			}
		}

		std::mutex m_Mutex;
		Scope<InstrumentationSession> m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		explicit InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		InstrumentationTimer(InstrumentationTimer const&) = delete;
		InstrumentationTimer& operator=(InstrumentationTimer const&) = delete;

		InstrumentationTimer(InstrumentationTimer&&) = delete;
		InstrumentationTimer& operator=(InstrumentationTimer&&) = delete;

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			const auto endTimepoint = std::chrono::steady_clock::now();
			const auto start = std::chrono::duration<double, std::micro>{ m_StartTimepoint.time_since_epoch() };
			const auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, start, elapsedTime, std::this_thread::get_id() });
			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils // https://stackoverflow.com/questions/59532907/how-to-remove-a-substring-from-a-string-view-at-compile-time - More info on this!
	{
		template <size_t N>
		struct ChangeResult // Needed to allocate a char array and return it inside a struct as it would point to some existing, contiguous block of data (stack)
		{
			char Data[N];
		};

		template <size_t N, size_t K> // Grabs the array size of the strings passed
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				{
					matchIndex++;
				}
				if (matchIndex == K - 1)
				{
					srcIndex += matchIndex;
				}
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define RS_PROFILE 0
#if RS_PROFILE
	// Resolve which function signature macro will be used.
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define RS_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define RS_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__FUNCSIG__)
		#define RS_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define RS_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define RS_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define RS_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define RS_FUNC_SIG __func__
	#else
		#define RS_FUNC_SIG "HZ_FUNC_SIG unknown!"
	#endif

	#define RS_PROFILE_BEGIN_SESSION(name, filepath) ::Rise::Instrumentor::Get().BeginSession(name, filepath)
	#define RS_PROFILE_END_SESSION() ::Rise::Instrumentor::Get().EndSession()
	#define RS_PROFILE_SCOPE(name) constexpr auto fixedName = ::Rise::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); ::Rise::InstrumentationTimer timer##__LINE__(fixedName.Data)
	#define RS_PROFILE_FUNCTION() RS_PROFILE_SCOPE(RS_FUNC_SIG)
#else
	#define RS_PROFILE_BEGIN_SESSION(name, filepath)
	#define RS_PROFILE_END_SESSION()
	#define RS_PROFILE_SCOPE(name)
	#define RS_PROFILE_FUNCTION()
#endif