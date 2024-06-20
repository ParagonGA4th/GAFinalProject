#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data 
{ 
	class Collider; 
	class SphereCollider;
}

namespace Pg::DataScript
{
	class PlayerBattleBehavior;
	class BattleArea : public ScriptInterface<BattleArea>
	{
		DEFINE_PARAGON_SCRIPT(BattleArea);

	public:
		BattleArea(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;
		
	private:
		Pg::Data::SphereCollider* _collider;
		Pg::DataScript::PlayerBattleBehavior* _player;

		int _monster = 1;
		bool _onTriggerStay;
	};
}

