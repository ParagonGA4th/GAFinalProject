#pragma once
#include "ScriptInterface.h"


namespace Pg::Data
{
	class StaticBoxCollider;
}

namespace Pg::DataScript
{
	class CombatSystem;

	class GolemBossSkillAttack : public ScriptInterface<GolemBossSkillAttack>
	{
		DEFINE_PARAGON_SCRIPT(GolemBossSkillAttack);
	public:
		GolemBossSkillAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	private:
		CombatSystem* _combatSystem{ nullptr };
	};
}


