#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class RadioButton : public Pg::UI::IWidget
	{
	public:
		RadioButton(std::string label, std::string& selectRadio);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	protected:
		std::string _btnName;
		std::string& _selectRadio;
	};
}

