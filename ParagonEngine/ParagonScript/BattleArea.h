#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data 
{ 
	class GameObject; 
	class Collider;
	class StaticBoxCollider;
	class CapsuleCollider;
}

namespace Pg::DataScript
{
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
		virtual void OnTriggerExit(Pg::Data::Collider* col) override;
		
	private:
		Pg::Data::StaticBoxCollider* _collider;
		bool _isInit;
	};
}

