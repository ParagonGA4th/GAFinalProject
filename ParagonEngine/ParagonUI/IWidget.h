#pragma once
#include <string>

namespace Pg::UI
{
	// widget�� �ֻ��� �������̽�
	class IWidget
	{
	public:
		// Draw 
		virtual void Update() abstract;
		virtual std::string GetWidgetLabel() abstract;
	};
}

