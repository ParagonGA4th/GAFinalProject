#pragma once
#include "ScriptInterface.h"
#include "IProjectile.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonData/Transform.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
	class StaticSphereCollider;
	class PhysicsCollision;
}

namespace Pg::API
{
	namespace Time { class PgTime; }
	namespace Tween { class PgTween; }
}

namespace Pg::DataScript
{
	class PlayerHandler;
	class ComboSystem;
	class CombatSystem;
}

/// <summary>
/// (WindAttack) 강공격을 위해서, SkinnedMeshRenderer를 가지고 있어야 하고 () / 
/// Static Sphere Collider를 받아야 한다. 컷씨
/// </summary>

namespace Pg::DataScript
{
	class StrongAttackLogic : public ScriptInterface<StrongAttackLogic>
	{
		DEFINE_PARAGON_SCRIPT(StrongAttackLogic);

	public:
		inline static const float STRONG_ATTACK_POWER = 10.f;
		inline static const float FADEINOUT_SPEED = 300.f;
		inline static const float KNOCKBACK_FACTOR = 16.0f;

	public:
		StrongAttackLogic(Pg::Data::GameObject* obj);

		virtual void GrabManagedObjects() override;
		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void LateUpdate() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

	public:
		void ResetSelfState();
		//각각 한 번만 호출되어야 하는 함수들이다.
		void Activate(const Pg::Math::PGFLOAT3* basePos);
		void Deactivate();

	private:
		void ApplyKnockback(Pg::Data::Collider* col);

	private:
		//내부 상태 관리해주는 Bool.
		bool _isAppearing{ false };
		bool _isDisappearing{ false };
		bool _isActivated{ false };


	private: 
		//API
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::Tween::PgTween* _pgTween{ nullptr };

		//자신 오브젝트 내부 타 컴포넌트.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer{ nullptr };
		Pg::Data::StaticSphereCollider* _collider{ nullptr }; // 트리거여야 한다.

		//로직 상.
		const Pg::Math::PGFLOAT3* _basePos{ nullptr }; //베이스가 되어 움직일 위치, 플레이어에서 받아올 것이다.
		//비활성화 전, 마지막으로 있던 Position.
		Pg::Math::PGFLOAT3 _previousEndedPosition{};

		const Pg::Math::PGFLOAT3 _prisonPos{ 0, -100, 0 };

		Pg::Data::Transform* _playerTransform{ nullptr };
	private:
		//ComboSystem 갖고 있기.
		ComboSystem* _comboSystem{ nullptr };
		CombatSystem* _combatSystem{ nullptr };
		
	};
}


