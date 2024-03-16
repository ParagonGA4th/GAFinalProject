#pragma once
#include "Script.h"
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

class RayCastTest : public Pg::script::Script
{
public:
	RayCastTest(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::API::Raycast::PgRayCast* _pgRayCast = nullptr;
	Pg::API::Input::PgInput* _pgInput = nullptr;

};

