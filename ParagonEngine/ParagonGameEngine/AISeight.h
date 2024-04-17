#pragma once
#include "../ParagonData/Component.h"


namespace Pg::Data
{
	class GameObject;
	class Collider;
	class DynamicCollider;
}

/// <summary>
/// AI관련 스크립팅
/// 2024.04.16
/// </summary>
class AISeight : public Pg::Data::Component
{
public:

	AISeight(Pg::Data::GameObject* obj);

	virtual void Start() override;
	virtual void Update() override;

	virtual void OnTriggerEnter(Pg::Data::Collider* c);
};

