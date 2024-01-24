#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

#include <string>

namespace Pg::UI::Widget
{
	class InputFloat3 : public IWidget
	{
	public:
		InputFloat3(std::string label, Pg::Math::PGFLOAT3* input);
		virtual void Update() override;
	
	private:
		std::string _label;
		float _inputFloat[3];
		Pg::Math::PGFLOAT3* _inputPGFloat;	
	};
}


