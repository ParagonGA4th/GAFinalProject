#pragma once
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/DynamicCollider.h"

#include "IObserver.h"
#include "IScriptResettable.h"

/// <summary>
/// (��) PlayerBattleBehavior�� ����ϴ� ����.
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

		//Monster OnHit : 5�� ������ ��� ����.
		//ex. Monster Wave�� ����, 3������ ������ ��ų �� �� ���� ���̴�.
		//Mana�� 0���� ����.
		inline static const float STRONG_ATTACK_COOLDOWN_TIME = 3.f;
		inline static const float ULTIMATE_ATTACK_COOLDOWN_TIME = 30.f;
		inline static const float ICE_ATTACK_COOLDOWN_TIME = 10.f;
		inline static const float FIRE_ATTACK_COOLDOWN_TIME = 10.f;

		inline static const float ULTIMATE_ATTACK_DURATION = 3.0f;
		inline static const float STRONG_ATTACK_DURATION = 1.0f;

		inline static const float ULTIMATE_ATTACK_REQUIRED_MANA = 100.f;
		inline static const float ICE_ATTACK_REQUIRED_MANA = 30.f;
		inline static const float FIRE_ATTACK_REQUIRED_MANA = 30.f;

		//100 �� 1/5
		inline static const int STRONG_ATTACK_REQUIRED_STAMINA = 1;

		inline static const float AFTER_ATTACK_WATING_TIME = 0.5f;

		// Stamina : 15�ʿ� ��ĭ�� ����. �� 5ĭ. ������ �ð� ������ 0ĭ�̾��.
		// �� �����Ǳ� ������ ���� �� ����.

	public:
		PlayerCombatSector(PlayerHandler* playerHandler);

		void GrabManagedObjects();

		//��׵��� Component�� ���� �۵��Ǵ� �� X, Script ��ü���� ������ �����Ѵ�.
		void BeforePhysicsAwake();
		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void OnAnimationEnd(const std::string& justEndedAnimation);

	public:
		//IObserver : ��ü���� Event ������ �������� �۵��� ��.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. �ٽ� �ڱ� �ڽ��� �����ϴ� �Լ�.
		virtual void ResetAll() override;

	private:
		void AllAttacksLogic();
		void NormalArrowShootingLogic();
		void FindAllArrowsInMap();
		//CombatSystem���� �̵�.
		////Monster Script���� ���������� ȣ���ϴ� �Լ�.
		//void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		//void AddMonsterOnHitList(BaseMonsterInfo* monster);
	private:
		void UpdateForGUIVariables();
		void ProcessInputsForActiveSkills();
		void ProcessInputsForStrongAttack();
		void ProcessInputsForUltimateAttack();

		//��׵��� ���� ��Ű��, ���� ������ ��������.
		//���� ������ ���� �̷������ �Ѵ�. 
		//�����ϸ� True / �����ϸ� False.
		//(�̴� �ٸ� ������ ���� ���� + ���ȿ� ���� ����)
		//���ο��� �ʿ� ���� �Ҹ���� �Ϸ��Ѵ�.

		//üũ�� �Ϸ��ϴ� �ֵ�.
		bool CheckStrongAttack();
		bool CheckUltimateAttack();
		bool CheckFireAttack();
		bool CheckIceAttack();

		bool CheckActivateStrongAttack();
		bool CheckActivateUltimateAttack();
		bool CheckActivateFireAttack();
		bool CheckActivateIceAttack();

		//��� �����ϴ� �Լ���.
		void UpdateExecuteStrongAttack();
		void UpdateExecuteUltimateAttack();
		//Ice/Fire/Normal
		//��� ���ݵ鿡 �����ؼ�, ���Ϳ� ����� ���� ������ �ϰ������� Shoot ����.
		void ExecuteSpecificArrowShoot(std::vector<ArrowLogic*>* typeArrowVec, 
			Pg::Data::AudioSource* audioSource, float& outIfDoneResetTime);
		
	private:
		//������ �� / �μ��� ���θ� ä����� �� �Լ���.
		//�� �ѹ�, ������ ������ �� �� �ܹ������� ȣ��Ǵ� �Լ�.
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
		//Normal�� ����.
		float _normal_timeSinceLastShot = 0.f;
		//���� ��Ÿ��
		const float _normal_shootCooldown = 0.8f;

	private:
		// �÷��̾� �ִϸ��̼� ���� ����
		std::string _prevAnimationInput;
		//bool _isHit;
		//int _hitCount = 0;
		bool _isWaiting = { false };
		float _attackWatingTime = AFTER_ATTACK_WATING_TIME;
		bool _useUltimateSkill{ false }; //�̰� Invoke�� ���� �Ѵ�.

	private:
		//������ ������ ����, Ŭ���� �������� ����Ѵ�.
		float _startedClickingTime{ 0.f };
		bool _isStrongAttackStartEligible{ true };
		float _startedStrongAttackChargeTime{ 0.f };
		bool _isStrongAttackingNow{ false };

		//�ñر� ������ ����, Ŭ���� �������� ����Ѵ�.
		bool _isUltimateAttackStartEligible{ true };
		float _isStartedUltimateAttackChargeTime{ 0.f };
		bool _isUltimateAttackingNow{ false };

		bool _isIceAttackStartEligible{ true };
		bool _isFireAttackStartEligible{ true };
		float _isStartedIceSkillChargeTime{ 0.f };
		float _isStartedFireSkillChargeTime{ 0.f };
		bool _isIceAttackingNow{ false };
		bool _isFireAttackingNow{ false };

		//�ܹ߼��� �ƴ�, Strong Attack / Ultimate Attack�� ���� �� ������ �Ϸ� ���� ����.
		bool _isJustUltimateAttackInvoked{ false };
		bool _isJustStrongAttackInvoked{ false };
		//float _startedUltimateAttackingTime{ 0.f };
		float _startedStrongAttackingTime{ 0.f };

	private:
		//���� �������� Ȱ����� �ʴ´�. ����, Check�ø����� ���� ������ ��.
		bool _checkStrongAttack{ false };
		bool _checkUltimateAttack{ false };
		bool _checkFireAttack{ false };
		bool _checkIceAttack{ false };

	private:
		Pg::API::Input::PgInput* _pgInput;
		Pg::API::Time::PgTime* _pgTime;
	};
}


