#include "CoreMain.h"
#include "../ParagonGameEngine/EngineMain.h"
#include "../ParagonGraphics/GraphicsMain.h"
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonGraphics/GraphicsDLLExporter.h"


namespace Pg::Core
{
	CoreMain::CoreMain() :
		_hWnd(), _msg(),
		_screenwidth(1920),
		_screenheight(1080),
		_className(L"ParagonEngine"),
		_windowName(L"ParagonEngine")
	{
		_engine = std::make_unique<Pg::Engine::EngineMain>();
		_graphics = std::make_unique<Pg::Graphics::GraphicsMain>();
		_logger = std::make_unique<Pg::Util::Debug::Log>();

		_inputSystem = std::make_unique <Pg::Engine::InputSystem>();
	}


	CoreMain::~CoreMain()
	{

	}

	long CoreMain::Initialize(void* hInstance, int cmdShow)
	{

		//윈도우 초기화
		RegisterClass((HINSTANCE)hInstance);

		RECT rect;
		GetClientRect(_hWnd, &rect);

		if (!CreateWindows((HINSTANCE)hInstance, cmdShow))
		{
			return S_FALSE;
		}

		//엔진 초기화
		_engine->Initialize();
		_graphics->Initialize();

		//디버그 초기화
		_logger->Initialize();
		_logger->SetLoggerLevel(0);

		// InputSystem 초기화
		_inputSystem->Initialize(_screenwidth, _screenheight);


		PG_TRACE("Engine Success!!");
		PG_DEBUG("Engine Success!!");
		PG_INFO("Engine Success!!");
		PG_WARN("Engine Success!!");

		return S_OK;
	}

	void CoreMain::Update()
	{
		while (true)
		{
			if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
			{
				if (_msg.message == WM_QUIT)
				{
					break;
				}

				DispatchMessage(&_msg);
			}
			else
			{
				//여기다가 시스템 싹 다 업데이트!!
				_engine->Update();
				_graphics->Update();			}
				//_inputSystem->Update();
		}
	}

	void CoreMain::Finalize()
	{

	}

	ATOM CoreMain::RegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEXW wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = _className;
		wcex.hIconSm = NULL;

		return RegisterClassExW(&wcex);
	}

	BOOL CoreMain::CreateWindows(HINSTANCE hInstance, int cmdShow)
	{
		_hWnd = CreateWindowW(_className, _windowName, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, _screenwidth, _screenheight, nullptr, nullptr, hInstance, nullptr);

		if (!_hWnd)
		{
			return FALSE;
		}

		ShowWindow(_hWnd, cmdShow);
		UpdateWindow(_hWnd);

		return TRUE;
	}

	LRESULT CALLBACK CoreMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_LBUTTONDOWN:
			return 0;

		case WM_MBUTTONDOWN:
			return 0;

		case WM_RBUTTONDOWN:
			return 0;

		case WM_LBUTTONUP:

		case WM_MBUTTONUP:

		case WM_RBUTTONUP:
			return 0;

		case WM_MOUSEMOVE:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
}