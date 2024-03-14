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
	//_selectedNumber = selectable.GetSelectIndex();
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
	GetPickedObject();

	//if (_prevObjName != _objNameList.at(*_selectedNumber))
	//{
	//	_prevObjName = _objNameList.at(*_selectedNumber);

	//	for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
	//	{
	//		if (i->GetName() == _prevObjName)
	//		{
	//			if (_dataContainer->GetPickObject() == nullptr || _dataContainer->GetPickObject()->GetName() != i->GetName()) 
	//				_dataContainer->SetPickObject(i);

				_changeObjectData->Invoke(eEventType::_OBJECTDATA, static_cast<void*>(_dataContainer->GetCurrentScene()->GetObjectList().at(0)));
	//			break;
	//		}
	//	}
	//}
}

void Pg::Editor::Window::Hierarchy::GetCurrentSceneObjectList()
{
	int objSize = _dataContainer->GetCurrentScene()->GetObjectList().size();

	// 여러 번 오브젝트 리스트를 가져오는 것을 막기 위해
	if (_prevObjListSize != objSize)
	{
		_prevObjListSize = objSize;
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

			}
		}
	}
}

void Pg::Editor::Window::Hierarchy::GetPickedObject()
{
	// picking된 오브젝트가 있다면 Hierarchy 창에 선택되어야 함
	if (_dataContainer->GetPickObject() != nullptr) 
		_pickingObjName = _dataContainer->GetPickObject()->GetName();

	if (!_pickingObjName.empty())
	{
		for (auto& objName : _objNameList)
		{
			if (objName.second.first == _pickingObjName)
			{
				break;
			}
		}
	}
}
