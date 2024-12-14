#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget
{
	class Button : public Pg::UI::IWidget
	{
	public:
		Button(std::string label, float width, float height, bool& btnClick);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	protected:
		std::string _btnName;
		
		float _width;
		float _height;
		
		bool& _isButtonClick;
	};
}

