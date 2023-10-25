#pragma once
#include <string>

namespace Pg::UI
{
	class ILayout
	{
		friend class Panel;
	public:
		virtual void CreateWidget() abstract;

	protected:
		virtual void Update() abstract;
		virtual std::string GetPanelName() abstract;
	};
}


