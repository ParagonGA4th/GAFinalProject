#pragma once
#include "UIEnum.h"
#include <vector>
#include <string>

namespace Pg::UI
{
	class ILayout;
	class Panel
	{
	public:
		Panel(std::string panelName);
		~Panel();

		std::string GetPanelName();
		void Update();

		void CreateLayout(std::string layoutName, Pg::UI::Enum::eLayoutType layoutType = Enum::eLayoutType::Collapsing);
		void SwitchingLayout(std::string layoutName);

		Pg::UI::ILayout* GetCurrentLayout();

	private:
		std::string _panelName;
		std::vector<Pg::UI::ILayout*> _layouts;
		ILayout* _currentLayout;
	};
}


