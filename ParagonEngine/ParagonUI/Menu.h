#pragma once
#include "IWidget.h"

#include <string>
#include <vector>

namespace Pg::UI::Widget
{
	class Menu : public Pg::UI::IWidget
	{
	public:
		Menu(std::string menuName, std::vector<Pg::UI::IWidget*> widgets);
		virtual void Update() override;

	protected:
		std::string _menuName;
		std::vector<Pg::UI::IWidget*> _widgets;
	};
}

