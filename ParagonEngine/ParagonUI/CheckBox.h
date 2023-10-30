#pragma once
#include "IWidget.h"
#include <string>

namespace Pg::UI::Widget 
{
	class CheckBox : public IWidget
	{
	public:
		CheckBox(std::string label, bool& check);
		virtual void Update() override;
	
	private:
		bool& _isCheck;
		std::string _label;
	};
}


