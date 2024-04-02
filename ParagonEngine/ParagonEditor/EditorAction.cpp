#include "EditorAction.h"
#include "IEditorManager.h"
#include "EditorManager.h"
#include "ProcessManager.h"
#include "FileSystem.h"
#include "Event.h"

Pg::Editor::Core::EditorAction::EditorAction()
	:_hWnd(),
	_screenWidth(1920), _screenHeight(1080),
	_appName(L"ParagonEngine")
{
	_editorManagers.emplace_back(new Pg::Editor::Manager::ProcessManager(_screenWidth, _screenHeight));
	_editorManagers.emplace_back(new Pg::Editor::Manager::EditorManager());

	_fileSystem = std::make_unique<Pg::Editor::System::FileSystem>();

	_editorEvent = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Core::EditorAction::~EditorAction()
{
	_editorManagers.clear();
}

void Pg::Editor::Core::EditorAction::Initialize()
{
	HINSTANCE ins = GetModuleHandle(NULL);
	WindowRegisterClass(ins);
	CreateWindows(ins);

	for (auto& manager : _editorManagers) { manager->Initialize(_hWnd); }
	_fileSystem->Initialize();
}
void Pg::Editor::Core::EditorAction::Loop()
{
	while (true)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (_msg.message == WM_QUIT) break;

			DispatchMessageW(&_msg);
			TranslateMessage(&_msg);

			for (auto& manager : _editorManagers) { manager->ManagerHandler(_msg); }
		}
		else
		{
			_editorManagers.at(0)->Update();
			_editorManagers.at(1)->Update();
			_editorManagers.at(1)->LateUpdate();
			_editorManagers.at(0)->LateUpdate();
		}
	}
}

void Pg::Editor::Core::EditorAction::Finalize()
{
	for (auto& manager : _editorManagers) { manager->Finalize(); }
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
	wcex.lpszClassName = _appName.c_str();
	wcex.hIconSm = NULL;

	return RegisterClassExW(&wcex);
}

BOOL Pg::Editor::Core::EditorAction::CreateWindows(HINSTANCE hInstance)
{
	//_hWnd = CreateWindowW(_appName.c_str(), _appName.c_str(), WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	//Minimize && Resize 허용하지 않음.
	_hWnd = CreateWindowW(_appName.c_str(), _appName.c_str(), WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd) return FALSE;

	//RECT rect;

	//GetClientRect(_hWnd, &rect);

	//_screenWidth = rect.right - rect.left;
	//_screenHeight = rect.bottom - rect.top;

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
			return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}
