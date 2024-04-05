#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::UI::Widget
{
	class DragFloat : public IWidget
	{
	public:
		DragFloat(std::string label, float* input);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _label;
		float* _inputFloat;
	};
}


