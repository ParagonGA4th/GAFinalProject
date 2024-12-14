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
	_editorEvent->AddEvent(Pg::Editor::eEventType::ONPROJECTLOAD, [&]() { OnParagonProjectLoad(); });

	_coreMain->OnParagonProjectLoad(_dataContainer->GetSceneList());
	_coreMain->GetEditorAdapter()->SetSceneList(_dataContainer->GetSceneList());
	_coreMain->GetEditorAdapter()->SetCurrentScene(_dataContainer->GetCurrentScene());
#if defined(DEBUG) | defined(_DEBUG)
#else
	SetEditorMode(Pg::Data::Enums::eEditorMode::_GAME);
#endif
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

#if defined(DEBUG) | defined(_DEBUG)
	if (_input->GetKeyDown(API::Input::eKeyCode::EditorOnOff))
	{
		_dataContainer->SetEditorOnOff(!_dataContainer->GetEditorOnOff());
	}

	if (_dataContainer->GetEditorOnOff())
	{
		if (_coreMain->GetEditorAdapter()->GetSceneList().size() != _dataContainer->GetSceneList().size())
		{
			if (_coreMain->GetEditorAdapter()->GetSceneList().size() < _dataContainer->GetSceneList().size())
				_coreMain->GetEditorAdapter()->SetSceneList(_dataContainer->GetSceneList());
			else
				_dataContainer->SetSceneList(_coreMain->GetEditorAdapter()->GetSceneList());
		}

		if (_coreMain->GetEditorAdapter()->GetCurrentScene()->GetSceneName() != _dataContainer->GetCurrentScene()->GetSceneName())
		{
			if (_dataContainer->GetIsSceneChange())
			{
				_coreMain->GetEditorAdapter()->SetCurrentScene(_dataContainer->GetCurrentScene());
				_dataContainer->SetIsSceneChange(false);
			}
			else
				_dataContainer->SetCurrentScene(_coreMain->GetEditorAdapter()->GetCurrentScene());
		}
	}
	else
	{
		_input->SetEditorMouseX(-10.f);
		_input->SetEditorMouseY(-10.f);
	}

#else
	_input->SetEditorMouseX(-10.f);
	_input->SetEditorMouseY(-10.f);
#endif
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

void Pg::Editor::Manager::ProcessManager::OnParagonProjectLoad()
{
	_coreMain->OnParagonProjectLoad(_dataContainer->GetSceneList());
}
