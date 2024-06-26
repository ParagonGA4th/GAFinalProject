#pragma once
#include "ScriptInterface.h"


namespace Pg::Data
{
	class StaticBoxCollider;
}

namespace Pg::DataScript
{
	class CombatSystem;
	class MiniGolemAttack;

	class MiniGolemAttack : public ScriptInterface<MiniGolemAttack>
	{
		DEFINE_PARAGON_SCRIPT(MiniGolemAttack);
	public:
		MiniGolemAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	private:
		CombatSystem* _combatSystem{ nullptr };
		MiniGolemAttack* _miniGolemAttack{ nullptr };
	};
}

