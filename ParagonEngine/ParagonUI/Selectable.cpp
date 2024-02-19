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

		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the index of our item (could be anything)
			ImGui::SetDragDropPayload("DND_DEMO_CELL", &_selectedNumber, sizeof(int));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
			{
				IM_ASSERT(payload->DataSize == sizeof(int));
				int payload_n = *(const int*)payload->Data;
				//if (mode == Mode_Copy)
				//{
				//	names[n] = names[payload_n];
				//}
				//if (mode == Mode_Move)
				//{
				//	names[n] = names[payload_n];
				//	names[payload_n] = "";
				//}
				//if (mode == Mode_Swap)
				//{
				std::string tmp = std::move(_selectList.at(_selectedNumber));
				_selectList.at(_selectedNumber) = _selectList.at(payload_n);
				_selectList.at(payload_n) = tmp;
				//}
			}
			ImGui::EndDragDropTarget();
		}
	}
}

int* Pg::UI::Widget::Selectable::GetSelectableNumber()
{
	return &_selectedNumber;
}

