#include "EditorAction.h"
#include "IEditorManager.h"
#include "EditorManager.h"
#include "ProcessManager.h"
#include "FileSystem.h"
#include "Event.h"

#include "../ParagonData/GameConstantData.h"

#include <shellapi.h>	// ExtractIconW 오류로 임시 설정

#include "../ParagonUtil/Log.h"

HCURSOR Pg::Editor::Core::EditorAction::_cursor = NULL;

Pg::Editor::Core::EditorAction::EditorAction()
	:_hWnd(),
	_screenWidth(Pg::Data::GameConstantData::WIDTH), _screenHeight(Pg::Data::GameConstantData::HEIGHT),
	_appName(L"ParagonEngine")
{
	_editorManagers.emplace_back(new Pg::Editor::Manager::ProcessManager(_screenWidth, _screenHeight));

#if defined(DEBUG) | defined(_DEBUG)
	_editorManagers.emplace_back(new Pg::Editor::Manager::EditorManager());
#endif
	
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
	SetCustomCursor();

	_fileSystem->Initialize();
	for (auto& manager : _editorManagers) { manager->Initialize(_hWnd); }
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
#if defined(DEBUG) | defined(_DEBUG)
			_editorManagers.at(1)->Update();
			_editorManagers.at(1)->LateUpdate();
#endif
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
	wcex.hIcon = ExtractIconW(hInstance, L"../Resources/Icons/TeamLogo.ico", 0);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _appName.c_str();

	return RegisterClassExW(&wcex);
}

BOOL Pg::Editor::Core::EditorAction::CreateWindows(HINSTANCE hInstance)
{
	//_hWnd = CreateWindowW(_appName.c_str(), _appName.c_str(), WS_OVERLAPPEDWINDOW,
	//	CW_USEDEFAULT, CW_USEDEFAULT, _screenWidth, _screenHeight, nullptr, nullptr, hInstance, nullptr);

	//Minimize && Resize 허용하지 않음.
	_hWnd = CreateWindowW(_appName.c_str(), _appName.c_str(), WS_POPUP,
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

void Pg::Editor::Core::EditorAction::SetCustomCursor()
{
	_cursor = LoadCursorFromFileW(L"../Resources/Textures/Sprites/UI/AimPoint.cur");
	assert(_cursor != NULL);
	SetCursor(_cursor);
}

LRESULT CALLBACK Pg::Editor::Core::EditorAction::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SETCURSOR:
			if (LOWORD(lParam) == HTCLIENT)
			{
				SetCursor(_cursor);
				return TRUE;
			}
			break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}



