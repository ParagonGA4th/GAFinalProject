#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data { class Collider; class StaticBoxCollider; }
namespace Pg::API::Time { class PgTime; }

namespace Pg::DataScript
{
	class PlayerMovement;
	class PlayerBattleBehavior;

	class TrapArea : public ScriptInterface<TrapArea>
	{
		DEFINE_PARAGON_SCRIPT(TrapArea);

	public:
		TrapArea(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		virtual void OnTriggerEnter(Pg::Data::Collider* col) override;

	private:
		Pg::API::Time::PgTime* _deltaTime;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::DataScript::PlayerMovement* _playerMovement;
		Pg::DataScript::PlayerBattleBehavior* _playerBattleBehavior;

		// 플레이어의 기본 속도
		float _previousMoveSpeed;
		float _fallSpeed = 0.2f; // 빠지는 속도
		float _damage = 0.2f;	 // 체력 감소 속도	
	};
}

