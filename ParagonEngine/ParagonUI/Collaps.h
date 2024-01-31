#pragma once
#include "IWidget.h"
#include <vector>
#include <string>

namespace Pg::UI::Widget::Layout
{
	class Collaps : public Pg::UI::IWidget
	{
	public:
		Collaps(std::string LayoutName, std::vector<Pg::UI::IWidget*> widgets, bool* isShow = nullptr);
		virtual void Update() override;

	private:
		std::string _layoutName;
		std::vector < Pg::UI::IWidget*> _widgets;
		bool* _isShow;
	};
}


