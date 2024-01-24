#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

#include <string>

namespace Pg::UI::Widget
{
	class InputFloat : public IWidget
	{
	public:
		InputFloat(std::string label, float* input);
		virtual void Update() override;

	private:
		std::string _label;
		float* _inputFloat;
	};
}


