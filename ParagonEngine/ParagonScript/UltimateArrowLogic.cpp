#include "UltimateArrowLogic.h"

#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticSphereCollider.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/PhysicsCollision.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgTween.h"
#include "../ParagonUtil/Log.h"

#include "BaseMonster.h"
#include "IEnemyBehaviour.h"
#include "PlayerHandler.h"
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

	void UltimateArrowLogic::BeforePhysicsAwake()
	{
		//CombatSystem 받아오자.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
		_collider = _object->GetComponent<Pg::Data::StaticSphereCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES);
	}

	void UltimateArrowLogic::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_meshRenderer != nullptr);
		_meshRenderer->SetActive(false);
	}

	void UltimateArrowLogic::Start()
	{
		_comboSystem = ComboSystem::GetInstance();
	}

	void UltimateArrowLogic::Update()
	{
		CarryOutShoot();
	}

	void UltimateArrowLogic::CarryOutShoot()
	{
		if (_isSkillStart)
		{
			///여기에 궁극기 애니메이션 및 콜라이더 추가되면 된다.
			_collider->SetActive(true);
			_meshRenderer->SetActive(true);
		}
	}

	void UltimateArrowLogic::EndShootingSelf()
	{
		//_collider->SetActive(false);
		//_meshRenderer->SetActive(false);

		//오브젝트 다시 초기위치로 돌아가야 함.
		_object->_transform._position = { 0.f,10.f,5.f };
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
				_combatSystem->AddMonsterHitList(tEnemyBehaviour->ReturnBaseMonsterInfo(), -(ARROW_ATTACK_POWER));
				_combatSystem->AddMonsterOnHitList(tEnemyBehaviour->ReturnBaseMonsterInfo());
			}
			else
			{
				//ComboSystem한테 때렸는데 몬스터가 아니었다고 전달.
				_comboSystem->HitObject(false);
			}
		}
	}
}