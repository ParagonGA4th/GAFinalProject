#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/GameObject.h"

namespace Pg::Data
{
	class CapsuleCollider;
}

namespace Pg::DataScript
{
	class MushroomTrampoline : public ScriptInterface<MushroomTrampoline>
	{
		DEFINE_PARAGON_SCRIPT(MushroomTrampoline);

	public:
		MushroomTrampoline(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;

		//자신이 Trigger, 자신에 부딪힌 Collider들의 정보를 받아서 활용하게 된다.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	private:
		Pg::Data::CapsuleCollider* _playerCap;
		float _jumpPower = 80.f;

		bool _isTrigger{ false };
	};
}


