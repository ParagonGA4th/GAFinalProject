#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

#include <string>

namespace Pg::UI::Widget
{
	class DragFloat : public IWidget
	{
	public:
		DragFloat(std::string label, float* input);
		virtual void Update() override;

	private:
		std::string _label;
		float* _inputFloat;
	};
}


