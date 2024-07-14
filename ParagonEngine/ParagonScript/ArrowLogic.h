#pragma once
#include "ScriptInterface.h"
#include "IProjectile.h"
#include "../ParagonMath/PgMath.h"

/// <summary>
/// Normal / Fire / Ice를 모두 담당할 것이다.
/// </summary>

namespace Pg::Data
{
	class StaticMeshRenderer;
	class BoxCollider;
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
	class ArrowLogic : public ScriptInterface<ArrowLogic>, public IProjectile
	{
		DEFINE_PARAGON_SCRIPT(ArrowLogic);

	public:
		inline static const float ARROW_ATTACK_POWER = 1.f;

	public:
		ArrowLogic(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void FixedUpdate() override;

		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;
		//virtual void OnCollisionExit(Pg::Data::PhysicsCollision** _colArr, unsigned int count) override;


	public:
		//Material은 이 Property 설정과 더불어 다르게 등록되어야 한다.
		//FitArrow는 같되, 내부 Material을 모두 다르게 등록하자.
		BEGIN_VISITABLES(ArrowLogic);
		VISITABLE(int, _arrowType); // -1 : Ice / 0 : Normal / 1 : Fire.
		END_VISITABLES;


	public:
		//BattleBehavior 스크립트를 갖고 로직 제어할 수 있게 하기. 
		//외적으로 할당해서 값을 넣어준다.
		PlayerHandler* _playerBattleBehavior{ nullptr };

	public:
		void ResetState(); //상태 내부 리셋.
		bool GetIsNowShooting(); //현재 쏘고 있는지 리턴.
		void ShootArrow(Pg::Math::PGFLOAT3 initialPos, Pg::Math::PGFLOAT3 shootDir); //자기 자신인 화살을 쏘기.

	private:
		void InitSelfAsNormalArrow();
		void InitSelfAsIceArrow();
		void InitSelfAsFireArrow();


	private:
		void EndShootingSelf(); //다 썼다는 얘기. 중력을 키지도 말고, 그냥사라지자.
		void CarryOutShoot(); //실제로 총을 쏘는 로직, Tweening 사용.
		void IfValidActualShootLogic(); //실제 총 쏘는 If문.
	private:	//인게임 내 프로퍼티들.
		//N초 이후에 지워라!
		const float _afterDestroySec{ 3.0f };
		//떨어지기 전의 거리.
		const float _arrowDistBeforeFall{ 13.0f };
		// 미리 자기 자신에 영향 안 주는 Layer 설정되어 있어야.
		const float _secondsBeforeGravity{ 1.0f };

	private:	//ArrowShooterBehavior(매니저)가 참조할 변수이다.
		bool _isNowShooting = false;
		
	private:
		bool _startCountingTime = false;
		float _elapsedTime = 0.0f;
		Pg::Math::PGFLOAT3 _initialPos;
		Pg::Math::PGFLOAT3 _targetPos;
		Pg::Math::PGFLOAT3 _shootDir;

	private:	//자신의 컴포넌트들.
		Pg::Data::StaticMeshRenderer* _meshRenderer;
		Pg::Data::BoxCollider* _collider;

	private:	//API
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;

	private:
		//ComboSystem 갖고 있기.
		ComboSystem* _comboSystem{ nullptr };

		CombatSystem* _combatSystem{ nullptr };
	};
}


