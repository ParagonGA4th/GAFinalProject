#pragma once
#include "ScriptInterface.h"

/// <summary>
/// 구현한 각종 이벤트들을 테스트할 스크립트.
/// </summary>

namespace Pg::Data
{
	class Button;
}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::DataScript
{
	class EventTest : public ScriptInterface<EventTest>
	{
		DEFINE_PARAGON_SCRIPT(EventTest);

	public:
		EventTest(Pg::Data::GameObject* owner);

	public:
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::Data::Button* tButton;
		Pg::API::Input::PgInput* _pgInput = nullptr;
	};

	//const bool EventTest::registered_ = ScriptInterface<EventTest>::register_type();
}



