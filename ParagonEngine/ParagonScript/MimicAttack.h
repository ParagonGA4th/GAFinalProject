#pragma once
#include "ScriptInterface.h"

namespace Pg::DataScript
{
	class CombatSystem;
	class MimicBehaviour;

	class MimicAttack : public ScriptInterface<MimicAttack>
	{
		DEFINE_PARAGON_SCRIPT(MimicAttack);
	public:
		MimicAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	private:
		CombatSystem* _combatSystem{ nullptr };
		MimicBehaviour* _mimicBehaviour;
	};
}

