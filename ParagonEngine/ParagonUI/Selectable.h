#pragma once
#include "IWidget.h"
#include <string>
#include <vector>

namespace Pg::UI::Widget
{
	class Selectable : public IWidget
	{
	public:
		Selectable(std::vector<std::string> objNameList);
		virtual void Update() override;

	private:
		std::vector<std::string> _selectList;
		int _selectedNumber;
	};
}


