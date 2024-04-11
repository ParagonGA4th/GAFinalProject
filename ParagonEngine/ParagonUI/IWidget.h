#pragma once
#include <string>

namespace Pg::UI
{
	// widget의 최상위 인터페이스
	class IWidget
	{
	public:
		// Draw 
		virtual void Update() abstract;
		virtual std::string GetWidgetLabel() abstract;
	};
}

