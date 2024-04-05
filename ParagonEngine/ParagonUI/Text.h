#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class Text : public Pg::UI::IWidget
	{
	public:
		Text(std::string text);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	protected:
		std::string _context;
	};
}

