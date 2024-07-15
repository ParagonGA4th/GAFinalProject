#pragma once
#include "ScriptInterface.h"
#include "IProjectile.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class StaticMeshRenderer;
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
		bool GetIsNowShooting(); //현재 쏘고 있는지 리턴.
		void CarryOutShoot(); //실제로 총을 쏘는 로직, Tweening 사용.

		bool _isSkillStart{ false };

	private:
		void EndShootingSelf(); //다 썼다는 얘기. 중력을 키지도 말고, 그냥사라지자.
	private:	//인게임 내 프로퍼티들.
		//N초 이후에 지워라!
		const float _afterDestroySec{ 3.0f };
		//떨어지기 전의 거리.
		const float _arrowDistBeforeFall{ 13.0f };
		// 미리 자기 자신에 영향 안 주는 Layer 설정되어 있어야.
		const float _secondsBeforeGravity{ 1.0f };

	private:	//ArrowShooterBehavior(매니저)가 참조할 변수이다.
		bool _isNowShooting = false;
		//스킬 끝나는 시간.
		bool _isSkillEnd{ false };

	private:
		bool _startCountingTime = false;
		float _elapsedTime = 0.0f;
		Pg::Math::PGFLOAT3 _initialPos;
		Pg::Math::PGFLOAT3 _targetPos;
		Pg::Math::PGFLOAT3 _shootDir;

	private:	//자신의 컴포넌트들.
		Pg::Data::StaticMeshRenderer* _meshRenderer;
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



