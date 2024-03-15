#include "Hierarchy.h"
#include "imgui.h"

#include <algorithm>

Pg::UI::Widget::Hierarchy::Hierarchy(std::map<int, std::pair<std::string, std::vector<std::string>>>& objNameList)
	:_objNameList(objNameList), _mode(1), _isNodeOpen(false), _isNodeSelected(false)
{
}

void Pg::UI::Widget::Hierarchy::Update()
{
	if (ImGui::RadioButton("Group", _mode == GROUP)) { _mode = GROUP; } ImGui::SameLine();
	if (ImGui::RadioButton("Move", _mode == MOVE)) { _mode = MOVE; }

	for (auto& obj : _objNameList)
	{
		// Child가 없는 Object
		if (obj.second.second.size() <= 0)
		{
			if (ImGui::Selectable(obj.second.first.c_str(), selectObj == obj.second.first))
			{
				selectObj = obj.second.first;
			}
		}

		// Child가 존재하는 Object
		else
		{
			ImGuiTreeNodeFlags flags = (selectObj == obj.second.first ? ImGuiTreeNodeFlags_Selected : 0);

			if (ImGui::TreeNodeEx(obj.second.first.c_str(), flags))
			{
				_isNodeSelected = ImGui::IsItemClicked();
				if (_isNodeSelected)
				{
					selectObj = obj.second.first;
				}

				for (auto& child : obj.second.second)
				{
					if (ImGui::Selectable(child.c_str(), selectObj == child))
					{
						selectObj = child;
					}
				}
				ImGui::TreePop();
			}
		}

		// Our buttons are both drag sources and drag targets here!
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			// Set payload to carry the index of our item (could be anything)
			ImGui::SetDragDropPayload("Move_Select", &obj.first, sizeof(int));
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Move_Select"))
			{
				IM_ASSERT(payload->DataSize == sizeof(int));
				int payload_n = *(int*)payload->Data;

				if (_mode == MOVE)
				{
					std::swap(_objNameList[payload_n], _objNameList[obj.first]);
				}
				else
				{
					obj.second.second.emplace_back(_objNameList[payload_n].first);
					_objNameList.erase(payload_n);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
}

std::string* Pg::UI::Widget::Hierarchy::GetSelectObjectName()
{
	return &selectObj;
}
