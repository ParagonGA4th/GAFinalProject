#pragma once
#include "IWidget.h"
#include <string>
#include <vector>

namespace Pg::UI::Widget
{
	class Combo : public Pg::UI::IWidget
	{
	public:
		Combo(std::string label, std::vector<std::string> itemList);
		virtual void Update() override;

		static int _itemType;
	protected:
		std::string _label;
		std::vector<std::string> _itemList;
		bool _isStart;
	};
}

