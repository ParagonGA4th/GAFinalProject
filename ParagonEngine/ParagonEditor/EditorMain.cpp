#include "EditorMain.h"

EditorMain::EditorMain()
	: _hWnd(), _msg(),
	_screenWidth(1920),
	_screenHeight(1080),
	_className(L"ParagonEngine"),
	_windowName(L"ParagonEngine")
{
	_coreMain = std::make_unique<Pg::Core::CoreMain>();
	_coreMain->Initialize(static_cast<void*>(_hWnd), _screenWidth, _screenHeight);
}

EditorMain::~EditorMain()
{

}

long EditorMain::Initialize(void* hInstance, int cmdShow)
{
	//윈도우 초기화
	RegisterClass((HINSTANCE)hInstance);

	RECT rect;
	GetClientRect(_hWnd, &rect);

	if (!CreateWindows((HINSTANCE)hInstance, cmdShow))
	{
		return S_FALSE;
	}
}

void EditorMain::Update()
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
			_coreMain->Update();
			_coreMain->BeginRender();
			_coreMain->Render();
			_coreMain->EndRender();
		}
	}
}

void EditorMain::Finalize()
{

}

ATOM EditorMain::RegisterClass(HINSTANCE hInstance)
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

BOOL EditorMain::CreateWindows(HINSTANCE hInstance, int cmdShow)
{
	_hWnd = CreateWindowW(_className, _windowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd)
	{
		return FALSE;
	}

	ShowWindow(_hWnd, cmdShow);
	UpdateWindow(_hWnd);

	return TRUE;
}

LRESULT CALLBACK EditorMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
