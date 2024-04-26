#pragma once
#include "IWidget.h"
#include <vector>

namespace Pg::UI::Widget
{
	class Popup : public IWidget
	{
	public:
		Popup(std::string popupName, std::vector<IWidget*> widgets);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _popupName;
		std::vector<IWidget*> _widgets;
	};
}


