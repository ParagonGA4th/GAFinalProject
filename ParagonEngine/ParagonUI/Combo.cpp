#include "Combo.h"
#include "imgui.h"

Pg::UI::Widget::Combo::Combo(std::string label, std::vector<std::string> itemList, int index)
	:_label(label), _itemList(itemList), _selectedIndex(index)
{
}

void Pg::UI::Widget::Combo::Update()
{
	std::string itemName = _itemList.at(_selectedIndex).substr(_itemList.at(_selectedIndex).rfind("::") + 2);

	if (ImGui::BeginCombo(_label.c_str(), itemName.c_str()))
	{
		for (size_t i = 0; i < _itemList.size(); i++) 
		{
			const bool isSelected = (_selectedIndex == static_cast<int>(i));
			itemName = _itemList.at(i).substr(_itemList.at(i).rfind("::") + 2);

			if (ImGui::Selectable(itemName.c_str(), isSelected))
			{
				_selectedIndex = static_cast<int>(i);
			}

			if (isSelected) 
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

int* Pg::UI::Widget::Combo::GetSelectedIndex()
{
	return &_selectedIndex;
}
