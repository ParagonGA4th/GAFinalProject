#include "ProcessManager.h"
#include "ExternalLib.h"

Pg::Editor::Manager::ProcessManager::ProcessManager()
	:_isCoreInitailized(false), _editorOnOff(false)
{
	// core
	_coreMain = std::make_unique<Pg::Core::ProcessMain>();

	// input
	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
	_input = &tInputSystem;

	auto& tEditorHelper = singleton<Pg::Editor::Helper::EditorHelper> ();
	_edHepler = &tEditorHelper; 
}

Pg::Editor::Manager::ProcessManager::~ProcessManager()
{

}

void Pg::Editor::Manager::ProcessManager::Initialize(void* hWnd, float screenWidth, float screenHeight)
{
	_coreMain->Initialize(hWnd, screenWidth, screenHeight);
	_isCoreInitailized = true;

	_edHepler->SetDevice(_coreMain->GetGraphicsDevice());
	_edHepler->SetDeviceContext(_coreMain->GetGraphicsDeviceContext());

	_edHepler->SetSceneTexture(_coreMain->GetEditorAdapter()->GetEditorCameraViewSRV());
}

void Pg::Editor::Manager::ProcessManager::Update()
{
	_coreMain->Update();
	_coreMain->BeginRender();
	_coreMain->Render();

	_edHepler->SetEditorOnOff(_input->GetKeyDown(API::Input::eKeyCode::EditorOnOff));
}

void Pg::Editor::Manager::ProcessManager::LastUpdate()
{
	_coreMain->EndRender();
}

void Pg::Editor::Manager::ProcessManager::Finalize()
{
	_coreMain->Finalize();
}

void Pg::Editor::Manager::ProcessManager::ProcessHandler(MSG message)
{
	if (_isCoreInitailized) _input->HandleMessage(message);
}

