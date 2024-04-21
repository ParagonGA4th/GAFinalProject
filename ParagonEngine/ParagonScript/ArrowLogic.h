#pragma once
#include "ScriptInterface.h"
#include "../ParagonMath/PgMath.h"

namespace Pg::Data
{
	class StaticMeshRenderer;
	class CapsuleCollider;
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
	class ArrowLogic : public ScriptInterface<ArrowLogic>
	{
		DEFINE_PARAGON_SCRIPT(ArrowLogic);

	public:
		ArrowLogic(Pg::Data::GameObject* obj);

		virtual void BeforePhysicsUpdate() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void FixedUpdate() override;

	public:
		void ResetState(); //상태 내부 리셋.
		bool GetIsNowShooting(); //현재 쏘고 있는지 리턴.
		void ShootArrow(Pg::Math::PGFLOAT3 initialPos, Pg::Math::PGFLOAT3 shootDir); //자기 자신인 화살을 쏘기.

	private:
		void EnableDrop(); //중력 다시 켜기.
		void CarryOutShoot(); //실제로 총을 쏘는 로직, Tweening 사용.

	private:	//인게임 내 프로퍼티들.
		//N초 이후에 지워라!
		const float _afterDestroySec{ 3.0f };
		//떨어지기 전의 거리.
		const float _arrowDistBeforeFall{ 5.0f };
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
		Pg::Data::CapsuleCollider* _collider;

	private:	//API
		Pg::API::Time::PgTime* _pgTime;
		Pg::API::Tween::PgTween* _pgTween;

	private:
		//미리 BeforePhysicsUpdate를 호출했었는지.
		bool _alreadyCalledBPU = false;
	};
}


