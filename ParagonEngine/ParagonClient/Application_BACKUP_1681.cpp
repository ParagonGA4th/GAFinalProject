#include "Application.h"
#include "../ParagonEditor/EditorMain.h"
#include "../ParagonEditor/imgui_impl_win32.h"
#include "../ParagonAPI/PgInput.h"
#include <singleton-cpp/singleton.h>

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonEditor.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonEditor.lib")
#endif // _DEBUG

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Pg::Client::Core::Application::Application()
	: _hWnd(),
	_screenWidth(1920),
	_screenHeight(1080),
	_appName(L"ParagonEngine")
{
}

Pg::Client::Core::Application::~Application()
{
}

void Pg::Client::Core::Application::Initialize(HINSTANCE hInstance)
{
	RegisterClass(hInstance);
	CreateWindows(hInstance);

	RECT rect;
	GetClientRect(_hWnd, &rect);

	_screenWidth = rect.right - rect.left;
	_screenHeight = rect.bottom - rect.top;

	_editor = new Pg::Editor::EditorMain();
	_editor->Initialize(static_cast<void*>(_hWnd), _screenWidth, _screenHeight);
}

void Pg::Client::Core::Application::Run()
{
	//Input 인스턴스 받기.
	auto& tInput = singleton<Pg::API::Input::PgInput>();
	
	MSG _msg;

	while (true)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT) break;

			DispatchMessage(&_msg);
			TranslateMessage(&_msg);

			//Input Update.
			tInput.HandleMessage(_msg);
		}
		else
		{
			_editor->Update();
		}
	}
}

void Pg::Client::Core::Application::Finalize()
{
	_editor->Finalize();
}

ATOM Pg::Client::Core::Application::RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);

	wcex.style = CS_CLASSDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _appName;
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

BOOL Pg::Client::Core::Application::CreateWindows(HINSTANCE hInstance)
{
	_hWnd = CreateWindowW(_appName, _appName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd)
	{
		return FALSE;
	}

	ShowWindow(_hWnd, SW_SHOWNORMAL);
	UpdateWindow(_hWnd);

	return TRUE;
}

LRESULT CALLBACK Pg::Client::Core::Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) return true;

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