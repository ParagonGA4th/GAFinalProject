#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class InputText : public Pg::UI::IWidget
	{
	public:
		InputText(std::string label, std::string* text);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _label;
		std::string* _text;
		char _inputText[256];
	};
}

