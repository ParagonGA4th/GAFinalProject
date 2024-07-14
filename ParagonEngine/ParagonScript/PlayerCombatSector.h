#pragma once
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/DynamicCollider.h"

#include "IObserver.h"
#include "IScriptResettable.h"

/// <summary>
/// (구) PlayerBattleBehavior가 담당하는 역할.
/// </summary>

namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Time { class PgTime; }
}

namespace Pg::DataScript
{
	class ArrowLogic;
}

namespace Pg::DataScript
{
	class PlayerHandler;

	class PlayerCombatSector : public IObserver, public IScriptResettable
	{
		friend class PlayerHandler;

		inline static const float STRONG_ATTACK_COOLDOWN_TIME = 3.f;
		inline static const float ULTIMATE_ATTACK_COOLDOWN_TIME = 30.f;
		inline static const float ULTIMATE_ATTACK_REQUIRED_MANA = 10.f;

	public:
		PlayerCombatSector(PlayerHandler* playerHandler);


		void GrabManagedObjects();

		//얘네들은 Component에 의해 작동되는 것 X, Script 자체에서 수명을 관리한다.
		void BeforePhysicsAwake();
		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void OnAnimationEnd(const std::string& justEndedAnimation);

	public:
		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

	private:
		void AllAttacksLogic();
		void NormalArrowShootingLogic();

		void FindAllArrowsInMap();
		void PlayAdequateAnimation();

		void SelectActivateActiveSkill();
		void ProcessInputsForStrongAttack();
		void ProcessInputsForUltimateAttack();

		//얘네들은 실행 시키고, 로직 실행은 외적으로.
		//여러 프레임 동안 이루어져야 한다. 
		//성공하면 True / 실패하면 False.
		//(이는 다른 공격의 실행 상태에 따라 결정)
		bool ActivateStrongAttack();
		bool ActivateUltimateAttack();
		bool ActivateFireAttack();
		bool ActivateIceAttack();

		//즉시 실행하는 함수들.
		void UpdateExecuteStrongAttack();
		void UpdateExecuteUltimateAttack();
		//Ice/Fire/Normal
		//모든 공격들에 한정해서, 벡터와 오디오 등을 받으면 일괄적으로 Shoot 실행.
		void ExecuteSpecificArrowShoot(std::vector<ArrowLogic*>* typeArrowVec, 
			Pg::Data::AudioSource* audioSource, float& outIfDoneResetTime);
	private:
		PlayerHandler* _playerHandler;
		Pg::Data::GameObject* _object;
		std::vector<ArrowLogic*> _normalArrowVec;
		std::vector<ArrowLogic*> _iceArrowVec;
		std::vector<ArrowLogic*> _fireArrowVec;
	private:
		//Normal에 한정.
		float _normal_timeSinceLastShot = 0.f;
		//공격 쿨타임
		const float _normal_shootCooldown = 0.6f;

	private:
		// 플레이어 애니매이션 관련 변수
		std::string _prevAnimationInput;
		//bool _isHit;
		//int _hitCount = 0;

	private:
		//강공격 실행을 위해, 클릭한 순간들을 기록한다.
		float _startedClickingTime{ 0.f };
		bool _isStrongAttackStartEligible{ true };
		float _startedStrongAttackChargeTime{ 0.f };
		bool _isStrongAttackingNow{ false };

		//궁극기 실행을 위해, 클릭한 순간들을 기록한다.
		bool _isUltimateAttackStartEligible{ true };
		float _isStartedUltimateAttackChargeTime{ 0.f };
		bool _isUltimateAttackingNow{ false };
		bool _isIceAttackingNow{ false };
		bool _isFireAttackingNow{ false };

	private:
		Pg::API::Input::PgInput* _pgInput;
		Pg::API::Time::PgTime* _pgTime;
	};
}


