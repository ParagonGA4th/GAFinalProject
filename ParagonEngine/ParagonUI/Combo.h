#pragma once
#include "IWidget.h"
#include <string>
#include <vector>

namespace Pg::UI::Widget
{
	class Combo : public Pg::UI::IWidget
	{
	public:
		Combo(std::string label, std::vector<std::string> itemList, int index = 0);
		virtual void Update() override;

		int* GetSelectedIndex();
	protected:
		std::string _label;
		std::vector<std::string> _itemList;
		int _selectedIndex;
	};
}

