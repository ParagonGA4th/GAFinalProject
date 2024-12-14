#pragma once
#include "../ParagonData/Button.h"
#include "../ParagonData/Slider.h"

namespace Pg::Data
{
	class Button;
}

/// <summary>
/// ��ũ���� �� ����� UI ���� �Լ���.
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
