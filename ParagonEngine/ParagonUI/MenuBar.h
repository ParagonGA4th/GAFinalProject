#pragma once
#include "IWidget.h"

#include <utility>
#include <map>

namespace Pg::UI::Widget
{
	class MenuBar : public Pg::UI::IWidget
	{
	public:
		MenuBar(std::map<std::string, std::map<std::string, bool*>> menubars);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	protected:
		std::map<std::string, std::map<std::string, bool*>> _menubars;
	};
}

