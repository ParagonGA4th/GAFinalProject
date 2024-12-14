#include "CombatSystem.h"
#include "TotalGameManager.h"

//실제로 CombatSystem을 사용하는 애들의 리스트.
#include "PlayerHandler.h"

//Event들의 리스트.
#include "EventList_PlayerRelated.h"

#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonUtil/Log.h"
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


	}

	void CombatSystem::Start()
	{
		
	}

	void CombatSystem::Update()
	{
		if (_currentHandlerBundle3D != nullptr)
		{
			//Normal Damage 계산. 
			CalculateMonsterDamages();
			CalculateMonsterHit();

			// Ice / Fire Damage 계산.
			CalculateFireDamages();
			CalculateIceDamages();
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

	void CombatSystem::Subscribe(const std::string& descriptor, SlotType&& slot, bool isGlobal)
	{
		//없으면 새로운 칸 추가.
		_observers.try_emplace(descriptor, std::vector<SlotType>());
		_globalObservers.try_emplace(descriptor, std::vector<SlotType>());

		if (isGlobal)
		{
			_globalObservers.at(descriptor).push_back(slot);
		}
		else
		{
			_observers.at(descriptor).push_back(slot);
		}
	}

	void CombatSystem::Post(const IEvent& eventVal, UsedVariant usedVariant1, UsedVariant usedVariant2) const
	{
		std::string tTypeString = std::string(eventVal.GetIdentifier());

		//아직 옵서버가 없는 이벤트들이라면 Early-Return. 
		if (_observers.find(tTypeString) != _observers.end())
		{
			//없으면 vector subscription range 문제로 죽어야 한다.
			//LOCAL.
			auto&& observers = _observers.at(tTypeString);
			for (auto&& observer : observers)
			{
				//실제로 관찰당한 옵서버들 : 호출.
				observer(eventVal, usedVariant1, usedVariant2);
			}
		}
		
		if (_globalObservers.find(tTypeString) != _globalObservers.end())
		{
			//GLOBAL
			auto&& g_observers = _globalObservers.at(tTypeString);
			for (auto&& observer : g_observers)
			{
				//실제로 관찰당한 옵서버들 : 호출.
				observer(eventVal, usedVariant1, usedVariant2);
			}
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

		Subscribe(Event_PlayerGetArtifact::_identifier,
			std::bind(&PlayerHandler::HandleEvents, player, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3));
	}

	void CombatSystem::RegisterSingleEnemy(IEnemyBehaviour* enemy)
	{

	}

	void CombatSystem::RegisterSingleProjectile(IProjectile* proj)
	{

	}

	void CombatSystem::ChangePlayerHealth(float level, bool isTrap)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerHealth(level, isTrap);

		//피격
		if (level <= std::numeric_limits<float>::epsilon() && !isTrap)
		{
			_currentHandlerBundle3D->_playerBehavior->_hitAudio->Play();

			//Effect도 플레이.
			_currentHandlerBundle3D->_playerBehavior->InvokeHitEffect();
		}
		//회복
		else
		{
			//_currentHandlerBundle3D->_playerBehavior->;
		}
	}

	void CombatSystem::ChangePlayerMana(float level)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerMana(level);
	}

	void CombatSystem::ChangePlayerStamina(float level)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerStamina(level);
	}

	void CombatSystem::ChangePlayerLife(int level)
	{
		_currentHandlerBundle3D->_playerBehavior->ChangePlayerLife(level);
	}

	void CombatSystem::AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl, ePartialAttackType partType)
	{
		_monsterHealthChangeList.push_back(BaseMonsterHealthChangePair(monster, healthChangeLvl, partType));
	}

	void CombatSystem::AddMonsterOnHitList(BaseMonsterInfo* monster)
	{
		_monsterOnHitList.push_back(BaseMonsterHitPair(monster));
	}

	void CombatSystem::AddMonsterIceDamageList(BaseMonsterInfo* monster)
	{
		_iceMonsterHealthChangeList.push_back(IceEffect_MonsterHitPair(monster));
	}

	void CombatSystem::AddMonsterFireDamageList(BaseMonsterInfo* monster)
	{
		_fireMonsterHealthChangeList.push_back(FireEffect_MonsterHitPair(monster));
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
			//여기서 스탯 충전까지 같이 해준다.
			switch (it._partialAttackType)
			{
				case ePartialAttackType::eNORMAL_HIT: [[fallthrough]];
				case ePartialAttackType::eSTRONG_HIT:
				{
					ChangePlayerMana(INCREMENT_MANA_ONHIT_AMT);
				}
				break;
				case ePartialAttackType::eULTIMATE_HIT:
				{
					//
				}
				break;
			}

			//안으로 Dead 호출부 움직임.
			it._baseMonster->ChangeMonsterHp(it._healthChangeLvl);
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

	void CombatSystem::CalculateIceDamages()
	{
		//없으면 리턴.
		if (_iceMonsterHealthChangeList.empty())
		{
			return;
		}

		//시간에 맞춰서 데미지 연산을 멀티 프레임 동안 해야 하기에,
		std::list<IceEffect_MonsterHitPair>::iterator tIter = _iceMonsterHealthChangeList.begin();
		float dt = _pgTime->GetDeltaTime();

		//끝날 때까지.
		while (tIter != _iceMonsterHealthChangeList.end())
		{
			//0보다 남은 시간이 줄으면 수명 CUT.
			bool tTimeLowerThanZero = (tIter->_remainingTime < 0);
			bool tIsMonsterDead = tIter->_baseMonster->IsMonsterDead();

			//지우기.
			if (tTimeLowerThanZero || tIsMonsterDead)
			{
				//지우기 전에, Speed 원래대로 돌려줘야 한다.
				tIter->_baseMonster->SetMonsterSpeedRatio(IceEffect_MonsterHitPair::NORMAL_SPEED_MULTIPLIER);
				//지우기.
				tIter = _iceMonsterHealthChangeList.erase(tIter);
			}
			else
			{
				//델타타임만큼 줄이기.
				tIter->_remainingTime -= dt;
				//tIter : Speed 세팅 필요. 그냥 매 프레임 세팅.
				tIter->_baseMonster->SetMonsterSpeedRatio(IceEffect_MonsterHitPair::SLOW_SPEED_MULTIPLIER);
				//다음 Iter로.
				++tIter;
			}
		}
	}

	void CombatSystem::CalculateFireDamages()
	{
		//없으면 리턴.
		if (_fireMonsterHealthChangeList.empty())
		{
			return;
		}

		//시간에 맞춰서 데미지 연산을 멀티 프레임 동안 해야 하기에,
		std::list<FireEffect_MonsterHitPair>::iterator tIter = _fireMonsterHealthChangeList.begin();
		float dt = _pgTime->GetDeltaTime();

		//끝날 때까지.
		while (tIter != _fireMonsterHealthChangeList.end())
		{
			//0보다 남은 시간이 줄으면 수명 CUT.
			bool tTimeLowerThanZero = (tIter->_remainingTime < 0);
			bool tIsMonsterDead = tIter->_baseMonster->IsMonsterDead();

			//지우기.
			if (tTimeLowerThanZero || tIsMonsterDead)
			{
				tIter = _fireMonsterHealthChangeList.erase(tIter);
			}
			else
			{
				//델타타임만큼 줄이기.
				tIter->_remainingTime -= dt;
				//true면 업데이트해야 한다. 값 세팅해야.
				if (tIter->_remainingTime - (float)(tIter->_roundingNum) < 0)
				{
					//실제로 데미지 계산하는 로직.
					PG_WARN("Fire DAMAGE");

					//OnDead는 Change 내부에 호출에 될 것이다. 또한, 중복되면 안 들어올 것.
					//미리 HealthChange에서 비슷한 로직이 실행될 것이기 때문.
					tIter->_baseMonster->ChangeMonsterHp(-FireEffect_MonsterHitPair::DOT_DAMAGE);

					//Fire Invoke.
					tIter->_baseMonster->OnHitEnableHitEffect(1);

					//Rounding Time 하나 마이너스.
					assert(tIter->_roundingNum >= 0); //로직 상 걸리면 안됨.
					--(tIter->_roundingNum);
				}

				//다음 Iter로.
				++tIter;
			}
		}
	}
	void CombatSystem::Initialize(Pg::Data::Scene* changedScene)
	{
		// 반드시 해당 Object는 Don't Destroy On Load 설정이 되어 있어야 한다.
		assert(_object->GetDontDestroyOnLoad() && "XML에서 이렇게 들어왔어야 한다");

		_pgScene = &singleton<Pg::API::PgScene>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
	}

	float CombatSystem::GetPlayerHealth()
	{
		return _currentHandlerBundle3D->_playerBehavior->healthPoint;
	}

	float CombatSystem::GetPlayerMana()
	{
		return _currentHandlerBundle3D->_playerBehavior->manaPoint;
	}

	unsigned int CombatSystem::GetPlayerStamina()
	{
		return _currentHandlerBundle3D->_playerBehavior->staminaPoint;
	}

	

}