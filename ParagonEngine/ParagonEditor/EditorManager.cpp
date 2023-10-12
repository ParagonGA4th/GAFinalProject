#include "EditorManager.h"
#include "ExternalLib.h"

Pg::Editor::Manager::EditorManager::EditorManager()
	:_isCoreInitailized(false)
{
	// File (SceneData)
	
	// core
	_coreMain = std::make_unique<Pg::Core::ProcessMain>();
	
	// input
	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
	_input = &tInputSystem;

	// UI
}

Pg::Editor::Manager::EditorManager::~EditorManager()
{

}

void Pg::Editor::Manager::EditorManager::Initialize(void* hWnd, float width, float height)
{
	_coreMain->Initialize(hWnd, width, height);
	_isCoreInitailized = true;

	// ImGui Dx11, Win32 Setting	
	//ImGui_ImplWin32_Init(_hWnd);
	//ImGui_ImplDX11_Init(_coreMain->GetGraphicsDevice(), _coreMain->GetGraphicsDeviceContext());
}

void Pg::Editor::Manager::EditorManager::Update()
{
	_coreMain->Update();
	_coreMain->BeginRender();
	_coreMain->Render();
	_coreMain->EndRender();
}

void Pg::Editor::Manager::EditorManager::Finalize()
{
	_coreMain->Finalize();
}

void Pg::Editor::Manager::EditorManager::ReSizeHandler(float width, float height)
{
	if(_isCoreInitailized) _coreMain->OnWindowResized(width, height);
}

void Pg::Editor::Manager::EditorManager::InputHandler(MSG message)
{
	_input->HandleMessage(message);
}
