#include "Hierarchy.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Selectable.h"

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
	auto& selectable = _widgetCon->CreateWidget<Pg::UI::Widget::Selectable>(_objNames);
	_selectedNumber = selectable.GetSelectableNumber();
}

void Pg::Editor::Window::Hierarchy::Update()
{
	_uiManager->WindowBegin(_winName);
	DataSet();
	_widgetCon->Update();
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

void Pg::Editor::Window::Hierarchy::DataSet()
{
	 // datacontainer에 현재 씬에 대한 정보가 없다면 실행하지 않음
	if (_dataContainer->GetCurrentScene() == NULL) return;

	// 여러 번 오브젝트 리스트를 가져오는 것을 막기 위해
	if (_prevObjListSize != _dataContainer->GetCurrentScene()->GetObjectList().size())
	{
		_prevObjListSize = _dataContainer->GetCurrentScene()->GetObjectList().size();
		_objNames.clear();

		for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			_objNames.emplace_back(i->GetName());
		}
	}

	// picking된 오브젝트가 있다면 Hierarchy 창에 선택되어야 함
	if (_dataContainer->GetPickObject() != nullptr)
	{
		_prevObjName = _dataContainer->GetPickObject()->GetName();
		_pickedObjName = _dataContainer->GetPickObject()->GetName();
	}


	if (_prevObjName != _objNames.at(*_selectedNumber))
	{
		if (_prevObjName.empty() || _dataContainer->GetPickObject() == nullptr)
		{
			_prevObjName = _objNames.at(*_selectedNumber);
		}

		for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			if (i->GetName() == _prevObjName)
			{
				int count = 0;
				for (auto name : _objNames)
				{
					if (name == i->GetName()) *_selectedNumber = count;

					count++;
				}

				if (_dataContainer->GetPickObject() == nullptr ||
					_dataContainer->GetPickObject()->GetName() != i->GetName()) _dataContainer->SetPickObject(i);

				_changeObjectData->Invoke(eEventType::_OBJECTDATA, static_cast<void*>(i));
				break;
			}
		}
	}
}
