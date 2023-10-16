#include "EditorManager.h"
#include "ExternalLib.h"
#include "UIManager.h"
#include "../ParagonAPI/PgInput.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib,"..\\Builds\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

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
	//_UIManager = std::make_unique<Pg::Editor::Manager::UIManager>();
}

Pg::Editor::Manager::EditorManager::~EditorManager()
{

}

void Pg::Editor::Manager::EditorManager::Initialize(HWND hWnd, float width, float height)
{
	_coreMain->Initialize(static_cast<void*>(hWnd), width, height);

	//_UIManager->Initialize(static_cast<void*>(hWnd), _coreMain->GetGraphicsDevice(), _coreMain->GetGraphicsDeviceContext());
	_isCoreInitailized = true;
}

void Pg::Editor::Manager::EditorManager::Update()
{
	_coreMain->Update();
	//_UIManager->Update();

	_coreMain->BeginRender();
	_coreMain->Render();
	//_UIManager->Render();
	_coreMain->EndRender();
}

void Pg::Editor::Manager::EditorManager::Finalize()
{
	_UIManager->Finalize();
	_coreMain->Finalize();
}

//void Pg::Editor::Manager::EditorManager::ReSizeHandler(float width, float height)
//{
//	//if(_isCoreInitailized) _coreMain->OnWindowResized(width, height);
//}


void Pg::Editor::Manager::EditorManager::InputHandler(MSG message)
{
	_input->HandleMessage(message);
}

void Pg::Editor::Manager::EditorManager::Handler(MSG message)
{
	if(_isCoreInitailized) _UIManager->Handler(message.hwnd, message.message, message.wParam, message.lParam);
}
