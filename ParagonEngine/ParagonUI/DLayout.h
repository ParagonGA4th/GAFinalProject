#pragma once
#include "ILayout.h"
#include <vector>

namespace Pg::UI { class IWidget; }

namespace Pg::UI::Layout
{
	class DLayout : public Pg::UI::ILayout
	{
	public:
		DLayout(std::string layoutName);
		~DLayout();

		virtual void CreateWidget() override;
	private:
		virtual void Update() override;
		virtual std::string GetPanelName() override;

	private:
		std::string _layoutName;
		std::vector<Pg::UI::IWidget*> _widgets;
	};
}
