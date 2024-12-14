#pragma once
#include "ScriptInterface.h"

namespace Pg::DataScript
{
	class CombatSystem;

	class TrentAttack : public ScriptInterface<TrentAttack>
	{
		DEFINE_PARAGON_SCRIPT(TrentAttack);
	public:
		TrentAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
	private:
		CombatSystem* _combatSystem{ nullptr };
	};
}
