#include "EditorMain.h"
#include "ImGuiManager.h"
#include "FileManager.h"

Pg::Core::CoreMain* EditorMain::_coreMainStatic = nullptr;	// WndProc 접근을 위한 스태틱 변수
bool EditorMain::_isCoreInitialized; // 코어의 Initialize 이후에 스태틱 변수에 접근하도록 하기 위한 bool 변수

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EditorMain::EditorMain()
	: _hWnd(), _msg(),
	_screenWidth(1920),
	_screenHeight(1080),
	_className(L"ParagonEngine"),
	_windowName(L"ParagonEngine")
{
	_imGuiManager = std::make_unique<ImGuiManager>();

	_isCoreInitialized = false;
	_coreMain = std::make_unique<Pg::Core::CoreMain>();
	_coreMainStatic = _coreMain.get();

	_fileManager = std::make_unique<FileManager>();
	_fileManager->XmlLoad();

	auto& tInputSystem = singleton<Pg::Core::Input::InputSystem>();
	_inputSystem = &tInputSystem;
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

	_coreMain->Initialize(static_cast<void*>(_hWnd), _screenWidth, _screenHeight);
	_isCoreInitialized = true;

	// ImGui Dx11, Win32 Setting	
	ImGui_ImplDX11_Init(_coreMain->GetGraphicsDevice(), _coreMain->GetGraphicsDeviceContext());
	ImGui_ImplWin32_Init(_hWnd);
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
			_inputSystem->HandleMessage(_msg);
		}
		else
		{
			_coreMain->Update();

			_imGuiManager->CreateFrame();

			//_imGuiManager->ShowDemoInspector();
			//_imGuiManager->ShowDemoHierarchy();
			//_imGuiManager->ShowDemoFilter();
			//_imGuiManager->ShowDemoViewPort();

			_coreMain->BeginRender();
			_coreMain->Render();
			_imGuiManager->Render();
			_coreMain->EndRender();
		}
	}
}

void EditorMain::Finalize()
{
	_imGuiManager->Finalize();
	_coreMain->Finalize();
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
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam)) { return true; }

	switch (message)
	{
		case WM_SIZE:
		{
			if (_isCoreInitialized)
			{
				_coreMainStatic->OnWindowResized(LOWORD(lParam), HIWORD(lParam));
			}
			
		}

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
