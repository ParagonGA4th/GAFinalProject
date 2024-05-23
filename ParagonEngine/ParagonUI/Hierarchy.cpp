#include "Hierarchy.h"
#include "imgui.h"

#include <algorithm>

Pg::UI::Widget::Hierarchy::Hierarchy(std::map<int, std::pair<std::string, std::vector<std::string>>>& objNameList)
	:_objNameList(objNameList), _mode(1), 
	_isNodeOpen(false), _isNodeSelected(false),
	_isAddObject(false), _isDeleteObject(false), _isObjectChildrenChange(false)
{
}

void Pg::UI::Widget::Hierarchy::Update()
{
	_isDeleteObject = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete));

	if (ImGui::Button("+", ImVec2(30, 30))) { _isAddObject = true; }
	else _isAddObject = false;
	
	ImGui::SameLine();
	
	if (ImGui::RadioButton("Group", _mode == GROUP)) { _mode = GROUP; } ImGui::SameLine();
	if (ImGui::RadioButton("Move", _mode == MOVE)) { _mode = MOVE; }

	if (selectObj.empty()) selectObj = _objNameList.at(0).first;

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
						selectObjParent = obj.second.first;
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
					if (selectObj == _objNameList[payload_n].first)
					{
						std::swap(_objNameList[payload_n], _objNameList[obj.first]);
					}
					else
					{
						auto& it = _objNameList[payload_n];

						if (it.first == selectObjParent)
						{
							for (auto& child : it.second)
							{
								if (child == selectObj)
								{
									std::vector<std::string> childObject;
									_objNameList[_objNameList.size()] = std::make_pair(child, childObject);

									auto vec = std::find(it.second.begin(), it.second.end(), child);
									it.second.erase(vec);

									std::swap(_objNameList[obj.first], _objNameList[_objNameList.size()]);
								}
							}
						}
					}
				}
				else
				{
					obj.second.second.emplace_back(_objNameList[payload_n].first);
					_objNameList.erase(payload_n);
					selectObj = obj.second.second.at(obj.second.second.size() - 1);
					_isObjectChildrenChange = true;
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

bool* Pg::UI::Widget::Hierarchy::GetBtnClick()
{
	return &_isAddObject;
}

bool* Pg::UI::Widget::Hierarchy::GetKeyDeleteInput()
{
	return &_isDeleteObject;
}

bool* Pg::UI::Widget::Hierarchy::GetISObjectChildrenChange()
{
	return &_isObjectChildrenChange;
}

std::string Pg::UI::Widget::Hierarchy::GetWidgetLabel()
{
	return "Hierarchy";
}


