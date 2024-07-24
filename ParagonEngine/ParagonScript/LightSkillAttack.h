#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
}

namespace Pg::DataScript
{
	class CombatSystem;

	class LightSkillAttack : public ScriptInterface<LightSkillAttack>
	{
		DEFINE_PARAGON_SCRIPT(LightSkillAttack);
	public:
		LightSkillAttack(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void Awake() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		void OnAnimationEnd(const std::string& justEndedAnimation);

	private:
		CombatSystem* _combatSystem{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _renderer;

		bool _animEnd{ false };
	};
}


