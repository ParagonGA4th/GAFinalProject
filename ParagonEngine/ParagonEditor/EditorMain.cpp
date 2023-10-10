#include "EditorMain.h"
#include "ImGuiManager.h"
#include "FileManager.h"
#include "../ParagonUtil/Log.h"

#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>

Pg::Core::ProcessMain* Pg::Editor::EditorMain::_coreMainStatic = nullptr;	// WndProc 접근을 위한 스태틱 변수
bool Pg::Editor::EditorMain::_isCoreInitialized; // 코어의 Initialize 이후에 스태틱 변수에 접근하도록 하기 위한 bool 변수

Pg::Editor::EditorMain::EditorMain()
{
	_isCoreInitialized = false;
	_coreMain = std::make_unique<Pg::Core::ProcessMain>();
	_coreMainStatic = _coreMain.get();

	_fileManager = std::make_unique<FileManager>();
	_fileManager->XmlLoad();

	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
	_input = &tInputSystem;

	_imGuiManager = std::make_unique<ImGuiManager>(_fileManager->GetGameObjectData());
}

Pg::Editor::EditorMain::~EditorMain()
{

}

void Pg::Editor::EditorMain::Initialize(void* hWnd, float width, float height)
{
	_coreMain->Initialize(hWnd, width, height);
	_isCoreInitialized = true;


	// ImGui Dx11, Win32 Setting	
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(_coreMain->GetGraphicsDevice(), _coreMain->GetGraphicsDeviceContext());
}

void Pg::Editor::EditorMain::Update()
{
	_coreMain->Update();
	_imGuiManager->CreateFrame();

	_imGuiManager->ShowDemoInspector();
	_imGuiManager->ShowDemoHierarchy();

	_coreMain->BeginRender();
	_coreMain->Render();
	_imGuiManager->Render();
	_coreMain->EndRender();
}

void Pg::Editor::EditorMain::Finalize()
{
	_imGuiManager->Finalize();
	_coreMain->Finalize();
}