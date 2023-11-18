#include "EditorAction.h"
#include "EditorManager.h"
#include "ProcessManager.h"
#include "FileManager.h"

// Message 처리를 위해 필요한 define
#define ID_OPEN_PROJECT 1000
#define ID_NEW_PROJECT 1001
#define ID_OPEN_SCENE 1002
#define ID_NEW_SCENE 1003
#define ID_SAVE 1004
#define ID_EXIT 1005

Pg::Editor::Core::EditorAction::EditorAction()
	:_hWnd(),
	_screenWidth(1920), _screenHeight(1080),
	_appName(L"ParagonEngine")
{
	_fileManager = std::make_unique<Pg::Editor::Manager::FileManager>();
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

	_fileManager->Initialize();
	_fileManager->FileOpen();
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
			_editorManager->WindowHandler(_msg);
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

	/// Project를 Load 하기 위한 Menubar 생성 예제 코드
	// 메뉴 핸들 생성
	HMENU hMenu = CreateMenu();
	HMENU hSubMenu = CreatePopupMenu();

	// 메뉴 항목 추가
	AppendMenu(hSubMenu, MF_STRING, ID_OPEN_PROJECT, "Open Project");
	AppendMenu(hSubMenu, MF_STRING, ID_NEW_PROJECT, "New Project");
	AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL); // 구분선 추가
	AppendMenu(hSubMenu, MF_STRING, ID_OPEN_SCENE, "Open Scene");
	AppendMenu(hSubMenu, MF_STRING, ID_NEW_SCENE, "New Scene");
	AppendMenu(hSubMenu, MF_SEPARATOR, 0, NULL); // 구분선 추가
	AppendMenu(hSubMenu, MF_STRING, ID_SAVE, "Save");
	AppendMenu(hSubMenu, MF_STRING, ID_EXIT, "Exit");
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "File");

	// 메뉴 핸들을 윈도우에 연결
	SetMenu(_hWnd, hMenu);

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
	
		case WM_COMMAND:
			// 메뉴 항목 선택 이벤트 처리
			switch (LOWORD(wParam)) 
			{
				case ID_OPEN_PROJECT:
					break;
				
				case ID_NEW_PROJECT:
					break;
				
				case ID_OPEN_SCENE:
					break;
				
				case ID_NEW_SCENE:
					break;
	
				case ID_SAVE:	
					break;		
				
				case ID_EXIT:
					PostMessage(hWnd, WM_CLOSE, 0, 0);
					break;
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
