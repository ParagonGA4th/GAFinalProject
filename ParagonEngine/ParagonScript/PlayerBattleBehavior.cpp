#include "PlayerBattleBehavior.h"
#include "CombatSystem.h"
#include "ComboSystem.h"
#include "ArrowLogic.h"

#include "../ParagonData/Scene.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonUtil/Log.h"

//일어날 수 있는 Event들의 리스트
#include "EventList_PlayerRelated.h"

#include <singleton-cpp/singleton.h>

#include <algorithm>
#include <limits>

namespace Pg::DataScript
{
	PlayerBattleBehavior::PlayerBattleBehavior(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgInput = &singleton<Pg::API::Input::PgInput>();
	}

	void PlayerBattleBehavior::BeforePhysicsUpdate()
	{
		static bool tVal = true;

		if (tVal)
		{
			_selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
			assert(_selfCol != nullptr);

			_selfCol->FreezeAxisX(true);
			_selfCol->FreezeAxisZ(true);
			_selfCol->SetMass(2.0f);
			//자기 자신이 Player이니, Collider의 레이어를 설정해준다.
			_selfCol->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PLAYER);

			tVal = false;
		}

	}

	void PlayerBattleBehavior::Awake()
	{
		//무조건 생성자는 안됨! -> AddComponent에서 만들어진 다음에는, Static Variable Initialization에 따라 재생성되지 않는다.
		_combatSystem = CombatSystem::GetInstance(nullptr);
		_comboSystem = ComboSystem::GetInstance();
	}

	void PlayerBattleBehavior::Start()
	{
		_comboSystem->SystemStart();

		FindAllArrowsInMap();

		_commonAttackSound = _object->GetScene()->FindObjectWithName("PlayerCommonAttackSound");
		_commonAttackAudio = _commonAttackSound->GetComponent<Pg::Data::AudioSource>();
	}

	void PlayerBattleBehavior::Update()
	{
		_comboSystem->SystemUpdate();

		ArrowShootingLogic();
		CalculateMonsterDamages();
		CalculateMonsterHit();

		
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
		//assert(false && "not implemented yet");
	}

	void PlayerBattleBehavior::ArrowShootingLogic()
	{
		//마우스 좌클릭 시 공격.
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
		{
			bool tDidShoot = false;

			for (int i = 0; i < _arrowVec.size(); i++)
			{
				//지금 쏘고 있지 않은 컴포넌트만 허용해야.
				if (!(_arrowVec[i]->GetIsNowShooting()))
				{
					using namespace Pg::Math;
					//우리 Forward랑 다른 로직이 된 것 같다. 그러니, Forward를 Rotation을 갖고 Custom으로 구해주자.
					//PlayerBehavior랑 같은 위치. -> 나중에 PhysX 연동은 고쳐져야!

					//시작점 역시 Offset 하기로 했었다.
					Pg::Math::PGFLOAT3 tStartingPosition = _object->_transform._position + Pg::Math::PGFLOAT3(0.f, 2.f, 0.f);


					//Z축 향해 뒤집기. 어디에서 불완전한 연결이 일어나는지는 확인해봐야 할 것 같다.
					Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGReflectVectorAgainstAxis(-_object->_transform.GetForward(), { 0,0,1 });
					tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);

					//Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFloat3Normalize(_object->_transform.GetForward());

					float tDistanceToSpawnFrom = 3.0f;
					//_arrowVec[i]->ShootArrow(_object->_transform._position + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
					_arrowVec[i]->ShootArrow(tStartingPosition + tShouldShootDir * tDistanceToSpawnFrom, tShouldShootDir);
					tDidShoot = true;
					break;
				}
			}

			if (!tDidShoot)
			{
				//쐈어야 하는데 기존의 것 중에서 준비되어 있는 오브젝트가 존재하지 않는다!
				//기존 것 중에서 오래된 요소를 찾아서, (FixedSizeQueue) 여기 기준으로 마지막을 원래대로 돌려야 한다.
				//여튼, 그건 나중에 할 일.
				PG_TRACE("아직 충분히 반환되지 않음. 나중에 FixedSizeQueue로?");
			}

			_commonAttackAudio->Play();
		}
	}

	void PlayerBattleBehavior::FindAllArrowsInMap()
	{
		//ArrowLogic의 Awake에서 자신의 Tag를 이미 "TAG_Arrow"로 설정해놨었을 것이다.
		//모든 Arrow들 받아오기. 한 30개는 되어야 빈틈을 눈치를 못 챌 것이다.
		std::vector<Pg::Data::GameObject*> allObjects = _object->GetScene()->FindObjectsWithTag("TAG_Arrow");
		for (auto& it : allObjects)
		{
			auto tALogic = it->GetComponent<Pg::DataScript::ArrowLogic>();
			if (tALogic != nullptr)
			{
				//Arrow에 자신 할당.
				tALogic->_playerBattleBehavior = this;
				_arrowVec.push_back(tALogic);
			}
		}
	}

	void PlayerBattleBehavior::AddMonsterHitList(BaseMonsterInfo* monster, float healthChangeLvl)
	{
		_monsterHealthChangeList.push_back(BaseMonsterHealthChangePair(monster,healthChangeLvl));
	}


	void PlayerBattleBehavior::AddMonsterOnHitList(BaseMonsterInfo* monster)
	{
		_monsterOnHitList.push_back(BaseMonsterHitPair(monster));
	}

	void PlayerBattleBehavior::CalculateMonsterDamages()
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

	void PlayerBattleBehavior::CalculateMonsterHit()
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
}