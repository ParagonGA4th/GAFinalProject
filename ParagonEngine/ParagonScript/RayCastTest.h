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
public:
	static inline const std::string class_identifier = "class RayCastTest";
	static std::unique_ptr<Script> create_instance(Pg::Data::GameObject* obj) { return std::make_unique<RayCastTest>(obj); }

public:
	RayCastTest(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::API::Raycast::PgRayCast* _pgRayCast = nullptr;
	Pg::API::Input::PgInput* _pgInput = nullptr;

};
}

