#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::UI::Widget
{
	class InputFloat3 : public IWidget
	{
	public:
		InputFloat3(std::string label, Pg::Math::PGFLOAT3* input);
		InputFloat3(std::string label, Pg::Math::PGFLOAT4* input);
		InputFloat3(std::string label, Pg::Math::PGQuaternion* input);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;
	
	private:
		std::string _label;

		float _inputFloat[3];
		
		Pg::Math::PGFLOAT3* _inputPGFloat3;	
		Pg::Math::PGFLOAT4* _inputPGFloat4;
		Pg::Math::PGQuaternion* _inputPGQuat;

		int _inputIndex;
	};
}


