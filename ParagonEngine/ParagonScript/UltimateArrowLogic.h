#pragma once
#include "ScriptInterface.h"
#include "IProjectile.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class SkinnedMeshRenderer;
	class StaticSphereCollider;
	class PhysicsCollision;
}

namespace Pg::API
{
	namespace Time
	{
		class PgTime;
	}

	namespace Tween
	{
		class PgTween;
	}
}

namespace Pg::DataScript
{
	class PlayerHandler;
	class ComboSystem;
	class CombatSystem;
}

namespace Pg::DataScript
{
	class UltimateArrowLogic : public ScriptInterface<UltimateArrowLogic>, public IProjectile
	{
		DEFINE_PARAGON_SCRIPT(UltimateArrowLogic);

	public:
		inline static const float ARROW_ATTACK_POWER = 1.f;

	public:
		UltimateArrowLogic(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		//virtual void OnCollisionExit(Pg::Data::PhysicsCollision** _colArr, unsigned int count) override;


		//BattleBehavior 스크립트를 갖고 로직 제어할 수 있게 하기. 
		//외적으로 할당해서 값을 넣어준다.
		PlayerHandler* _playerBattleBehavior{ nullptr };

	public:
		void CarryOutShoot(); //실제로 총을 쏘는 로직, Tweening 사용.

		bool _isSkillStart{ false };

	private:
		void EndShootingSelf();

	private:
		//스킬 끝나는 시간.
		bool _isSkillEnd{ false };

	private:

		bool _startCountingTime = false;
		float _elapsedTime = 0.0f;


	private:	//자신의 컴포넌트들.
		Pg::Data::SkinnedMeshRenderer* _meshRenderer;
		Pg::Data::StaticSphereCollider* _collider;

	private:	//API
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;

	private:
		//ComboSystem 갖고 있기.
		ComboSystem* _comboSystem{ nullptr };

		CombatSystem* _combatSystem{ nullptr };
	};
}



