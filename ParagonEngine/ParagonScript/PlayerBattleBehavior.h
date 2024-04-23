#pragma once
#include "ScriptInterface.h"
#include "IObserver.h"

namespace Pg::DataScript
{
	class CombatSystem;
}

namespace Pg::DataScript
{
	class PlayerBattleBehavior : public ScriptInterface<PlayerBattleBehavior>, public IObserver
	{
		DEFINE_PARAGON_SCRIPT(PlayerBattleBehavior);
	public:
		PlayerBattleBehavior(Pg::Data::GameObject* obj); 

		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void LateUpdate();

		//IObserver
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;
	
	public:
		//체력 관련. -> 지금은 맵 기믹때문에 이렇게 해놨지만, 나중에는 별도로 이동해야.
		float healthPoint{ 100.0f };


	private:
		CombatSystem* _combatSystem{ nullptr };
	};
}


