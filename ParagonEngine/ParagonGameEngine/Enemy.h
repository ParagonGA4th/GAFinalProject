#pragma once
#include "../ParagonData/Component.h"
#include "AISeight.h"

#include <vector>


namespace Pg::Data
{
	class GameObject;
	class Collider;
	class DynamicCollider;
	class StaticBoxCollider;
	class StaticSphereCollider;
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

class AISeight;

/// <summary>
/// AI관련 스크립팅
/// 2024.04.16
/// </summary>
class Enemy : public Pg::Data::Component
{
public:

	Enemy(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(Pg::Data::Collider* c);
	virtual void OnTriggerExit(Pg::Data::Collider* c);

private:
	std::vector<Pg::Data::StaticBoxCollider*> colVec;
	//std::vector<Pg::Data::StaticSphereCollider*> colVec;
	Pg::Util::Time::TimeSystem* tTime = nullptr;
	Pg::Engine::Physic::PhysicSystem* tPhysic = nullptr;

	std::vector<AISeight*> aiSightVec;

	bool _colVecActive = true;

};

