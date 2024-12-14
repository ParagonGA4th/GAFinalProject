#pragma once
#include "../ParagonData/Component.h"


namespace Pg::Data
{
	class GameObject;
	class Collider;
	class DynamicCollider;
	class StaticBoxCollider;
}

namespace Pg::Engine
{
	namespace Physic
	{
		class PhysicSystem;
	}
}

namespace Pg::Util
{
	namespace Time
	{
		class TimeSystem;
	}
}

/// <summary>
/// AI관련 스크립팅
/// 2024.04.16
/// </summary>
class AISeight : public Pg::Data::Component
{
public:

	AISeight(Pg::Data::GameObject* obj);

	virtual void BeforePhysicsAwake() override;
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;

	bool _playerDetected = false;

private:
	Pg::Data::StaticBoxCollider* col;
	Pg::Util::Time::TimeSystem* tTime = nullptr;
	Pg::Engine::Physic::PhysicSystem* tPhysic = nullptr;

	float _afterPlayerExit = 0.0f;

};

