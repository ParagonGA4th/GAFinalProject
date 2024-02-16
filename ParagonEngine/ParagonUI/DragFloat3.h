#pragma once
#include "IWidget.h"
#include "../ParagonMath/PgMath.h"

#include <string>

namespace Pg::UI::Widget
{
	class DragFloat3 : public IWidget
	{
	public:
		DragFloat3(std::string label, Pg::Math::PGFLOAT3* input);
		DragFloat3(std::string label, Pg::Math::PGFLOAT4* input);
		DragFloat3(std::string label, Pg::Math::PGQuaternion* input);
		virtual void Update() override;

	private:
		std::string _label;

		float _inputFloat[3];

		Pg::Math::PGFLOAT3* _inputPGFloat3;
		Pg::Math::PGFLOAT4* _inputPGFloat4;
		Pg::Math::PGQuaternion* _inputPGQuat;

		int _inputIndex;
	};
}


