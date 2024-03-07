#pragma once
#include "../ParagonData/Script.h"

/// <summary>
/// 구현한 각종 이벤트들을 테스트할 스크립트.
/// </summary>

namespace Pg::Data
{
	class Button;
}

namespace Pg::Engine
{
	//namespace Input
	//{
	//	class InputSystem;
	//}

	class EventTest : public Pg::Data::Script
	{
	public:
		EventTest(Pg::Data::GameObject* owner);

	public:
		virtual void Start() override;
		virtual void Update() override;

	private:
		//Input::InputSystem* _inputSystem = nullptr;
		Pg::Data::Button* tButton;
	};
}

