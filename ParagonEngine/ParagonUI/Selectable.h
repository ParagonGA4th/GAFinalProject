#pragma once
#include "IWidget.h"

#include <vector>

namespace Pg::UI::Widget
{
	class Selectable : public IWidget
	{
	public:
		Selectable(std::vector<std::string> objNameList, std::string& selectedObject);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;
		bool* GetSelectedObjectDoubleClicked();

	private:
		std::vector<std::string> _selectList;
		std::string& _selectObj;
		std::string _prevSelectObj;

		bool _isDoubleClicked;
	};
}


