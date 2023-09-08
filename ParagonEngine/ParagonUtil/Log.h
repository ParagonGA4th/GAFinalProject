#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>
#include <windows.h>
#include <string>
#include <iostream>
#include <stdarg.h>

/// 엔진 개발에 사용할 Logger.
/// 실제로 클래스의 멤버함수를 사용하는 것이 아닌, 매크로를 활용해,
/// 다른 경고 수준의 로그를 남길 수 있다.

namespace Pg::Util::Debug
{
	class Log
	{
	public:
		//초기화.
		void Initialize();

		//클린업
		void Finalize();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return _spdLogger; }

		//Logger의 레벨 바꾸기 : 0-5단계가 있다. 높아질 수록 더 심각한 오류만 캐치. -1이 들어가면 디버거 작동을 끄는 것.
		void SetLoggerLevel(int level);
	private:
		void CreateConsoleWindow();

		static std::shared_ptr<spdlog::logger> _spdLogger;

		CONSOLE_SCREEN_BUFFER_INFO m_ConsoleInfo;
		HANDLE m_stdioHandle = nullptr;
		int m_ConsoleHandleR = 0;
		int m_ConsoleHandleW = 0;
		FILE* m_ConsoleFilePtr = nullptr;
	};
}

#define PG_TRACE(...) Pg::Util::Debug::Log::GetLogger()->trace(__VA_ARGS__);		//0단계 : 평화
#define PG_DEBUG(...) Pg::Util::Debug::Log::GetLogger()->debug(__VA_ARGS__);		//1단계
#define PG_INFO(...)  Pg::Util::Debug::Log::GetLogger()->info(__VA_ARGS__);			//2단계
#define PG_WARN(...)  Pg::Util::Debug::Log::GetLogger()->warn(__VA_ARGS__);			//3단계
#define PG_ERROR(...) Pg::Util::Debug::Log::GetLogger()->error(__VA_ARGS__);		//4단계
#define PG_CRITICAL(...) Pg::Util::Debug::Log::GetLogger()->critical(__VA_ARGS__);	//5단계 : 죽음

