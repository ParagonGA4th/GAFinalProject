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
		std::string temp;
		if (ImGui::Selectable(_selectList.at(i).c_str(), i == _selectedNumber))
		{
			_selectedNumber = i;
		}

		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			int n_next = i + (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			if (n_next >= 0 && n_next < _selectList.size())
			{
				temp = _selectList.at(n_next);
				_selectList.at(n_next) = _selectList.at(i);
				_selectList.at(i) = temp;
				ImGui::ResetMouseDragDelta();
			}
		}
	}
}

int* Pg::UI::Widget::Selectable::GetSelectableNumber()
{
	return &_selectedNumber;
}

