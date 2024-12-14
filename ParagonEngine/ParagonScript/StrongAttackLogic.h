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
/// (WindAttack) �������� ���ؼ�, SkinnedMeshRenderer�� ������ �־�� �ϰ� () / 
/// Static Sphere Collider�� �޾ƾ� �Ѵ�. �ƾ�
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
		//���� �� ���� ȣ��Ǿ�� �ϴ� �Լ����̴�.
		void Activate(const Pg::Math::PGFLOAT3* basePos);
		void Deactivate();

	private:
		void ApplyKnockback(Pg::Data::Collider* col);

	private:
		//���� ���� �������ִ� Bool.
		bool _isAppearing{ false };
		bool _isDisappearing{ false };
		bool _isActivated{ false };


	private: 
		//API
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::Tween::PgTween* _pgTween{ nullptr };

		//�ڽ� ������Ʈ ���� Ÿ ������Ʈ.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer{ nullptr };
		Pg::Data::StaticSphereCollider* _collider{ nullptr }; // Ʈ���ſ��� �Ѵ�.

		//���� ��.
		const Pg::Math::PGFLOAT3* _basePos{ nullptr }; //���̽��� �Ǿ� ������ ��ġ, �÷��̾�� �޾ƿ� ���̴�.
		//��Ȱ��ȭ ��, ���������� �ִ� Position.
		Pg::Math::PGFLOAT3 _previousEndedPosition{};

		const Pg::Math::PGFLOAT3 _prisonPos{ 0, -100, 0 };

		Pg::Data::Transform* _playerTransform{ nullptr };
	private:
		//ComboSystem ���� �ֱ�.
		ComboSystem* _comboSystem{ nullptr };
		CombatSystem* _combatSystem{ nullptr };
		
	};
}


