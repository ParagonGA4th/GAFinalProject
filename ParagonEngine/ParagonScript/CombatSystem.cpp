#include "CombatSystem.h"
#include "TotalGameManager.h"

//������ CombatSystem�� ����ϴ� �ֵ��� ����Ʈ.
#include "PlayerHandler.h"

//Event���� ����Ʈ.
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

		//Priority Index ���п� ���� �������� ȣ��Ǿ��� ���̴�.
		//�갡 �ϰ� �־�� �ϴ� �� :
		//Scene���� Player�� ����Ʈ, Monster�� ����Ʈ�� �޾ƾ� �Ѵ�.
		//�̴� EnemyHandler / PlayerHandler�̱⿡, ������ ���� �����;�.

		//NullPtr�� ��� ���� �� �� ��.
		_currentHandlerBundle3D = TotalGameManager::GetInstance(nullptr)->GetCurrentHandlerBundle();


	}

	void CombatSystem::Start()
	{
		
	}

	void CombatSystem::Update()
	{
		if (_currentHandlerBundle3D != nullptr)
		{
			//Normal Damage ���. 
			CalculateMonsterDamages();
			CalculateMonsterHit();

			// Ice / Fire Damage ���.
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
		//������ Ŭ����.
		if (!_observers.empty())
		{
			_observers.clear();
		}
	}

	void CombatSystem::Subscribe(const std::string& descriptor, SlotType&& slot, bool isGlobal)
	{
		//������ ���ο� ĭ �߰�.
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

		//���� �ɼ����� ���� �̺�Ʈ���̶�� Early-Return. 
		if (_observers.find(tTypeString) != _observers.end())
		{
			//������ vector subscription range ������ �׾�� �Ѵ�.
			//LOCAL.
			auto&& observers = _observers.at(tTypeString);
			for (auto&& observer : observers)
			{
				//������ �������� �ɼ����� : ȣ��.
				observer(eventVal, usedVariant1, usedVariant2);
			}
		}
		
		if (_globalObservers.find(tTypeString) != _globalObservers.end())
		{
			//GLOBAL
			auto&& g_observers = _globalObservers.at(tTypeString);
			for (auto&& observer : g_observers)
			{
				//������ �������� �ɼ����� : ȣ��.
				observer(eventVal, usedVariant1, usedVariant2);
			}
		}
	}

	void CombatSystem::RegisterPlayer(PlayerHandler* player)
	{
		//����ü�� ���� � �̺�Ʈ���� ����ϰ� �������Ѿ� ����, ���ؾ� �Ѵ�.
		//_identifier�� �̺�Ʈ �߰��� ���� �þ ���̱⿡, ���� ���� Subscribe�ؾ� �� ���̴�.
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

		//�ǰ�
		if (level <= std::numeric_limits<float>::epsilon() && !isTrap)
		{
			_currentHandlerBundle3D->_playerBehavior->_hitAudio->Play();

			//Effect�� �÷���.
			_currentHandlerBundle3D->_playerBehavior->InvokeHitEffect();
		}
		//ȸ��
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
		//SceneSystem �Լ��� ������ Physics�� On�ø���� ���� ȣ��ȴٴ� ���� Ȱ��.
		if (_monsterHealthChangeList.empty())
		{
			return;
		}

		//������ 
		for (auto& it : _monsterHealthChangeList)
		{
			//���⼭ ���� �������� ���� ���ش�.
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

			//������ Dead ȣ��� ������.
			it._baseMonster->ChangeMonsterHp(it._healthChangeLvl);
		}

		//���� Ŭ����.
		_monsterHealthChangeList.clear();
	}

	void CombatSystem::CalculateMonsterHit()
	{
		//SceneSystem �Լ��� ������ Physics�� On�ø���� ���� ȣ��ȴٴ� ���� Ȱ��.
		if (_monsterOnHitList.empty())
		{
			return;
		}

		//���Ͱ� �ǰ� �� Hit �Լ� ȣ��
		for (auto& it : _monsterOnHitList)
		{
			it._baseMonster->_onHit();
		}

		_monsterOnHitList.clear();
	}

	void CombatSystem::CalculateIceDamages()
	{
		//������ ����.
		if (_iceMonsterHealthChangeList.empty())
		{
			return;
		}

		//�ð��� ���缭 ������ ������ ��Ƽ ������ ���� �ؾ� �ϱ⿡,
		std::list<IceEffect_MonsterHitPair>::iterator tIter = _iceMonsterHealthChangeList.begin();
		float dt = _pgTime->GetDeltaTime();

		//���� ������.
		while (tIter != _iceMonsterHealthChangeList.end())
		{
			//0���� ���� �ð��� ������ ���� CUT.
			bool tTimeLowerThanZero = (tIter->_remainingTime < 0);
			bool tIsMonsterDead = tIter->_baseMonster->IsMonsterDead();

			//�����.
			if (tTimeLowerThanZero || tIsMonsterDead)
			{
				//����� ����, Speed ������� ������� �Ѵ�.
				tIter->_baseMonster->SetMonsterSpeedRatio(IceEffect_MonsterHitPair::NORMAL_SPEED_MULTIPLIER);
				//�����.
				tIter = _iceMonsterHealthChangeList.erase(tIter);
			}
			else
			{
				//��ŸŸ�Ӹ�ŭ ���̱�.
				tIter->_remainingTime -= dt;
				//tIter : Speed ���� �ʿ�. �׳� �� ������ ����.
				tIter->_baseMonster->SetMonsterSpeedRatio(IceEffect_MonsterHitPair::SLOW_SPEED_MULTIPLIER);
				//���� Iter��.
				++tIter;
			}
		}
	}

	void CombatSystem::CalculateFireDamages()
	{
		//������ ����.
		if (_fireMonsterHealthChangeList.empty())
		{
			return;
		}

		//�ð��� ���缭 ������ ������ ��Ƽ ������ ���� �ؾ� �ϱ⿡,
		std::list<FireEffect_MonsterHitPair>::iterator tIter = _fireMonsterHealthChangeList.begin();
		float dt = _pgTime->GetDeltaTime();

		//���� ������.
		while (tIter != _fireMonsterHealthChangeList.end())
		{
			//0���� ���� �ð��� ������ ���� CUT.
			bool tTimeLowerThanZero = (tIter->_remainingTime < 0);
			bool tIsMonsterDead = tIter->_baseMonster->IsMonsterDead();

			//�����.
			if (tTimeLowerThanZero || tIsMonsterDead)
			{
				tIter = _fireMonsterHealthChangeList.erase(tIter);
			}
			else
			{
				//��ŸŸ�Ӹ�ŭ ���̱�.
				tIter->_remainingTime -= dt;
				//true�� ������Ʈ�ؾ� �Ѵ�. �� �����ؾ�.
				if (tIter->_remainingTime - (float)(tIter->_roundingNum) < 0)
				{
					//������ ������ ����ϴ� ����.
					PG_WARN("Fire DAMAGE");

					//OnDead�� Change ���ο� ȣ�⿡ �� ���̴�. ����, �ߺ��Ǹ� �� ���� ��.
					//�̸� HealthChange���� ����� ������ ����� ���̱� ����.
					tIter->_baseMonster->ChangeMonsterHp(-FireEffect_MonsterHitPair::DOT_DAMAGE);

					//Fire Invoke.
					tIter->_baseMonster->OnHitEnableHitEffect(1);

					//Rounding Time �ϳ� ���̳ʽ�.
					assert(tIter->_roundingNum >= 0); //���� �� �ɸ��� �ȵ�.
					--(tIter->_roundingNum);
				}

				//���� Iter��.
				++tIter;
			}
		}
	}
	void CombatSystem::Initialize(Pg::Data::Scene* changedScene)
	{
		// �ݵ�� �ش� Object�� Don't Destroy On Load ������ �Ǿ� �־�� �Ѵ�.
		assert(_object->GetDontDestroyOnLoad() && "XML���� �̷��� ���Ծ�� �Ѵ�");

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