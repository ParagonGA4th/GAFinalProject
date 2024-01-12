#include "Inspector.h"
#include "InspectorHelper.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_insHelper = std::make_unique<Pg::Editor::Window::InspectorHelper>();
	_changeObjectData = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{	
	_insHelper->Initialize();
	_changeObjectData->AddEvent(Pg::Editor::eEventType::ChangeObjectData, [&](void* data) { SetData(data); });
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	_insHelper->Update();
	_uiManager->WindowEnd();

	//if (_dataContainer->GetSave())
	//{
	//	for (auto vobj : _dataContainer->GetCurrentScene()->GetObjectList())
	//	{
	//		if (vobj->GetName() == "New Object")
	//		{
	//			vobj->SetName(_objName);
	//			vobj->SetActive(_isObjActive);
	//		}
	//	}
	//}
}

void Pg::Editor::Window::Inspector::Finalize()
{

}

void Pg::Editor::Window::Inspector::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Inspector::GetShow()
{
	return _isShow;
}

void Pg::Editor::Window::Inspector::SetData(void* data)
{
	if (data != nullptr)
	{
		_selectGameObject = static_cast<Pg::Data::GameObject*>(data);
		_insHelper->SetData(_selectGameObject);
	}
}
