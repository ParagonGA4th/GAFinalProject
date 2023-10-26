#include "EditorAction.h"
#include "EditorManager.h"
#include "ProcessManager.h"

Pg::Editor::Core::EditorAction::EditorAction()
	:_hWnd(),
	_screenWidth(1920), _screenHeight(1080),
	_appName(L"ParagonEngine")
{
	_processManager = std::make_unique<Pg::Editor::Manager::ProcessManager>();
	_editorManager = std::make_unique<Pg::Editor::Manager::EditorManager>();
}

Pg::Editor::Core::EditorAction::~EditorAction()
{

}

void Pg::Editor::Core::EditorAction::Initialize()
{
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);

	_processManager->Initialize(static_cast<void*>(_hWnd), _screenWidth, _screenHeight);
	_editorManager->Initialize(_hWnd);
}

void Pg::Editor::Core::EditorAction::Loop()
{
	while (true)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT) break;

			DispatchMessage(&_msg);
			TranslateMessage(&_msg);
			_processManager->ProcessHandler(_msg);
			_editorManager->UIHandler(_msg);
		}
		else
		{
			_processManager->Update();
			_editorManager->Update();
			_editorManager->LastUpdate();
			_processManager->LastUpdate();
		}
	}
}

void Pg::Editor::Core::EditorAction::Finalize()
{
	_editorManager->Finalize();
	_processManager->Finalize();
}

ATOM Pg::Editor::Core::EditorAction::WindowRegisterClass(HINSTANCE hInstance)
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

BOOL Pg::Editor::Core::EditorAction::CreateWindows(HINSTANCE hInstance)
{
	_hWnd = CreateWindowW(_appName, _appName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd) return FALSE;

	RECT rect;

	GetClientRect(_hWnd, &rect);

	_screenWidth = rect.right - rect.left;
	_screenHeight = rect.bottom - rect.top;

	ShowWindow(_hWnd, SW_SHOWNORMAL);
	UpdateWindow(_hWnd);

	return TRUE;
}

LRESULT CALLBACK Pg::Editor::Core::EditorAction::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
