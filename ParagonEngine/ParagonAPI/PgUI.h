#pragma once
#include "../ParagonData/Button.h"
#include "../ParagonData/Slider.h"

namespace Pg::Data
{
	class Button;
}

/// <summary>
/// 스크립팅 시 사용할 UI 관련 함수들.
/// </summary>
namespace Pg::API::UI
{
	class PgUI
	{
	public:
		void Initialize();

		void SetOnClickEvent(std::function<void()> onClickEvent);
		void Click();
	private:
		Pg::Data::Button* _button;
	};
}
