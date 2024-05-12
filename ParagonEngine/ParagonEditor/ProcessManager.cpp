#include "ProcessManager.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonProcess/ProcessMain.h"
#include "../ParagonProcess/EditorAdapter.h"
#include "../ParagonUtil/InputSystem.h"

#include "../ParagonData/AssetDefines.h"
#include "../ParagonUtil/Log.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>


Pg::Editor::Manager::ProcessManager::ProcessManager(float width, float height)
	:_screenWidth(width), _screenHeight(height), _isCoreInit(false)
{
	// core
	_coreMain = std::make_unique<Pg::Core::ProcessMain>();

	// input
	auto& tInputSystem = singleton<Pg::Util::Input::InputSystem>();
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
	_editorEvent->AddEvent(Pg::Editor::eEventType::_ADDOBJECT, [&](void* objList) { SetAddObject(objList); });
	_editorEvent->AddEvent(Pg::Editor::eEventType::_MODIFIEDOBJECT, [&](void* objList) { SetModifiedObject(objList); });
	_editorEvent->AddEvent(Pg::Editor::eEventType::_DELETEOBJECT, [&](void* objList) { SetDeleteObject(objList); });
	_editorEvent->AddEvent(Pg::Editor::eEventType::_ASSETLIST, [&](void* define) { GetAssetList(define); });
	_editorEvent->AddEvent(Pg::Editor::eEventType::_MOUSEPOSX, [&](void* posX) { SetEditorMousePosX(posX); });
	_editorEvent->AddEvent(Pg::Editor::eEventType::_MOUSEPOSY, [&](void* posY) { SetEditorMousePosY(posY); });

	if (_dataContainer->GetCurrentScene() == nullptr)
		_dataContainer->SetCurrentScene(_coreMain->GetEditorAdapter()->GetCurrentScene());
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

		if (_coreMain->GetEditorAdapter()->GetCurrentScene() != _dataContainer->GetCurrentScene())
			_dataContainer->SetCurrentScene(_coreMain->GetEditorAdapter()->GetCurrentScene());
	}
	else
	{
		_input->SetEditorMouseX(-10.f);
		_input->SetEditorMouseY(-10.f);
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

void Pg::Editor::Manager::ProcessManager::SetAddObject(void* objectList)
{
	_coreMain->GetEditorAdapter()->SetAddObjectList(static_cast<std::vector<Pg::Data::GameObject*>*>(objectList));
}

void Pg::Editor::Manager::ProcessManager::SetModifiedObject(void* objectList)
{
	_coreMain->GetEditorAdapter()->SetModifyObjectList(static_cast<std::vector<Pg::Data::GameObject*>*>(objectList));
}

void Pg::Editor::Manager::ProcessManager::SetDeleteObject(void* objectList)
{
	_coreMain->GetEditorAdapter()->SetDeleteObjectList(static_cast<std::vector<Pg::Data::GameObject*>*>(objectList));
}

void Pg::Editor::Manager::ProcessManager::GetAssetList(void* define)
{
	auto tvec = _coreMain->GetAssetList(*(static_cast<Pg::Data::Enums::eAssetDefine*>(define)));
	_dataContainer->SetAssetList(tvec);
}

void Pg::Editor::Manager::ProcessManager::SetEditorMousePosX(void* x)
{
	if (_coreMain->GetEditorAdapter()->GetEditorMode() == Pg::Data::Enums::eEditorMode::_EDIT || 
		_coreMain->GetEditorAdapter()->GetEditorMode() == Pg::Data::Enums::eEditorMode::_PLAY)
		_input->SetEditorMouseX(*(static_cast<float*>(x)));
	else 
		_input->SetEditorMouseX(-10.f);
}

void Pg::Editor::Manager::ProcessManager::SetEditorMousePosY(void* y)
{
	if (_coreMain->GetEditorAdapter()->GetEditorMode() == Pg::Data::Enums::eEditorMode::_EDIT ||
		_coreMain->GetEditorAdapter()->GetEditorMode() == Pg::Data::Enums::eEditorMode::_PLAY)
		_input->SetEditorMouseY(*(static_cast<float*>(y)));
	else 
		_input->SetEditorMouseY(-10.f);
}
