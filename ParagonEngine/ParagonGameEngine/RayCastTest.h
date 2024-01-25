#pragma once
#include "../ParagonData/Script.h"

/// <summary>
/// RayCast를 테스트하기 위한 스크립트.
/// 2024.01.25
/// </summary>

namespace Pg::Engine
{
	namespace Input
	{
		class InputSystem;
	}

	namespace Physic
	{
		class PhysicSystem;
	}
}

namespace Pg::API
{
	namespace Raycast
	{
		class PgRayCast;
	}
}

class RayCastTest : public Pg::Data::Script
{
public:
	RayCastTest(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

private:
	Pg::Engine::Input::InputSystem* tInput = nullptr;

	Pg::Engine::Physic::PhysicSystem* tPhysic = nullptr;
};

 