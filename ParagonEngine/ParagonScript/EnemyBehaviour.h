#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class GameObject;
	class StaticBoxCollider;
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

	class EnemyBehaviour : public ScriptInterface<EnemyBehaviour>
	{
		DEFINE_PARAGON_SCRIPT(EnemyBehaviour);

	public:
		EnemyBehaviour(Pg::Data::GameObject* obj);

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		Pg::API::Raycast::PgRayCast* _pgRayCast = nullptr;
		EnemySight* _enemySight = nullptr;

		std::vector<Pg::Data::StaticBoxCollider*> colVec;
		std::vector<EnemySight*> aiSightVec;

		Pg::Data::SkinnedMeshRenderer* _renderer;

		bool _colVecActive = true;
	};

	//const bool PlayerMove::registered_ = ScriptInterface<PlayerMove>::register_type();
}