#pragma once
#include "IWidget.h"
#include <string>

namespace Pg::UI::Widget
{
	class Button : public Pg::UI::IWidget
	{
	public:
		Button(std::string label, float x, float y);
		virtual void Update() override;

		static bool _isButtonClick;
	protected:
		std::string _btnName;
		float _x;
		float _y;
	};
}

