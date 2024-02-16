#pragma once
#include "RendererBase2D.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/DebugData.h"


#include <functional>

/// <summary>
/// 버튼을 위한 클래스. 
/// 23.10.10
/// </summary>

namespace Pg::Data
{
	class GameObject;

	class Button : public RendererBase2D
	{
	public:
		Button(GameObject* obj);

	public:
		virtual void Update() override;

		void Click();

		void SetOnClickEvent(std::function<void()> onClickEvent);
		std::function<void()> GetOnClickEvent() const;

	public:
		//Debug System용 사용
		Box2DInfo _box2dInfo;

	private:
		//TODO 나중에 필요하면 템플릿으로 바꿀 것
		std::function<void()> _onClickEvent;

		bool _isPressed;	//버튼 누른 여부 체크.
	};
}


