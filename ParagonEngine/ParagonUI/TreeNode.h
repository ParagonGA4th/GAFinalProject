#pragma once
#include "IWidget.h"
#include <vector>

namespace Pg::UI::Widget
{
	class TreeNode : public Pg::UI::IWidget
	{
	public:
		TreeNode(std::string nodeName, std::vector<IWidget*> widgets);
		virtual void Update() override;
		virtual std::string GetWidgetLabel() override;

	private:
		std::string _nodeName;
		std::vector<IWidget*> _widgets;
	};
}

