#pragma once
#include "ScriptInterface.h"

/// <summary>
/// RayCast를 테스트하기 위한 스크립트.
/// 2024.01.25
/// </summary>

namespace Pg::API
{
	namespace Raycast
	{
		class PgRayCast;
	}

	namespace Input
	{
		class PgInput;
	}
}

namespace Pg::DataScript
{
class RayCastTest : public ScriptInterface<RayCastTest>
{
	DEFINE_PARAGON_SCRIPT(RayCastTest);

public:
	RayCastTest(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::API::Raycast::PgRayCast* _pgRayCast = nullptr;
	Pg::API::Input::PgInput* _pgInput = nullptr;

};
}

