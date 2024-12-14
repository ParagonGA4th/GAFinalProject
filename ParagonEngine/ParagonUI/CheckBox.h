#pragma once
#include "IWidget.h"

namespace Pg::UI::Widget 
{
	class CheckBox : public IWidget
	{
	public:
		CheckBox(std::string label, bool* check);
		CheckBox(std::string label, bool check);

		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;
	
		bool GetCheck();

	private:
		bool* _isCheck;
		std::string _label;
	};
}


