#pragma once
#include "ScriptInterface.h"
#include "IMovableObject.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data 
{ 
	class Collider; 
	class SphereCollider;
}

namespace Pg::DataScript
{
	class PlayerHandler;
	class BattleArea : public ScriptInterface<BattleArea>, public IMovableObject
	{
		DEFINE_PARAGON_SCRIPT(BattleArea);

	public:
		BattleArea(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	public:
		virtual void ResetAll() override;
		virtual unsigned int GetDesignatedAreaIndex() override;
	private:
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		virtual void OnTriggerExit(Pg::Data::Collider** _colArr, unsigned int count) override;
		
	private:
		Pg::Data::SphereCollider* _collider;
		Pg::DataScript::PlayerHandler* _player;

		int _monster = 1;
		bool _onTriggerStay;
	};
}

