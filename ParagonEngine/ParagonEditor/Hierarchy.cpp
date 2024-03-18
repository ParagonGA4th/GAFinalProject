#include "Hierarchy.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Hierarchy.h"

#include "../ParagonData/Scene.h"

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
}

void Pg::Editor::Window::Hierarchy::Update()
{
	_uiManager->WindowBegin(_winName);
	_uiManager->BeginDisable(_isDisable);

	DataSet();
	_widgetCon->Update();
	if(_isDisable) _uiManager->EndDisable();
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
		if (name.second.second.size() > 0)
		{
			Pg::Data::GameObject* tobj = nullptr;
			for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
			{
				if (obj->GetName() == name.second.first)
				{
					tobj = obj;
					break;
				}
			}

			if (tobj != nullptr && tobj->_transform.GetChildren().size() != name.second.second.size())
			{
				for (auto& obj : _dataContainer->GetCurrentScene()->GetObjectList())
				{
					for (auto& child : name.second.second)
					{
						if (child == obj->GetName())
						{
							tobj->_transform.AddChild(obj);
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
	if (_prevSceneName != sceneName)
	{
		_prevSceneName = sceneName;
		_objNameList.clear();

		int count = 0;
		for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			std::vector<std::string> childObject;
			
			if (i->_transform.GetChildren().size() < 0)
			{
				_objNameList[count++] = std::make_pair(i->GetName(), childObject);
			}
			else
			{
				for (auto& child : i->_transform.GetChildren())
				{
					childObject.emplace_back(child->_object->GetName());
				}
				_objNameList[count++] = std::make_pair(i->GetName(), childObject);
			}
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
			_dataContainer->SetPickObject(i);

			_changeObjectData->Invoke(eEventType::_OBJECTDATA, static_cast<void*>(i));
			break;
		}
	}
}
