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
	class WaspBehaviour;

	class WaspAttack : public ScriptInterface<WaspAttack>
	{
		DEFINE_PARAGON_SCRIPT(WaspAttack);
	public:
		WaspAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		bool _isPlayerHit{ false };

	public:

		//개별적으로 쓰이는 코인 오브젝트가 다르기에 설정.
		BEGIN_VISITABLES(Pg::DataScript::WaspAttack);
		VISITABLE(std::string, _cornName);
		END_VISITABLES;

	private:
		CombatSystem* _combatSystem{ nullptr };
		WaspBehaviour* _waspBehaviour;

		Pg::Data::StaticBoxCollider* _col;

		Pg::Data::GameObject* _corn;
		Pg::Data::StaticMeshRenderer* _cornRenderer;

		Pg::Math::PGFLOAT3 _startPosition = { 0.f, 1.f, 2.5f };

	};
}

