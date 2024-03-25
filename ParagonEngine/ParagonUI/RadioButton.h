#pragma once
#include "IWidget.h"
#include <string>

namespace Pg::UI::Widget
{
	class RadioButton : public Pg::UI::IWidget
	{
	public:
		RadioButton(std::string label, std::string& selectRadio);
		virtual void Update() override;


	protected:
		std::string _btnName;
		std::string& _selectRadio;
	};
}

