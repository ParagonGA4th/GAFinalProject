#include "EditorMain.h"
#include "EditorManager.h"

Pg::Editor::Manager::EditorManager* Pg::Editor::EditorMain::_editorManager = nullptr;	// WndProc 접근을 위한 스태틱 변수

Pg::Editor::EditorMain::EditorMain()
	:_hWnd(),
	_screenWidth(1920), _screenHeight(1080),
	_appName(L"ParagonEngine")
{
	_editorManager = new Pg::Editor::Manager::EditorManager();
}

Pg::Editor::EditorMain::~EditorMain()
{

}

void Pg::Editor::EditorMain::Initialize()
{
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);

	_editorManager->Initialize(static_cast<void*>(_hWnd), _screenWidth, _screenHeight);
}

void Pg::Editor::EditorMain::Run()
{
	MSG _msg;

	while (true)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT) break;

			DispatchMessage(&_msg);
			TranslateMessage(&_msg);
			_editorManager->InputHandler(_msg);
		}
		else
		{
			_editorManager->Update();
		}
	}
}

void Pg::Editor::EditorMain::Finalize()
{
	_editorManager->Finalize();
}

ATOM Pg::Editor::EditorMain::WindowRegisterClass(HINSTANCE hInstance)
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

BOOL Pg::Editor::EditorMain::CreateWindows(HINSTANCE hInstance)
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

LRESULT CALLBACK Pg::Editor::EditorMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		 _editorManager->ReSizeHandler(LOWORD(lParam), HIWORD(lParam));
		break;

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
