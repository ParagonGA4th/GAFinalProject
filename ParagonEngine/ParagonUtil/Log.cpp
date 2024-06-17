#include "Log.h"

#include <io.h>
#include <fcntl.h>
#include <algorithm>

namespace Pg::Util::Debug
{
	void Log::Initialize()
	{
		CreateConsoleWindow();

		//spdlog::set_pattern("%^[%T] %n: %v%$");
		//spdlog::set_pattern("%^[%g] : %v%$");
		spdlog::set_pattern("%^[%s / %! / %#]: %v%$");
		_spdLogger = spdlog::stdout_color_mt("PgLog");
		_spdLogger->set_level(spdlog::level::trace);

		//InitializeµÇ¾ú´Ù.
		_isInitialized = true;
	}

	void Log::Finalize()
	{

	}

	void Log::CreateConsoleWindow()
	{
		BOOL res = AllocConsole();
		std::wstring strW = L"Paragon Debugger";
		SetConsoleTitle(strW.c_str());

		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
		DrawMenuBar(GetConsoleWindow());

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &m_ConsoleInfo);

		m_stdioHandle = GetStdHandle(STD_INPUT_HANDLE);
		m_ConsoleHandleR = _open_osfhandle(reinterpret_cast<intptr_t>(m_stdioHandle), _O_TEXT);
		m_ConsoleFilePtr = _fdopen(m_ConsoleHandleR, "r");
		*stdin = *m_ConsoleFilePtr;
		setvbuf(stdin, NULL, _IONBF, 0);

		m_stdioHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		m_ConsoleHandleW = _open_osfhandle(reinterpret_cast<intptr_t>(m_stdioHandle), _O_TEXT);
		m_ConsoleFilePtr = _fdopen(m_ConsoleHandleW, "w");
		*stdout = *m_ConsoleFilePtr;
		setvbuf(stdout, NULL, _IONBF, 0);

		m_stdioHandle = GetStdHandle(STD_ERROR_HANDLE);
		*stderr = *m_ConsoleFilePtr;
		setvbuf(stderr, NULL, _IONBF, 0);
	}

	void Log::SetLoggerLevel(int level)
	{
		level = std::clamp(level, -1, 5);

		switch (level)
		{
			case -1:
			{
				_spdLogger->set_level(spdlog::level::off);
			}
			break;
			case 0:
			{
				_spdLogger->set_level(spdlog::level::trace);
			}
			break;
			case 1:
			{
				_spdLogger->set_level(spdlog::level::debug);
			}
			break;
			case 2:
			{
				_spdLogger->set_level(spdlog::level::info);
			}
			break;
			case 3:
			{
				_spdLogger->set_level(spdlog::level::warn);
			}
			break;
			case 4:
			{
				_spdLogger->set_level(spdlog::level::err);
			}
			break;
			case 5:
			{
				_spdLogger->set_level(spdlog::level::critical);
			}
			break;
		}
	}

	std::shared_ptr<spdlog::logger>& Log::GetLogger()
	{
		 return _spdLogger;
	}

}