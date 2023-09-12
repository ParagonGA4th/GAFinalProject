#pragma once
#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGameEngine.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGameEngine.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonGraphics.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonGraphics.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\ParagonUtil.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\ParagonUtil.lib")
#endif // _DEBUG

#include "IEngine.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonGameEngine/InputSystem.h"
#include "../ParagonGameEngine/TimeSystem.h"

#include <memory>
#include <windows.h>

namespace Pg::Core
{
	class CoreMain
	{
	public:
		CoreMain();
		~CoreMain();

		long Initialize(void* hInstance, int cmdShow);
		void Update();
		void Finalize();

	private:

		//윈도우 관련 함수들
		ATOM RegisterClass(HINSTANCE hInstance);
		BOOL CreateWindows(HINSTANCE hInstance, int cmdShow);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:

		//윈도우 관련 변수들
		HWND _hWnd;
		MSG _msg;
		int _screenwidth;
		int _screenheight;

		const WCHAR* _className;
		const WCHAR* _windowName;

	private:
		
		std::unique_ptr<IEngine> _engine;				//게임 엔진
		std::unique_ptr<IEngine> _graphics;				//그래픽스 엔진
		std::unique_ptr<Pg::Util::Debug::Log> _logger;
		std::unique_ptr<Pg::Engine::Input::InputSystem> _inputSystem;

		Pg::Engine::Time::TimeSystem* _timeSystem;
	};
}

