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
		
	private:
		// Selectable
		std::map<int, std::pair<std::string, std::vector<std::string>>>& _objNameList;
		int _selectedNumber;
		int _mode;
	};
}


