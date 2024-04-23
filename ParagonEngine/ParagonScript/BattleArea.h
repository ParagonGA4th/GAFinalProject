#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data { class Collider; class StaticBoxCollider; }

namespace Pg::DataScript
{
	class PlayerBehavior;
	class BattleArea : public ScriptInterface<BattleArea>
	{
		DEFINE_PARAGON_SCRIPT(BattleArea);

	public:
		BattleArea(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		virtual void OnTriggerEnter(Pg::Data::Collider* col) override;
		
	private:
		Pg::Data::StaticBoxCollider* _collider;
		Pg::DataScript::PlayerBehavior* _player;

		int _monster = 1;
	};
}

