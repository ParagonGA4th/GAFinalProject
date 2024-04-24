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
	_prevObjName = selectable.GetSelectObjectName();
	_isNewObject = selectable.GetBtnClick();
	_isDeleteObject = selectable.GetKeyDeleteInput();

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
	if (_dataContainer->GetCurrentScene() == NULL) return;

	GetCurrentSceneObjectList();
	GetSelectedObject();

	for (auto& name : _objNameList)
	{
		if (name.second.second.empty())
		{
			for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
			{
				if (obj->_transform._object->GetName() == name.second.first)
				{
					if (obj->_transform.HasParent())
					{
						obj->_transform.GetParent()->RemoveChild(obj->GetName());
						obj->_transform._parent = nullptr;
					}
				}
			}
		}
		else
		{
			Pg::Data::GameObject* tobj = nullptr;
			for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
			{
				if (obj->GetName() == name.second.first) tobj = obj;
			}

			if (tobj != nullptr)
			{
				for (auto& childName : name.second.second)
				{
					for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
					{
						if (childName == obj->GetName())
						{
							if (tobj->_transform.GetChildren().empty())	tobj->_transform.AddChild(obj);

							auto it = std::find_if(tobj->_transform.GetChildren().begin(), tobj->_transform.GetChildren().end(),
								[&](Pg::Data::Transform* trans)
								{
									return trans->_object->GetName() == childName;
								});

							if (it == tobj->_transform.GetChildren().end()) tobj->_transform.AddChild(obj);
						}
					}
				}
			}
		}
	}
}



void Pg::Editor::Window::Hierarchy::GetCurrentSceneObjectList()
{
	std::string sceneName = _dataContainer->GetCurrentScene()->GetSceneName();

	// 여러 번 오브젝트 리스트를 가져오는 것을 막기 위해
	if (_prevSceneName != sceneName || (* _isNewObject) || (*_isDeleteObject) || _isObjectChange || _isRefresh)
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
				if (obj->GetName() == *_prevObjName)
				{
					tObjList.emplace_back(obj);
					break;
				}
			}
				
			_changeObjectData->Invoke(eEventType::_DELETEOBJECT, static_cast<void*>(&tObjList));
			tObjList.clear();
			_dataContainer->GetCurrentScene()->DeleteObject((*_prevObjName));

			if(_count > 0) _count--;
		}

		_prevSceneName = sceneName;
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

			if(childObj == childObject.end())
				_objNameList[count++] = std::make_pair(i->GetName(), childObject);
		}
	}
}

void Pg::Editor::Window::Hierarchy::GetSelectedObject()
{
	for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
	{
		if (_prevObjName->empty())
		{
			_changeObjectData->Invoke(eEventType::_OBJECTDATA,
				static_cast<void*>(_dataContainer->GetCurrentScene()->GetObjectList().at(0)));
		}

		if (i->GetName() == *_prevObjName)
		{
			_isObjectChange = false;
			_dataContainer->SetPickObject(i);

			_changeObjectData->Invoke(eEventType::_OBJECTDATA, static_cast<void*>(i));
			break;
		}
		else _isObjectChange = true;
	}
}

void Pg::Editor::Window::Hierarchy::IsObjectChanged(void* isChanged)
{
	if (_isRefresh) _isRefresh = false;
	_isRefresh = *(static_cast<bool*>(isChanged));
}
