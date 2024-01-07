#pragma once
#include "IWidget.h"
#include <string>
#include <map>

namespace Pg::UI::Widget
{
	class MenuBar : public Pg::UI::IWidget
	{
	public:
		MenuBar(std::map<std::string, std::map<std::string, bool*>> manubar);
		virtual void Update() override;

	protected:
		std::map<std::string, std::map<std::string, bool*>> _manubars;
	};
}

