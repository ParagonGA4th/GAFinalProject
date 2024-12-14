#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::UI::Widget
{
	class DragFloat3 : public IWidget
	{
	public:
		DragFloat3(std::string label, Pg::Math::PGFLOAT3* input, float min = 0.f);
		DragFloat3(std::string label, Pg::Math::PGFLOAT4* input, float min = 0.f);
		DragFloat3(std::string label, Pg::Math::PGQuaternion* input, float min = 0.f);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _label;

		float _inputFloat[3];

		Pg::Math::PGFLOAT3* _inputPGFloat3;
		Pg::Math::PGFLOAT4* _inputPGFloat4;
		Pg::Math::PGQuaternion* _inputPGQuat;

		int _inputIndex;

		float _minFloat;
	};
}


