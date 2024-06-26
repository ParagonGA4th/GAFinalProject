#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::UI::Widget
{
	class InputInt : public IWidget
	{
	public:
		InputInt(std::string label, int* input);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _label;
		int* _inputFloat;
	};
}


