#include "Inspector.h"
#include "InspectorHelper.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"

#include "../ParagonUtil/Log.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Inspector::Inspector()
	:_winName("Inspector"), _isShow(true)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	_insHelper = std::make_unique<Pg::Editor::Window::InspectorHelper>();
}

Pg::Editor::Window::Inspector::~Inspector()
{

}

void Pg::Editor::Window::Inspector::Initialize()
{
	_insHelper->Initialize();

	std::unique_ptr<Pg::Editor::Event> changeObjectData = std::make_unique<Pg::Editor::Event>();
	changeObjectData->AddEvent(Pg::Editor::eEventType::_OBJECTDATA, [&](void* data) { SetData(data); });
}

void Pg::Editor::Window::Inspector::Update()
{
	_uiManager->WindowBegin(_winName);
	_uiManager->BeginDisable(_isDisable);

	_insHelper->Update();


	AddComponent();
	ModifiedObject();

	PG_TRACE(_flag);

	if (_isDisable) _uiManager->EndDisable();
	_uiManager->WindowEnd();
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

void Pg::Editor::Window::Inspector::AddComponent()
{
	//if (*_isClick)
	//{
	//	if (_selectGameObject != nullptr)
	//	{
	//		_selectGameObject->AddComponent(_componentList.at(*_componentIndex));
	//		_insHelper->SetData(_selectGameObject);

	//		if (!_modifiedObjList.empty())
	//		{
	//			auto tchObj = std::find_if(_modifiedObjList.begin(), _modifiedObjList.end(),
	//				[&](Pg::Data::GameObject* obj)
	//				{
	//					return obj->GetName() == _selectGameObject->GetName();
	//				});

	//			if (tchObj != _modifiedObjList.end()) return;
	//		}

	//		_modifiedObjList.emplace_back(_selectGameObject);
	//	}
	//}
}

void Pg::Editor::Window::Inspector::ModifiedObject()
{
	_flag = _uiManager->IsWindowFocus(_winName);
	if (_flag == -1)
	{
		//if (_modifiedObjList.empty()) return;

		//std::vector<Pg::Data::GameObject*> tobjList(_modifiedObjList.size());
		//std::copy(_modifiedObjList.begin(), _modifiedObjList.end(), tobjList.begin());

		//tobjList.erase(std::remove_if(tobjList.begin(), tobjList.end(),
		//	[&](Pg::Data::GameObject* obj)
		//	{
		//		for (auto& component : obj->GetComponentList())
		//		{
		//			if (component.first.find(typeid(Pg::Data::StaticMeshRenderer).name()) != std::string::npos)
		//			{
		//				Pg::Data::StaticMeshRenderer* smRenderer = _selectGameObject->GetComponent<Pg::Data::StaticMeshRenderer>();
		//				if (smRenderer->_meshName.empty()) return true;
		//			}
		//		}

		//	}), tobjList.end());
		// process로 넘기기 
	}
}

std::string Pg::Editor::Window::Inspector::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Inspector::SetDisable(bool disable)
{
	_isDisable = disable;
}


