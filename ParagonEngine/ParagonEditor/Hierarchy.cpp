#include "Hierarchy.h"
#include "DataContainer.h"
#include "Event.h"

#include "../ParagonUI/UIManager.h"
#include "../ParagonUI/WidgetContainer.h"
#include "../ParagonUI/Selectable.h"

#include "../ParagonData/Scene.h"

#include <singleton-cpp/singleton.h>

Pg::Editor::Window::Hierarchy::Hierarchy()
	:_winName("Hierarchy"), _isShow(true)
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
	if (_dataContainer->GetCurrentScene() != NULL)
	{
		_objNames.clear();
		for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
		{
			_objNames.emplace_back(i->GetName());
		}

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

			int count = 0;
			for (auto i : _dataContainer->GetCurrentScene()->GetObjectList())
			{
				if (i->GetName() == _prevObjName)
				{
					*_selectedNumber = count;
					
					if (_dataContainer->GetPickObject() == nullptr ||
						_dataContainer->GetPickObject()->GetName() != i->GetName()) _dataContainer->SetPickObject(i);

					_changeObjectData->Invoke(eEventType::_OBJECTDATA, static_cast<void*>(i));
					break;
				}
				count++;
			}
		}
	}
}
