#pragma once
#include "IWidget.h"
#include <string>
#include <unordered_map>
#include <map>

namespace Pg::UI::Widget
{
	class MenuBar : public Pg::UI::IWidget
	{
	public:
		MenuBar(std::unordered_map<std::string, std::unordered_map<std::string, bool*>> manubar);
		virtual void Update() override;

	protected:
		std::unordered_map<std::string, std::unordered_map<std::string, bool*>> _manubars;
	};
}

