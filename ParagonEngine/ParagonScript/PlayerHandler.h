#pragma once
#include "ScriptInterface.h"
#include "IObserver.h"
#include "IScriptResettable.h"
#include "BaseMonster.h"

namespace Pg::DataScript
{
	class CombatSystem;
	class ComboSystem;

	class ArrowLogic;
	class PlayerMovementSector;
	class PlayerBattleSector;
}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	};

	namespace Time
	{
		class PgTime;
	};
}

namespace Pg::Data
{
	class DynamicCollider;
	class AudioSource;
	class SkinnedMeshRenderer;
}

namespace Pg::DataScript
{
	class PlayerHandler : public ScriptInterface<PlayerHandler>, public IObserver, public IScriptResettable
	{
		DEFINE_PARAGON_SCRIPT(PlayerHandler);

	public:
		//디파인.
		inline static const float MAX_PLAYER_HEALTH = 100.0f;
		inline static const float PLAYER_HEALTH_LOW_BARRIER = 20.0f;
		inline static const float MAX_PLAYER_MANA = 100.0f;
		inline static const float MAX_PLAYER_STAMINA = 100.0f;

	public:
		PlayerHandler(Pg::Data::GameObject* obj); 

		virtual void BeforePhysicsAwake() override;
		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void LateUpdate();

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;
	
		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

		//Animation이 끝났을 때 호출 되는 함수
		virtual void OnAnimationEnd() override;

	private:
		void FindAllArrowsInMap();
		void ArrowShootingLogic();
		void CalculateMonsterDamages();
		void CalculateMonsterHit();

	private:
		void PlayAdequateAnimation();	// 플레이어 행동에 따른 맞는 애니매이션 출력

	public:
		//플레이어에게 들어오는 개별적인 로직은 따로 분리됨.
		void ChangePlayerHealth(float level);
		void ChangePlayerMana(float level);
		void ChangePlayerStamina(float level);

		void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		void AddMonsterOnHitList(BaseMonsterInfo* monster);

		void SetPlayerMoveSpeed(float val);
		float GetPlayerMoveSpeed();

	public:
		//체력 관련. -> 지금은 맵 기믹때문에 이렇게 해놨지만, 나중에는 별도로 이동해야.
		//최고 체력은 일단 100이다.
		float healthPoint{ MAX_PLAYER_HEALTH };
		float manaPoint{ MAX_PLAYER_MANA };
		float staminaPoint{ MAX_PLAYER_STAMINA };

		float _timeSinceLastShot = 0.f;
		//공격 쿨타임
		const float _shootCooldown = 0.8f; 

	private:
		CombatSystem* _combatSystem{ nullptr };
		ComboSystem* _comboSystem{ nullptr };
		

	private:
		//내부적으로 Movement 등 Sector들 관리.
		std::unique_ptr<PlayerMovementSector> _playerMovementSector;
		std::unique_ptr<PlayerBattleSector> _playerBattleSector;
	private:
		//매 프레임마다 clear.
		std::vector<BaseMonsterHealthChangePair> _monsterHealthChangeList;
		std::vector<BaseMonsterHitPair> _monsterOnHitList;

	private:
		Pg::API::Input::PgInput* _pgInput;
		Pg::API::Time::PgTime* _pgTime;
		std::vector<ArrowLogic*> _arrowVec;
		Pg::Data::DynamicCollider* _selfCol{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _meshRenderer{ nullptr };
		PlayerMovementSector* _playerMovement;

	private:
		//사운드 관련 변수
		Pg::Data::GameObject* _commonAttackSound;
		Pg::Data::AudioSource* _commonAttackAudio;


	private:
		// 플레이어 애니매이션 관련 변수
		std::string _prevAnimationInput;
		bool _isHit;
		int _hitCount = 0;
	};
}


