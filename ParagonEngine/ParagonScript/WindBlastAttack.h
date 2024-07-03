#pragma once
#include "ScriptInterface.h"

namespace Pg::DataScript
{
	class CombatSystem;

	class WindBlastAttack : public ScriptInterface<WindBlastAttack>
	{
		DEFINE_PARAGON_SCRIPT(WindBlastAttack);
	public:
		WindBlastAttack(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	private:
		CombatSystem* _combatSystem{ nullptr };
	};
}


