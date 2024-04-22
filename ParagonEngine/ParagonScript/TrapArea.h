#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data { class Collider; class StaticBoxCollider; }
namespace Pg::API::Time { class PgTime; }

namespace Pg::DataScript
{
	class PlayerBehavior;
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
		virtual void OnTriggerExit(Pg::Data::Collider* col) override;

	private:
		Pg::API::Time::PgTime* _deltaTime;
		Pg::Data::StaticBoxCollider* _collider;
		Pg::DataScript::PlayerBehavior* _player;

		float _previousMoveSpeed;
		float _fallSpeed;
		float _damage;

		bool _isInit;
		bool _prevIsInit;
	};
}

