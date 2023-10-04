#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>
#include <windows.h>
#include <string>
#include <iostream>
#include <stdarg.h>
#include <singleton-cpp/singleton.h>

/// 엔진 개발에 사용할 Logger.
/// 실제로 클래스의 멤버함수를 사용하는 것이 아닌, 매크로를 활용해,
/// 다른 경고 수준의 로그를 남길 수 있다.
/// 자체적으로 Singleton을 받아서 쓰이지 않는다.
/// singleton-cpp를 활용해서, 인스턴스를 받아서 사용된다.

namespace Pg::Util::Debug
{
	class Log
	{
	public:
		//초기화.
		void Initialize();

		//클린업
		void Finalize();

		//Logger의 레벨 바꾸기 : 0-5단계가 있다. 높아질 수록 더 심각한 오류만 캐치. -1이 들어가면 디버거 작동을 끄는 것.
		void SetLoggerLevel(int level);

		std::shared_ptr<spdlog::logger>& GetLogger();
	private:
		void CreateConsoleWindow();

		std::shared_ptr<spdlog::logger> _spdLogger;

		CONSOLE_SCREEN_BUFFER_INFO m_ConsoleInfo;
		HANDLE m_stdioHandle = nullptr;
		int m_ConsoleHandleR = 0;
		int m_ConsoleHandleW = 0;
		FILE* m_ConsoleFilePtr = nullptr;
	};
}

//0단계 : 평화
#define PG_TRACE(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_TRACE(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)

//1단계
#define PG_DEBUG(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_DEBUG(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)

//2단계
#define PG_INFO(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_INFO(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)

//3단계
#define PG_WARN(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_WARN(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)

//4단계
#define PG_ERROR(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_ERROR(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)

//5단계
#define PG_CRITICAL(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_CRITICAL(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)

//</디버깅 함수들>