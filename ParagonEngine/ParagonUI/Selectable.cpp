#include "Selectable.h"
#include "imgui.h"

Pg::UI::Widget::Selectable::Selectable(std::vector<std::string>& objNameList)
	:_selectList(objNameList), _selectedNumber(0)
{
}

void Pg::UI::Widget::Selectable::Update()
{
	for (int i = 0; i < _selectList.size(); i++)
	{
		if (ImGui::Selectable(_selectList.at(i).c_str(), i == _selectedNumber))
		{
			_selectedNumber = i;
		}
	}
}

int* Pg::UI::Widget::Selectable::GetSelectableNumber()
{
	return &_selectedNumber;
}

