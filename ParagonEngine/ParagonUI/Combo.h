#pragma once
#include "IWidget.h"
#include <vector>

namespace Pg::UI::Widget
{
	class Combo : public Pg::UI::IWidget
	{
	public:
		Combo(std::string label, std::vector<std::string> itemList, int& index);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	protected:
		std::string _label;
		std::vector<std::string> _itemList;
		int& _selectedIndex;
	};
}

