#pragma once
#include "IWidget.h"
#include <string>

namespace Pg::UI::Widget
{
	class Button : public Pg::UI::IWidget
	{
	public:
		Button(std::string label, float width, float height);
		virtual void Update() override;
		bool* GetBtnClick();

	protected:
		std::string _btnName;
		
		float _width;
		float _height;
		
		bool _isButtonClick;
	};
}

