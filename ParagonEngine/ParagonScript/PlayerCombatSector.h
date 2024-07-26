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

namespace Pg::Data 
{
	class StaticBoxCollider;
	class StaticSphereCollider;
}
namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Time { class PgTime; }
}

namespace Pg::DataScript
{
	class ArrowLogic;
	class UltimateArrowLogic;
	class StrongAttackLogic;
	class ComboSystem;
}

namespace Pg::DataScript
{
	class PlayerHandler;
	class Stage1GUIHandler;
	class Stage2GUIHandler;
	class BossStageGUIHandler;
	class BossBehaviour;

	class PlayerCombatSector : public IObserver, public IScriptResettable
	{
		friend class PlayerHandler;
		friend class Stage1GUIHandler;
		friend class Stage2GUIHandler;
		friend class BossStageGUIHandler;

		//Monster OnHit : 5의 마나를 얻는 느낌.
		//ex. Monster Wave에 들어가서, 3마리를 잡으면 스킬 쓸 수 있을 것이다.
		//Mana는 0으로 시작.
		inline static const float STRONG_ATTACK_COOLDOWN_TIME = 3.f;
		inline static const float ULTIMATE_ATTACK_COOLDOWN_TIME = 30.f;
		inline static const float ICE_ATTACK_COOLDOWN_TIME = 10.f;
		inline static const float FIRE_ATTACK_COOLDOWN_TIME = 10.f;

		inline static const float ULTIMATE_ATTACK_DURATION = 3.0f;
		inline static const float STRONG_ATTACK_DURATION = 1.0f;

		inline static const float ULTIMATE_ATTACK_REQUIRED_MANA = 100.f;
		inline static const float ICE_ATTACK_REQUIRED_MANA = 30.f;
		inline static const float FIRE_ATTACK_REQUIRED_MANA = 30.f;

		//100 중 1/5
		inline static const int STRONG_ATTACK_REQUIRED_STAMINA = 1;

		inline static const float AFTER_ATTACK_WATING_TIME = 0.5f;

		// Stamina : 15초에 한칸씩 찬다. 총 5칸. 재충전 시간 조건은 0칸이어야.
		// 다 충전되기 전에는 쓰일 수 없다.

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
		//CombatSystem으로 이동.
		////Monster Script들이 자의적으로 호출하는 함수.
		//void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		//void AddMonsterOnHitList(BaseMonsterInfo* monster);
	private:
		void UpdateForGUIVariables();
		void ProcessInputsForActiveSkills();
		void ProcessInputsForStrongAttack();
		void ProcessInputsForUltimateAttack();

		//얘네들은 실행 시키고, 로직 실행은 외적으로.
		//여러 프레임 동안 이루어져야 한다. 
		//성공하면 True / 실패하면 False.
		//(이는 다른 공격의 실행 상태 + 스탯에 따라 결정)
		//내부에서 필요 스탯 소모까지 완료한다.

		//체크만 완료하는 애들.
		bool CheckStrongAttack();
		bool CheckUltimateAttack();
		bool CheckFireAttack();
		bool CheckIceAttack();

		bool CheckActivateStrongAttack();
		bool CheckActivateUltimateAttack();
		bool CheckActivateFireAttack();
		bool CheckActivateIceAttack();

		//즉시 실행하는 함수들.
		void UpdateExecuteStrongAttack();
		void UpdateExecuteUltimateAttack();
		//Ice/Fire/Normal
		//모든 공격들에 한정해서, 벡터와 오디오 등을 받으면 일괄적으로 Shoot 실행.
		void ExecuteSpecificArrowShoot(std::vector<ArrowLogic*>* typeArrowVec, 
			Pg::Data::AudioSource* audioSource, float& outIfDoneResetTime);
		
	private:
		//지상이 형 / 민서가 내부를 채워줘야 할 함수들.
		//딱 한번, 공격이 나가야 할 때 단발적으로 호출되는 함수.
		void InvokeSingleUltimateAttack();
		void InvokeSingleStrongAttack();
	
	private:
		PlayerHandler* _playerHandler;
		Pg::Data::GameObject* _object;
		std::vector<ArrowLogic*> _normalArrowVec;
		std::vector<ArrowLogic*> _iceArrowVec;
		std::vector<ArrowLogic*> _fireArrowVec;

		UltimateArrowLogic* _ulArrowLogic;
		StrongAttackLogic* _strongAttackLogic;

		ComboSystem* _comboSystem;
		BossBehaviour* _bossBehaviour;
	private:
		//Normal에 한정.
		float _normal_timeSinceLastShot = 0.f;
		//공격 쿨타임
		const float _normal_shootCooldown = 0.8f;

	private:
		// 플레이어 애니매이션 관련 변수
		std::string _prevAnimationInput;
		//bool _isHit;
		//int _hitCount = 0;
		bool _isWaiting = { false };
		float _attackWatingTime = AFTER_ATTACK_WATING_TIME;
		bool _useUltimateSkill{ false }; //이건 Invoke에 들어가야 한다.

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

		bool _isIceAttackStartEligible{ true };
		bool _isFireAttackStartEligible{ true };
		float _isStartedIceSkillChargeTime{ 0.f };
		float _isStartedFireSkillChargeTime{ 0.f };
		bool _isIceAttackingNow{ false };
		bool _isFireAttackingNow{ false };

		//단발성이 아닌, Strong Attack / Ultimate Attack을 위해 값 보관을 하려 쓰는 값들.
		bool _isJustUltimateAttackInvoked{ false };
		bool _isJustStrongAttackInvoked{ false };
		//float _startedUltimateAttackingTime{ 0.f };
		float _startedStrongAttackingTime{ 0.f };

	private:
		//실제 로직에서 활용되지 않는다. 그저, Check시리즈의 값을 전해줄 뿐.
		bool _checkStrongAttack{ false };
		bool _checkUltimateAttack{ false };
		bool _checkFireAttack{ false };
		bool _checkIceAttack{ false };

	private:
		Pg::API::Input::PgInput* _pgInput;
		Pg::API::Time::PgTime* _pgTime;
	};
}


