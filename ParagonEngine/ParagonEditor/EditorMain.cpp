#include "EditorMain.h"
#include "EditorManager.h"

Pg::Editor::Core::EditorMain::EditorMain()
	:_hWnd(),
	_screenWidth(1920), _screenHeight(1080),
	_appName(L"ParagonEngine")
{
	_editorManager = std::make_unique<Pg::Editor::Manager::EditorManager>();

}

Pg::Editor::Core::EditorMain::~EditorMain()
{

}

void Pg::Editor::Core::EditorMain::Initialize()
{
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);

	_editorManager->Initialize(_hWnd, _screenWidth, _screenHeight);
}

void Pg::Editor::Core::EditorMain::Loop()
{
	while (true)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT) break;

			DispatchMessage(&_msg);
			TranslateMessage(&_msg);
			_editorManager->Handler(_msg);
			_editorManager->InputHandler(_msg);
		}
		else
		{
			_editorManager->Update();  
		}
	}
}

void Pg::Editor::Core::EditorMain::Finalize()
{
	_editorManager->Finalize();
}

ATOM Pg::Editor::Core::EditorMain::WindowRegisterClass(HINSTANCE hInstance)
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

BOOL Pg::Editor::Core::EditorMain::CreateWindows(HINSTANCE hInstance)
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

LRESULT CALLBACK Pg::Editor::Core::EditorMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
