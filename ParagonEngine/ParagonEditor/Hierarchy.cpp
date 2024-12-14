#include "Hierarchy.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Hierarchy.h"

#include "../ParagonData/Scene.h"

#include <algorithm>
#include <singleton-cpp/singleton.h>


Pg::Editor::Window::Hierarchy::Hierarchy()
	:_winName("Hierarchy"), _isShow(true), _prevObjListSize(0)
{
	auto& tUIManager = singleton<Pg::UI::Manager::UIManager>();
	_uiManager = &tUIManager;

	auto& tdataCon = singleton<Pg::Editor::Data::DataContainer>();
	_dataContainer = &tdataCon;

	_widgetCon = std::make_unique<Pg::UI::WidgetContainer>();

	_changeObjectData = std::make_unique<Pg::Editor::Event>();
}

Pg::Editor::Window::Hierarchy::~Hierarchy()
{

}

void Pg::Editor::Window::Hierarchy::Initialize()
{
	auto& selectable = _widgetCon->CreateWidget<Pg::UI::Widget::Hierarchy>(_objNameList);
	_currentObjName = selectable.GetSelectObjectName();
	_isNewObject = selectable.GetBtnClick();
	_isDeleteObject = selectable.GetKeyDeleteInput();
	_isObjectChange = selectable.GetISObjectChildrenChange();

	_changeObjectData->AddEvent(Pg::Editor::eEventType::_REFRESHOBJECT, [&](void* data) { IsObjectChanged(data); });
}

void Pg::Editor::Window::Hierarchy::Update()
{
	DataSet();

	_uiManager->WindowBegin(_winName);
	_uiManager->BeginDisable(_isDisable);

	_widgetCon->Update();

	if (_isDisable) _uiManager->EndDisable();
	_uiManager->WindowEnd();
}

void Pg::Editor::Window::Hierarchy::Finalize()
{

}

void Pg::Editor::Window::Hierarchy::SetShow(bool show)
{
	_isShow = show;
}

bool Pg::Editor::Window::Hierarchy::GetShow()
{
	return _isShow;
}

std::string Pg::Editor::Window::Hierarchy::GetWindowName()
{
	return _winName;
}

void Pg::Editor::Window::Hierarchy::SetDisable(bool disable)
{
	_isDisable = disable;
}

void Pg::Editor::Window::Hierarchy::DataSet()
{
	// datacontainer에 현재 씬에 대한 정보가 없다면 실행하지 않음
	if (_dataContainer->GetCurrentScene() == nullptr) return;

	GetCurrentSceneObjectList();
	GetSelectedObject();

	if (!(*_isObjectChange)) return;

	for (auto& name : _objNameList)
	{
		bool isbreak = false;
		for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			if (isbreak) break;
			if (obj->GetName() != name.second.first) continue;

			for (auto& childName : name.second.second)
			{
				auto it = std::find_if(_dataContainer->GetCurrentScene()->GetObjectList().begin(),
					_dataContainer->GetCurrentScene()->GetObjectList().end(),
					[&](Pg::Data::GameObject* childObj)
					{
						return childObj->GetName() == childName;
					});

				if (it != _dataContainer->GetCurrentScene()->GetObjectList().end())
				{
					obj->_transform.AddChild(*it);
					isbreak = true;
				}
			}
		}
	}

	*_isObjectChange = false;
}



void Pg::Editor::Window::Hierarchy::GetCurrentSceneObjectList()
{
	std::string sceneName = _dataContainer->GetCurrentScene()->GetSceneName();

	// 여러 번 오브젝트 리스트를 가져오는 것을 막기 위해
	if (_prevSceneName != sceneName || (*_isNewObject) || (*_isDeleteObject) || _isRefresh)
	{
		std::vector<Pg::Data::GameObject*> tObjList;

		if (*_isNewObject)
		{
			if (_count == 0)
			{
				auto obj = _dataContainer->GetCurrentScene()->AddObject("New Object");
				tObjList.emplace_back(obj);
				_count++;
			}
			else
			{
				auto obj = _dataContainer->GetCurrentScene()->AddObject("New Object " + std::to_string(_count++));
				tObjList.emplace_back(obj);
			}

			_changeObjectData->Invoke(eEventType::_ADDOBJECT, static_cast<void*>(&tObjList));
			tObjList.clear();
		}

		if (*_isDeleteObject)
		{
			for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
			{
				if (obj->GetName() == *_currentObjName)
				{
					tObjList.emplace_back(obj);
					break;
				}
			}

			_changeObjectData->Invoke(eEventType::_DELETEOBJECT, static_cast<void*>(&tObjList));
			tObjList.clear();
			_dataContainer->GetCurrentScene()->DeleteObject((*_currentObjName));

			if (_count > 0) _count--;
		}

		_prevSceneName = sceneName;
		*_currentObjName = _dataContainer->GetCurrentScene()->GetObjectList().at(0)->GetName();
		_objNameList.clear();

		int count = 0;
		std::vector<std::string> childObject;

		for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			if (!i->_transform.GetChildren().empty())
			{
				for (auto& child : i->_transform.GetChildren())
				{
					childObject.emplace_back(child->_object->GetName());
				}
			}

			auto childObj = std::find(childObject.begin(), childObject.end(), i->GetName());

			if (childObject.size() == 1)
			{
				if (childObject.at(0) != i->GetName())
					_objNameList[count++] = std::make_pair(i->GetName(), childObject);
				else
					childObject.erase(childObj);
			}
			else
			{
				if (childObj == childObject.end())
					_objNameList[count++] = std::make_pair(i->GetName(), childObject);
				else
					childObject.erase(childObj);
			}
		}
	}
}

void Pg::Editor::Window::Hierarchy::GetSelectedObject()
{
	if (_currentObjName->empty())
	{
		_changeObjectData->Invoke(eEventType::_OBJECTDATA,
			static_cast<void*>(_dataContainer->GetCurrentScene()->GetObjectList().at(0)));
	}

	if (_prevObjName == *_currentObjName) return;

	for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
	{
		if (i->GetName() == *_currentObjName)
		{
			_prevObjName = *_currentObjName;
			_dataContainer->SetPickObject(i);

			_changeObjectData->Invoke(eEventType::_OBJECTDATA, static_cast<void*>(i));
		}
	}
}

void Pg::Editor::Window::Hierarchy::IsObjectChanged(void* isChanged)
{
	_isRefresh = *(static_cast<bool*>(isChanged));
	if (_isRefresh) _isRefresh = false;
}
