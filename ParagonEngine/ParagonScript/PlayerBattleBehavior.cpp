#include "PlayerBattleBehavior.h"
#include "CombatSystem.h"

//일어날 수 있는 Event들의 리스트
#include "EventList_PlayerRelated.h"

#include <algorithm>
#include <limits>

namespace Pg::DataScript
{
	PlayerBattleBehavior::PlayerBattleBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		
	}

	void PlayerBattleBehavior::Awake()
	{
		//무조건 생성자는 안됨! -> AddComponent에서 만들어진 다음에는, Static Variable Initialization에 따라 재생성되지 않는다.
		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void PlayerBattleBehavior::Start()
	{

	}

	void PlayerBattleBehavior::Update()
	{

	}

	void PlayerBattleBehavior::LateUpdate()
	{

	}

	void PlayerBattleBehavior::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{
		if (e.GetIdentifier() == Event_PlayerDeath::_identifier);
		{
			//여러 개의 이벤트들이 한꺼번에 핸들링 될 경우, 이렇게 활용됨. 
			//const Event_PlayerDeath& demoEvent = static_cast<const Event_PlayerDeath&>(e);



		}
	}

	void PlayerBattleBehavior::ChangePlayerHealth(float level)
	{
		healthPoint = std::clamp<float>(healthPoint + level, 0.0f, MAX_PLAYER_HEALTH);

		if (healthPoint > std::numeric_limits<float>::epsilon() &&
			healthPoint < PLAYER_HEALTH_LOW_BARRIER)
		{
			_combatSystem->Post(Event_PlayerOnLowHealth(), _object, healthPoint);
		}
		else if(healthPoint < std::numeric_limits<float>::epsilon())
		{
			//만약 체력이 0일 경우, 
			_combatSystem->Post(Event_PlayerDeath(), _object, 0.0f);
		}

	}

	void PlayerBattleBehavior::ChangePlayerMana(float level)
	{
		manaPoint = std::clamp<float>(manaPoint + level, 0.0f, MAX_PLAYER_MANA);
	}

	void PlayerBattleBehavior::ChangePlayerStamina(float level)
	{
		staminaPoint = std::clamp<float>(staminaPoint + level, 0.0f, MAX_PLAYER_STAMINA);
	}

	void PlayerBattleBehavior::ResetAll()
	{

	}

}