#include "ProcessManager.h"
#include "DataContainer.h"

#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonProcess/EditorAdapter.h"
#include "../ParagonAPI/PgInput.h"

#include <singleton-cpp/singleton.h>


Pg::Editor::Manager::ProcessManager::ProcessManager(float width, float height)
	:_screenWidth(width), _screenHeight(height),
	_isCoreInitailized(false)
{
	// core
	_coreMain = std::make_unique<Pg::Core::ProcessMain>();

	// input
	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
	_input = &tInputSystem;

	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer> ();
	_dataContainer = &tdataCon;
}

Pg::Editor::Manager::ProcessManager::~ProcessManager()
{
}

void Pg::Editor::Manager::ProcessManager::Initialize(void* hWnd)
{
	_coreMain->Initialize(hWnd, _screenWidth, _screenHeight);
	_isCoreInitailized = true;

	_dataContainer->SetGraphicsData(_coreMain->GetGraphicsDevice(), _coreMain->GetGraphicsDeviceContext());
	_dataContainer->SetSceneTexture(_coreMain->GetEditorAdapter()->GetEditorCameraViewSRV());
}

void Pg::Editor::Manager::ProcessManager::Update()
{
	_coreMain->Update();
	_coreMain->BeginRender();
	_coreMain->Render();

	if (_input->GetKeyDown(API::Input::eKeyCode::EditorOnOff))
	{
		_dataContainer->SetEditorOnOff(!_dataContainer->GetEditorOnOff());
		_coreMain->GetEditorAdapter()->SetEditorMode(Pg::Data::Enums::eEditorMode::_EDIT);
	}
}

void Pg::Editor::Manager::ProcessManager::LateUpdate()
{
	_coreMain->EndRender();
}

void Pg::Editor::Manager::ProcessManager::Finalize()
{
	_coreMain->Finalize();
}

void Pg::Editor::Manager::ProcessManager::ManagerHandler(MSG message)
{
	if (_isCoreInitailized) _input->HandleMessage(message);
}

