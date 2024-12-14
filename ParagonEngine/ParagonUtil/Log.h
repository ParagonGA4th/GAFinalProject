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
#include "UtilDLLExporter.h"

/// ���� ���߿� ����� Logger.
/// ������ Ŭ������ ����Լ��� ����ϴ� ���� �ƴ�, ��ũ�θ� Ȱ����,
/// �ٸ� ��� ������ �α׸� ���� �� �ִ�.
/// ��ü������ Singleton�� �޾Ƽ� ������ �ʴ´�.
/// singleton-cpp�� Ȱ���ؼ�, �ν��Ͻ��� �޾Ƽ� ���ȴ�.

namespace Pg::Util::Debug
{
	class Log
	{
	public:
		//�ʱ�ȭ.
		void Initialize();

		//Ŭ����
		void Finalize();

		//Logger�� ���� �ٲٱ� : 0-5�ܰ谡 �ִ�. ������ ���� �� �ɰ��� ������ ĳġ. -1�� ���� ����� �۵��� ���� ��.
		PARAGON_UTIL_DLL void SetLoggerLevel(int level);

		PARAGON_UTIL_DLL std::shared_ptr<spdlog::logger>& GetLogger();

		PARAGON_UTIL_DLL bool IsInitialized() { return _isInitialized; }

	private:
		void CreateConsoleWindow();

		std::shared_ptr<spdlog::logger> _spdLogger;

		CONSOLE_SCREEN_BUFFER_INFO m_ConsoleInfo;
		HANDLE m_stdioHandle = nullptr;
		int m_ConsoleHandleR = 0;
		int m_ConsoleHandleW = 0;
		FILE* m_ConsoleFilePtr = nullptr;

	private:
		//IsInitialized
		bool _isInitialized{ false };
	};
}


//0�ܰ� : ��ȭ
#ifndef PG_TRACE
#define PG_TRACE(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_TRACE(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)
#endif

//1�ܰ�
#ifndef PG_DEBUG
#define PG_DEBUG(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_DEBUG(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)
#endif

//2�ܰ�
#ifndef PG_INFO
#define PG_INFO(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_INFO(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)
#endif

//3�ܰ�
#ifndef PG_WARN
#define PG_WARN(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_WARN(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)
#endif

//4�ܰ�
#ifndef PG_ERROR
#define PG_ERROR(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_ERROR(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)
#endif

//5�ܰ�
#ifndef PG_CRITICAL
#define PG_CRITICAL(...) \
	do { \
		Pg::Util::Debug::Log& tLogger = singleton<Pg::Util::Debug::Log>(); \
		SPDLOG_LOGGER_CRITICAL(tLogger.GetLogger(), __VA_ARGS__); \
	} while (false)
#endif

//</����� �Լ���>