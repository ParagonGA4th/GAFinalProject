#pragma once
#include "IWidget.h"
#include <string>

namespace Pg::UI::Widget
{
	class Text : public Pg::UI::IWidget
	{
	public:
		Text(std::string text);
		virtual void Update() override;

	protected:
		std::string _context;
	};
}

