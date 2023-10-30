#pragma once
#include "IWidget.h"
#include <string>

namespace Pg::UI::Widget
{
	class InputFloat3 : public IWidget
	{
	public:
		InputFloat3(std::string label, float input[3]);
		virtual void Update() override;
	
	private:
		std::string _label;
		float _inputfloat[3];
		float* _inputPtr;
	};
}


