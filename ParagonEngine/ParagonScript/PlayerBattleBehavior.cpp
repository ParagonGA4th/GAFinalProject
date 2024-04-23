#include "PlayerBattleBehavior.h"
#include "CombatSystem.h"

//일어날 수 있는 Event들의 리스트
#include "Event_PlayerDeath.h"
#include "Event_PlayerHealthChange.h"
#include "Event_PlayerManaChange.h"
#include "Event_PlayerStaminaChange.h"



namespace Pg::DataScript
{
	PlayerBattleBehavior::PlayerBattleBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{

	}

	void PlayerBattleBehavior::Awake()
	{
		//무조건 생성자는 안됨! -> AddComponent에서 만들어진 다음에는, Static Variable Initialization에 따라 재생성되지 않는다.
		//_combatSystem = CombatSystem::GetInstance(nullptr);
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
			const Event_PlayerDeath& demoEvent = static_cast<const Event_PlayerDeath&>(e);
		}
	}

}