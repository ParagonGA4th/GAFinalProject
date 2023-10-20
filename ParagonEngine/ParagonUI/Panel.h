#pragma once
#include <vector>

namespace Pg::UI
{
	class Widget;
	class Panel
	{
	public:
		Panel();
		~Panel();

		void AddWidget();
	private:
		std::vector<Pg::UI::Widget*> _widgets;
	};
}


