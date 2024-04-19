#include "Selectable.h"
#include "imgui.h"

Pg::UI::Widget::Selectable::Selectable(std::vector<std::string> objNameList, std::string& selectedObject)
	:_selectList(objNameList), _selectObj(selectedObject)
{
	_prevSelectObj = _selectObj;
}

Pg::UI::Widget::Selectable::Selectable(std::vector<std::string>* objNameList, std::string& selectedObject)
	:_selectPointerList(objNameList), _selectObj(selectedObject)
{
	_prevSelectObj = _selectObj;
}

void Pg::UI::Widget::Selectable::Update()
{
	if (_selectList.empty())
	{
		for (int i = 0; i < (*_selectPointerList).size(); i++)
		{
			if (ImGui::Selectable((*_selectPointerList).at(i).c_str(), (*_selectPointerList).at(i) == _selectObj))
			{
				_prevSelectObj = _selectObj;
				_selectObj = (*_selectPointerList).at(i);
			}
		}
	}
	else
	{
		for (int i = 0; i < _selectList.size(); i++)
		{
			if (ImGui::Selectable(_selectList.at(i).c_str(), _selectList.at(i) == _selectObj))
			{
				_prevSelectObj = _selectObj;
				_selectObj = _selectList.at(i);
			}
		}
	}

	if (ImGui::IsMouseDoubleClicked(0) && _prevSelectObj.compare(_selectObj) == 0)
	{
		_isDoubleClicked = true;
	}
	else _isDoubleClicked = false;
}

bool* Pg::UI::Widget::Selectable::GetSelectedObjectDoubleClicked()
{
	return &_isDoubleClicked;
}

std::string Pg::UI::Widget::Selectable::GetWidgetLabel()
{
	return "Selectable";
}
