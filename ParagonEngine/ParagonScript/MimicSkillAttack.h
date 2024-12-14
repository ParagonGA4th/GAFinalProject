#pragma once
#include "ScriptInterface.h"
#include <visit_struct/visit_struct.hpp>

namespace Pg::Data
{
	class GameObject;
	class StaticBoxCollider;
	class StaticMeshRenderer;
}

namespace Pg::DataScript
{
	class CombatSystem;
	class MimicBehaviour;

	class MimicSkillAttack : public ScriptInterface<MimicSkillAttack>
	{
		DEFINE_PARAGON_SCRIPT(MimicSkillAttack);
	public:
		MimicSkillAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		bool _isPlayerHit{ false };

	public:

		//개별적으로 쓰이는 코인 오브젝트가 다르기에 설정.
		BEGIN_VISITABLES(Pg::DataScript::MimicSkillAttack);
		VISITABLE(std::string, _coinName);
		END_VISITABLES;

	private:
		CombatSystem* _combatSystem{ nullptr };
		MimicBehaviour* _mimicBehaviour;

		Pg::Data::StaticBoxCollider* _col;

		Pg::Data::GameObject* _coin;
		Pg::Data::StaticMeshRenderer* _coinRenderer;

		Pg::Math::PGFLOAT3 _startPosition = { 0.f, 1.f, 2.5f };

	};
}

