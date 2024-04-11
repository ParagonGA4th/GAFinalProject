#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

#include <vector>

namespace Pg::UI::Layout
{
	class ChildWindow : public Pg::UI::IWidget
	{
	public:
		ChildWindow(std::string layoutName, Pg::Math::PGFLOAT2 size, std::vector<Pg::UI::IWidget*> widgets);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _layoutName;
		Pg::Math::PGFLOAT2 _size;
		std::vector<Pg::UI::IWidget*> _widgets;
	};
}


