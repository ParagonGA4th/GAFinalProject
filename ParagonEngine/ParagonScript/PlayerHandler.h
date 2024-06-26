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
	class PlayerCombatSector;
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

		friend class CombatSystem;
	public:
		//디파인.
		inline static const float MAX_PLAYER_HEALTH = 100.0f;
		inline static const float PLAYER_HEALTH_LOW_BARRIER = 20.0f;
		inline static const float MAX_PLAYER_MANA = 100.0f;
		inline static const float MAX_PLAYER_STAMINA = 100.0f;

	public:
		PlayerHandler(Pg::Data::GameObject* obj); 

		virtual void GrabManagedObjects() override;

		virtual void BeforePhysicsAwake() override;
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void LateUpdate() override;

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;
	
		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

		//Animation이 끝났을 때 호출 되는 함수
		virtual void OnAnimationEnd(const std::string& justEndedAnimation) override;

		//이거 CombatSystem에 있어야.
		//void AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl);
		//void AddMonsterOnHitList(BaseMonsterInfo* monster);

		//CPP에서는 잔재가 남아있었다.
		virtual void OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count) override;

		void SetPlayerMoveSpeed(float val);
		float GetPlayerMoveSpeed();

	private:
		//플레이어에게 들어오는 개별적인 로직은 따로 분리됨.
		void ChangePlayerHealth(float level);
		void ChangePlayerMana(float level);
		void ChangePlayerStamina(float level);

	public:
		//체력 관련. -> 지금은 맵 기믹때문에 이렇게 해놨지만, 나중에는 별도로 이동해야.
		//최고 체력은 일단 100이다.
		float healthPoint{ MAX_PLAYER_HEALTH };
		float manaPoint{ MAX_PLAYER_MANA };
		float staminaPoint{ MAX_PLAYER_STAMINA };

	public:
		//Sector 사이의 교류를 위해.
		//나중에는 사용 최소화를 목적으로.
		PlayerMovementSector* GetPlayerMovementSector();
		PlayerCombatSector* GetPlayerCombatSector();

	public:
		//GUI를 보여주기 위해, 수정이 불가능한 포인터를 반환하게 한다.
		const float* ReturnPlayerHealthPointPointerConst() const;
		const float* ReturnPlayerManaPointPointerConst() const;
		const float* ReturnPlayerStaminaPointPointerConst() const;

	private:
		CombatSystem* _combatSystem{ nullptr };
		ComboSystem* _comboSystem{ nullptr };
		

	private:
		//내부적으로 Movement 등 Sector들 관리.
		std::unique_ptr<PlayerMovementSector> _playerMovementSector;
		std::unique_ptr<PlayerCombatSector> _playerCombatSector;

		Pg::Data::DynamicCollider* _selfCol{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _meshRenderer{ nullptr };
	};
}


