#pragma once

#include <Peanut/Core/Assert.hpp>

#include <algorithm>
#include <chrono>
#include <memory>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

namespace pn::internal {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	template <size_t N>
	struct ChangeResult
	{
		char Data[N];
	};

	template <size_t N, size_t K>
	constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
	{
		ChangeResult<N> result = {};

		size_t srcIndex = 0;
		size_t dstIndex = 0;

		while (srcIndex < N) {
			size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex]) {
				matchIndex++;
			}

			if (matchIndex == K - 1) {
				srcIndex += matchIndex;
			}

			result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}

		return result;
	}

	}

	namespace pn {

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "profile_data.json")
		{
			std::lock_guard lock(m_mutex);

			if (m_currentSession) {
				if (Log::GetCoreLogger()) {
					PN_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_currentSession->Name);
				}
				InternalEndSession();
			}

			m_outputStream.open(filepath);
			if (m_outputStream.is_open()) {
				m_currentSession.reset(new pn::internal::InstrumentationSession({name}));
				WriteHeader();
			} else {
				if (Log::GetCoreLogger()) {
					PN_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_mutex);
			InternalEndSession();
		}

		void WriteProfile(const pn::internal::ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_mutex);
			if (m_currentSession) {
				m_outputStream << json.str();
				m_outputStream.flush();
			}
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor()
			: m_currentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			EndSession();
		}

		void WriteHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_outputStream.flush();
		}

		void WriteFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		// Note: you must already own lock on m_mutex before
		// calling InternalEndSession()
		void InternalEndSession()
		{
			if (m_currentSession) {
				WriteFooter();
				m_outputStream.close();
				m_currentSession.reset(nullptr);
			}
		}

	private:
		std::mutex m_mutex;
		std::unique_ptr<pn::internal::InstrumentationSession> m_currentSession;
		std::ofstream m_outputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_name(name), m_isStopped(false)
		{
			m_startTimePoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_isStopped) {
				Stop();
			}
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = pn::internal::FloatingPointMicroseconds{ m_startTimePoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_isStopped = true;
		}

	private:
		const char* m_name;
		std::chrono::time_point<std::chrono::steady_clock> m_startTimePoint;
		bool m_isStopped;
	};

}

#if defined(PN_PROFILE)
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define PN_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define PN_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define PN_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define PN_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define PN_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define PN_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define PN_FUNC_SIG __func__
	#else
		#define PN_FUNC_SIG "PN_FUNC_SIG unknown!"
	#endif

	#define PN_PROFILE_BEGIN_SESSION(name, filepath) ::pn::Instrumentor::Get().BeginSession(name, filepath)
	#define PN_PROFILE_END_SESSION() ::pn::Instrumentor::Get().EndSession()
	#define PN_PROFILE_SCOPE_LINE(name, line) ::pn::InstrumentationTimer timer##line(name)
	#define PN_PROFILE_SCOPE(name) PN_PROFILE_SCOPE_LINE(name, __LINE__)
	#define PN_PROFILE_FUNCTION() PN_PROFILE_SCOPE(PN_FUNC_SIG)
#else
	#define PN_PROFILE_BEGIN_SESSION(name, filepath)
	#define PN_PROFILE_END_SESSION()
	#define PN_PROFILE_SCOPE(name)
	#define PN_PROFILE_FUNCTION()
#endif