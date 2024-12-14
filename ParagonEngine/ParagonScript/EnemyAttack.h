#pragma once
#include "ScriptInterface.h"
#include <vector>

/// <summary>
/// �Ϲ� ������ ������ �����ϱ� ���� ��ũ��Ʈ.
/// 2024.06.08
/// </summary>

namespace Pg::Data 
{
	class GameObject;
	class StaticBoxCollider;
}

namespace Pg::API
{
	class PgScene;
}

namespace Pg::DataScript
{
	class BossBehaviour;

	class EnemyAttack : public ScriptInterface<EnemyAttack>
	{
		DEFINE_PARAGON_SCRIPT(EnemyAttack);
	public:
		EnemyAttack(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::API::PgScene* _pgScene;
		Pg::Data::StaticBoxCollider* _collider;

		std::vector<Pg::Data::GameObject*> _enemy;
		Pg::DataScript::BossBehaviour* _enemyBehaviour;
	};
}

