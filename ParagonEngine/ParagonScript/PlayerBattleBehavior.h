#pragma once
#include "ScriptInterface.h"
#include "IObserver.h"
#include "IScriptResettable.h"

namespace Pg::DataScript
{
	class CombatSystem;
}

namespace Pg::DataScript
{
	class PlayerBattleBehavior : public ScriptInterface<PlayerBattleBehavior>, public IObserver, public IScriptResettable
	{
		DEFINE_PARAGON_SCRIPT(PlayerBattleBehavior);

	public:
		//디파인.
		inline static const float MAX_PLAYER_HEALTH = 100.0f;
		inline static const float PLAYER_HEALTH_LOW_BARRIER = 20.0f;
		inline static const float MAX_PLAYER_MANA = 100.0f;
		inline static const float MAX_PLAYER_STAMINA = 100.0f;

	public:
		PlayerBattleBehavior(Pg::Data::GameObject* obj); 

		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void LateUpdate();

		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;
	
		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;


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

	private:
		CombatSystem* _combatSystem{ nullptr };
	};
}


