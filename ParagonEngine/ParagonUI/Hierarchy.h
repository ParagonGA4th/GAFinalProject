#pragma once
#include "IWidget.h"

#include <map>
#include <utility>
#include <vector>
#include <string>

namespace Pg::UI::Widget
{
	enum eMode
	{
		GROUP,
		MOVE
	};

	class Hierarchy : public IWidget
	{
	public:
		Hierarchy(std::map<int, std::pair<std::string, std::vector<std::string>>>& objNameList);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

		std::string* GetSelectObjectName();
		bool* GetBtnClick();
		bool* GetKeyDeleteInput();
		bool* GetISObjectChildrenChange();
		
	private:
		// Selectable
		std::map<int, std::pair<std::string, std::vector<std::string>>>& _objNameList;
		std::string selectObj;
		std::string selectObjParent;
		int _mode;

		bool _isNodeOpen;
		bool _isNodeSelected;
		bool _isAddObject;
		bool _isDeleteObject;
		bool _isObjectChildrenChange;
	};
}


