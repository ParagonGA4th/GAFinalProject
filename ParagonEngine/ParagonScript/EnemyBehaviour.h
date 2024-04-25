#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class GameObject;
	class Transform;
	class StaticBoxCollider;
	class BoxCollider;
	class CapsuleCollider;
	class SkinnedMeshRenderer;
}

namespace Pg::API
{
	namespace Raycast
	{
		class PgRayCast;
	}
}

/// <summary>
/// 몬스터 관련 스크립트
/// </summary>
namespace Pg::DataScript
{
	class EnemySight;
	class PlayerBattleBehavior;

	class EnemyBehaviour : public ScriptInterface<EnemyBehaviour>
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
		Pg::API::Raycast::PgRayCast* _pgRayCast;
		EnemySight* _enemySight = nullptr;
		PlayerBattleBehavior* _playerBattleBehavior;

		std::vector<Pg::Data::StaticBoxCollider*> colVec;
		std::vector<Pg::Data::BoxCollider*> boxColVec;
		std::vector<EnemySight*> aiSightVec;

		Pg::Data::SkinnedMeshRenderer* _renderer;

		bool _colVecActive = true;

		//미리 BeforePhysicsUpdate를 호출했었는지.
		bool _alreadyCalledBPU = false;

		bool _isAnimStart = false;
	};

	//const bool PlayerMove::registered_ = ScriptInterface<PlayerMove>::register_type();
}