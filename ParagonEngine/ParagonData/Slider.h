#pragma once
#include "Component.h"

#include <functional>
/// <summary>
/// 변지상의 Slider 컴포넌트.
/// Slider의 변경값에 따라 콜백되는 이벤트가 있어야 한다.
/// </summary>
namespace Pg::Data
{
	class Slider : Component
	{
	public:
		Slider(GameObject* owner);

		virtual void Update() override;

		void SetValue(float val);
		float GetValue();

	private:
		//슬라이더의 값이 변경될 때 발생하는 이벤트.
		std::function<void(float)> _onValueEvent;
		float _value;
	};
}
