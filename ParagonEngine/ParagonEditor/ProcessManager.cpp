#include "ProcessManager.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonProcess/EditorAdapter.h"
#include "../ParagonAPI/PgInput.h"

#include "../ParagonUtil/Log.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>


Pg::Editor::Manager::ProcessManager::ProcessManager(float width, float height)
	:_screenWidth(width), _screenHeight(height), _isCoreInit(false)
{
	// core
	_coreMain = std::make_unique<Pg::Core::ProcessMain>();

	// input
	auto& tInputSystem = singleton<Pg::API::Input::PgInput>();
	_input = &tInputSystem;

	// editor helper
	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;
}

Pg::Editor::Manager::ProcessManager::~ProcessManager()
{
}

void Pg::Editor::Manager::ProcessManager::Initialize(void* hWnd)
{
	_coreMain->Initialize(hWnd, _screenWidth, _screenHeight);
	_isCoreInit = true;

	_dataContainer->SetGraphicsData(_coreMain->GetGraphicsDevice(), _coreMain->GetGraphicsDeviceContext());
	_dataContainer->SetSceneTexture(_coreMain->GetEditorAdapter()->GetEditorCameraViewSRV()); //SRV (에디터 카메라 전달)

	std::unique_ptr<Pg::Editor::Event> _editorEvent = std::make_unique<Pg::Editor::Event>();
	_editorEvent->AddEvent(Pg::Editor::eEventType::_EDITORMODE, [&](void* mode) { SetEditorMode(mode); });
}

void Pg::Editor::Manager::ProcessManager::Update()
{
	_coreMain->Update();
	_coreMain->SetOutlinedObject(_dataContainer->GetPickObject());
	_coreMain->BeginRender();
	_coreMain->Render();
	//Picking + Outline Effect. Editor에서 Edit Mode일때만 발동할 것. 그래픽스 리소스를 아끼기 위해.
	//_dataContainer->SetPickObject(_coreMain->PassPickedObject());

	_coreMain->FinalRender();

	if (_input->GetKeyDown(API::Input::eKeyCode::EditorOnOff))
	{
		_dataContainer->SetEditorOnOff(!_dataContainer->GetEditorOnOff());
	}

	if (_dataContainer->GetEditorOnOff())
	{
		if (_dataContainer->GetSceneList().size() > 0)
		{
			if (_dataContainer->GetSceneList().size() > 0)
			{
				_coreMain->GetEditorAdapter()->SetSceneList(_dataContainer->GetSceneList());
				_coreMain->GetEditorAdapter()->SetCurrentScene(_dataContainer->GetCurrentScene());
			}

			//if (_input->GetKeyDown(API::Input::eKeyCode::Save)) _editorEvent->Invoke(eEventType::_SAVEPROJECT);
		}
		_coreMain->GetEditorAdapter()->SetSceneList(_dataContainer->GetSceneList());
		_coreMain->GetEditorAdapter()->SetCurrentScene(_dataContainer->GetCurrentScene());
	}


	//if (_coreMain->GetEditorAdapter()->GetCurrentScene() != nullptr
	//	&& _dataContainer->GetCurrentScene() == nullptr)
	//{
	//	_dataContainer->SetCurrentScene(_coreMain->GetEditorAdapter()->GetCurrentScene());
	//}

	//if (_dataContainer->GetSceneList().size() > 0)
	//{
	//	_coreMain->GetEditorAdapter()->SetSceneList(_dataContainer->GetSceneList());
	//	_coreMain->GetEditorAdapter()->SetCurrentScene(_dataContainer->GetCurrentScene());
	//}

	//if (_input->GetKeyDown(API::Input::eKeyCode::Save)) _editorEvent->Invoke(eEventType::_SAVEPROJECT);
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
	if (_isCoreInit) _input->HandleMessage(message);
}

void Pg::Editor::Manager::ProcessManager::SetEditorMode(void* mode)
{
	Pg::Data::Enums::eEditorMode* eMode = static_cast<Pg::Data::Enums::eEditorMode*>(mode);
	SetEditorMode(*eMode);
}

void Pg::Editor::Manager::ProcessManager::SetEditorMode(Pg::Data::Enums::eEditorMode mode)
{
	_coreMain->GetEditorAdapter()->SetEditorMode(mode);
}

