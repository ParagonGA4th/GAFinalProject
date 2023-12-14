#pragma once
#include "IWidget.h"

#include <string>
#include <vector>

namespace Pg::UI::Widget
{
	class Selectable : public IWidget
	{
	public:
		Selectable(std::vector<std::string>& objNameList);
		virtual void Update() override;
		int* GetSelectableNumber();

	private:
		std::vector<std::string>& _selectList;
		std::string _name;
		int _selectedNumber;
	};
}


