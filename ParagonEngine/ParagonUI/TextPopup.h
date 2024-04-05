#pragma once
#include "IWidget.h"
#include <vector>

namespace Pg::UI::Widget
{
	class TextPopup : public IWidget
	{
	public:
		TextPopup(std::string TextPopupName, bool& popupShow, std::string text);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _popupName;
		std::string _text;
		bool& _isPopupShow;
	};
}


