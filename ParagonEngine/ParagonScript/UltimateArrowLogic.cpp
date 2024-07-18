#include "UltimateArrowLogic.h"

#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/PhysicsCollision.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgTween.h"
#include "../ParagonUtil/Log.h"

#include "BaseMonster.h"
#include "IEnemyBehaviour.h"
#include "PlayerHandler.h"
#include "PlayerMovementSector.h"
#include "TotalGameManager.h"
#include "ComboSystem.h"
#include "CombatSystem.h"

#include <cassert>
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	UltimateArrowLogic::UltimateArrowLogic(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
	}

	void UltimateArrowLogic::GrabManagedObjects()
	{
		//
	}

	void UltimateArrowLogic::BeforePhysicsAwake()
	{
		//CombatSystem 받아오자.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
		_collider = _object->GetComponent<Pg::Data::StaticSphereCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES);
		_collider->SetActive(false);

		HandlerBundle3D* tH3d = TotalGameManager::GetInstance(nullptr)->GetHandlerBundleByScene(_object->GetScene());
		_playerHandler = tH3d->_playerBehavior;
	}

	void UltimateArrowLogic::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_meshRenderer != nullptr);
		//_meshRenderer->SetActive(false);
		_meshRenderer->_alphaPercentage = 0.f;


		_meshRenderer->SetAnimation("ult_atk_0.pganim", false);
		_meshRenderer->PauseAnim();
	}

	void UltimateArrowLogic::Start()
	{
		_comboSystem = ComboSystem::GetInstance();
	}

	void UltimateArrowLogic::Update()
	{
		CarryOutShoot();
		EndShootingSelf();
	}

	void UltimateArrowLogic::CarryOutShoot()
	{
		if (_isSkillStart)
		{
			///여기에 궁극기 애니메이션 및 콜라이더 추가되면 된다.
			_meshRenderer->PlayAnim();
			_collider->SetActive(true);
			//_meshRenderer->SetActive(true);
			_meshRenderer->_alphaPercentage = 100.f;
			_playerHandler->GetPlayerMovementSector()->SetUSeUltimateSkill(true);

			//사운드 재생
			_playerHandler->GetUltimateSkillAudio()->Play();

			//플레이어 무적
			_playerHandler->GetPlayerSelfCol()->SetActive(false);
			_isSkillStart = false;
		}
	}

	void UltimateArrowLogic::EndShootingSelf()
	{
		if (_isAnimEnd)
		{
			_meshRenderer->_alphaPercentage -= ALPHA_PERCENT;
			
			if (_meshRenderer->_alphaPercentage <= std::numeric_limits<float>::epsilon())
			{
				_meshRenderer->SetAnimation("ult_atk_0.pganim", false);
				_meshRenderer->PauseAnim();
				_collider->SetActive(false);
				_playerHandler->GetPlayerMovementSector()->SetUSeUltimateSkill(false);

				//플레이어 무적 해제.
				_playerHandler->GetPlayerSelfCol()->SetActive(true);
				_isAnimEnd = false;
				_ultimateSkill = false;
			}
		}
	}

	void UltimateArrowLogic::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		if (_comboSystem == nullptr)
		{
			return;
		}

		//SceneSystem의 함수들은 이거 발생 후 호출된다. 밖의 로직애서 하는 게 맞다.
		for (int i = 0; i < count; i++)
		{
			//무조건 해당 수만큼은 들어온다.
			Pg::Data::Collider* tCol = _colArr[i];

			//Physics Layer로 검사한다.
			//몬스터일 때 설정하는 것이니.
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MONSTER ||
				tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_BOSS)
			{
				//몬스터 때렸다는 것.
				//자신이 직접 데미지를 연산하는 것이 아니다! 
				//기록해서 PlayerBattleBehavior가 처리해 줄 것.
				IEnemyBehaviour* tEnemyBehaviour = tCol->_object->GetComponent<IEnemyBehaviour>();
				assert((tEnemyBehaviour != nullptr) && "무조건 찾았어야 했다");

				//ComboSystem한테 적 때렸다고 전달.
				_comboSystem->HitObject(true);

				//해당 데미지를 입력, PlayerBattleBehavior로 하여금 이를 처리할 수 있게 만든다.
				_combatSystem->AddMonsterHitList(tEnemyBehaviour->ReturnBaseMonsterInfo(), -(ULTIMATE_ATTACK_POWER), ePartialAttackType::eULTIMATE_HIT);
				_combatSystem->AddMonsterOnHitList(tEnemyBehaviour->ReturnBaseMonsterInfo());
			}
			else
			{
				//ComboSystem한테 때렸는데 몬스터가 아니었다고 전달.
				_comboSystem->HitObject(false);
			}
		}
	}

	void UltimateArrowLogic::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_isAnimEnd = true;
	}

	void UltimateArrowLogic::StartSkill()
	{
		_isSkillStart = true;
		_ultimateSkill = true;
	}

	bool UltimateArrowLogic::GetUltimateSkillEnd()
	{
		return _ultimateSkill;
	}


}