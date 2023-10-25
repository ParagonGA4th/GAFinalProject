#pragma once
#include "Text.h"

namespace Pg::UI::Widget
{
	class InputText : public Pg::UI::IWidget
	{
	public:
		InputText(std::string label, std::string& text);
		virtual void Update() override;

	private:
		std::string _label;
		std::string& _text;
		char _inputText[256];
	};
}

