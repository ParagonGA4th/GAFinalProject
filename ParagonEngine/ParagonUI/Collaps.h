#pragma once
#include "IWidget.h"
#include <vector>

namespace Pg::UI::Layout
{
	class Collaps : public Pg::UI::IWidget
	{
	public:
		Collaps(std::string LayoutName, std::vector<Pg::UI::IWidget*> widgets, bool* isShow = nullptr);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;
	private:
		std::string _layoutName;
		std::vector < Pg::UI::IWidget*> _widgets;
		bool* _isShow;
	};
}


