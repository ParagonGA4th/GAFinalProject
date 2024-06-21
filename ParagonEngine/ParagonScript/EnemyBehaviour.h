#pragma once
#include "ScriptInterface.h"
#include "BaseMonster.h"

namespace Pg::Data
{
	class GameObject;
	class Transform;
	class StaticBoxCollider;
	class BoxCollider;
	class CapsuleCollider;
	class SkinnedMeshRenderer;
	class MonsterHelper;
}

namespace Pg::API
{
	namespace Raycast
	{
		class PgRayCast;
	}
	namespace Time
	{
		class PgTime;
	}
}

/// <summary>
/// 몬스터 관련 스크립트
/// </summary>
namespace Pg::DataScript
{
	class EnemySight;
	class PlayerHandler;

	class EnemyBehaviour : public ScriptInterface<EnemyBehaviour>, public BaseMonsterInfo
	{
		DEFINE_PARAGON_SCRIPT(EnemyBehaviour);

	public:
		EnemyBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void BeforePhysicsUpdate() override;
		virtual void Start() override;
		virtual void Awake() override;
		virtual void Update() override;

	private:
		Pg::Data::CapsuleCollider* _collider;
		Pg::Data::MonsterHelper * _monsterHelper;
		Pg::API::Raycast::PgRayCast* _pgRayCast;
		EnemySight* _enemySight = nullptr;
		PlayerHandler* _playerBattleBehavior;

		std::vector<Pg::Data::StaticBoxCollider*> colVec;
		std::vector<Pg::Data::BoxCollider*> boxColVec;
		std::vector<EnemySight*> aiSightVec;

		Pg::Data::SkinnedMeshRenderer* _renderer;

		// 발표 시연을 위한 임시 변수들
		Pg::API::Time::PgTime* _deltaTime;
		float _time;
	};
}