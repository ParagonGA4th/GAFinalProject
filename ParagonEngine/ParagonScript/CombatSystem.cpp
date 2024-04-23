#include "CombatSystem.h"

//실제로 CombatSystem을 사용하는 애들의 리스트.
#include "PlayerBattleBehavior.h"

//Event들의 리스트.
#include "Event_PlayerDeath.h"
#include "Event_PlayerHealthChange.h"
#include "Event_PlayerManaChange.h"
#include "Event_PlayerStaminaChange.h"

namespace Pg::DataScript
{
	//CombatSystem::CombatSystem(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	//{
	//
	//}

	void CombatSystem::Start()
	{
		
	}

	void CombatSystem::FixedUpdate()
	{

	}

	void CombatSystem::ResetAll()
	{
		//다시 재사용해야 할 경우, 값을 활용.
		_player = nullptr;

		if (!_monsterList.empty())
		{
			_monsterList.clear();
		}
		if (!_projectileList.empty())
		{
			_projectileList.clear();
		}
		if (!_observers.empty())
		{
			_observers.clear();
		}
	}

	void CombatSystem::Subscribe(const std::string& descriptor, SlotType&& slot)
	{
		_observers[descriptor].push_back(slot);
	}

	void CombatSystem::Post(const IEvent& eventVal, UsedVariant usedVariant1, UsedVariant usedVariant2) const
	{
		std::string tTypeString = std::string(eventVal.GetIdentifier());

		//아직 옵서버가 없는 이벤트들이라면 Early-Return. 
		if (_observers.find(tTypeString) == _observers.end())
		{
			return;
		}
		
		//없으면 vector subscription range 문제로 죽어야 한다.
		auto&& observers = _observers.at(tTypeString);

		for (auto&& observer : observers)
		{
			//실제로 관찰당한 옵서버들 : 호출.
			observer(eventVal, usedVariant1, usedVariant2);
		}
	}

	void CombatSystem::RegisterPlayer(Pg::Data::GameObject* obj)
	{
		PlayerBattleBehavior* tPlayer = obj->GetComponent<PlayerBattleBehavior>();
		assert(tPlayer != nullptr);
		_player = tPlayer;

		//구현체에 따라서 어떤 이벤트들을 등록하고 삭제시켜야 할지, 정해야 한다.
		//_identifier가 이벤트 추가에 따라 늘어날 것이기에, 여러 개를 Subscribe해야 할 것이다.
		Subscribe(Event_PlayerDeath::_identifier,
			std::bind(&PlayerBattleBehavior::HandleEvents, _player, std::placeholders::_1, 
				std::placeholders::_2, std::placeholders::_3));

		Subscribe(Event_PlayerHealthChange::_identifier,
			std::bind(&PlayerBattleBehavior::HandleEvents, _player, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		Subscribe(Event_PlayerManaChange::_identifier,
			std::bind(&PlayerBattleBehavior::HandleEvents, _player, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		Subscribe(Event_PlayerStaminaChange::_identifier,
			std::bind(&PlayerBattleBehavior::HandleEvents, _player, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));
	}

	void CombatSystem::RegisterSingleMonster(Pg::Data::GameObject* obj)
	{

	}

	void CombatSystem::RegisterProjectiles(Pg::Data::GameObject* obj)
	{

	}

	

}