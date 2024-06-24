#include "CombatSystem.h"
#include "TotalGameManager.h"

//실제로 CombatSystem을 사용하는 애들의 리스트.
#include "PlayerHandler.h"

//Event들의 리스트.
#include "EventList_PlayerRelated.h"

#include "../ParagonAPI/PgScene.h"
#include <singleton-cpp/singleton.h>
#include <cassert>

namespace Pg::DataScript
{
	void CombatSystem::OnSceneChange_Global(Pg::Data::Scene* changedScene)
	{
		if (!_isManagingInitializeCalled)
		{
			Initialize(changedScene);
			_isManagingInitializeCalled = true;
		}

		//Priority Index 덕분에 가장 마지막에 호출되었을 것이다.
		//얘가 하고 있어야 하는 것 :
		//Scene별로 Player의 리스트, Monster의 리스트를 받아야 한다.
		//이는 EnemyHandler / PlayerHandler이기에, 별도로 값을 가져와야.

		//NullPtr일 경우 동작 안 할 것.
		_currentHandlerBundle3D = TotalGameManager::GetInstance(nullptr)->GetCurrentHandlerBundle();

		//리셋은 TotalGameManager에서 이미 되었음.

		//이제, 다시 값을 가져와야 한다. 
		//이미 

	}

	void CombatSystem::Start()
	{
		
	}

	void CombatSystem::Update()
	{
		if (_currentHandlerBundle3D != nullptr)
		{
			CalculateMonsterDamages();
			CalculateMonsterHit();
		}
	}

	void CombatSystem::FixedUpdate()
	{
		if (_currentHandlerBundle3D != nullptr)
		{

		}
	}

	void CombatSystem::LateUpdate()
	{
		if (_currentHandlerBundle3D != nullptr)
		{

		}
	}

	void CombatSystem::ResetAll()
	{
		//옵저버 클리어.
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

	void CombatSystem::RegisterPlayer(PlayerHandler* player)
	{
		//구현체에 따라서 어떤 이벤트들을 등록하고 삭제시켜야 할지, 정해야 한다.
		//_identifier가 이벤트 추가에 따라 늘어날 것이기에, 여러 개를 Subscribe해야 할 것이다.
		Subscribe(Event_PlayerDeath::_identifier,
			std::bind(&PlayerHandler::HandleEvents, player, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));

		Subscribe(Event_PlayerOnLowHealth::_identifier,
			std::bind(&PlayerHandler::HandleEvents, player, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));
	}

	void CombatSystem::RegisterSingleEnemy(IEnemyBehaviour* enemy)
	{

	}

	void CombatSystem::RegisterSingleProjectile(IProjectile* proj)
	{

	}

	void CombatSystem::ChangePlayerHealth(float level)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerHealth(level);
	}

	void CombatSystem::ChangePlayerMana(float level)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerMana(level);
	}

	void CombatSystem::ChangePlayerStamina(float level)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerStamina(level);
	}

	void CombatSystem::AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl)
	{
		_monsterHealthChangeList.push_back(BaseMonsterHealthChangePair(monster, healthChangeLvl));
	}

	void CombatSystem::AddMonsterOnHitList(BaseMonsterInfo* monster)
	{
		_monsterOnHitList.push_back(BaseMonsterHitPair(monster));
	}

	
	void CombatSystem::CalculateMonsterDamages()
	{
		//SceneSystem 함수는 무조건 Physics의 On시리즈보다 빨리 호출된다는 것을 활용.
		if (_monsterHealthChangeList.empty())
		{
			return;
		}

		//실제로 
		for (auto& it : _monsterHealthChangeList)
		{
			it._baseMonster->ChangeMonsterHp(it._healthChangeLvl);

			if (it._baseMonster->GetMonsterHp() <= std::numeric_limits<float>::epsilon())
			{
				it._baseMonster->_onDead();
			}
		}

		//이제 클리어.
		_monsterHealthChangeList.clear();
	}

	void CombatSystem::CalculateMonsterHit()
	{
		//SceneSystem 함수는 무조건 Physics의 On시리즈보다 빨리 호출된다는 것을 활용.
		if (_monsterOnHitList.empty())
		{
			return;
		}

		//몬스터가 피격 시 Hit 함수 호출
		for (auto& it : _monsterOnHitList)
		{
			it._baseMonster->_onHit();
		}

		_monsterOnHitList.clear();
	}

	void CombatSystem::Initialize(Pg::Data::Scene* changedScene)
	{
		// 반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");

		_pgScene = &singleton<Pg::API::PgScene>();
	}

	

}