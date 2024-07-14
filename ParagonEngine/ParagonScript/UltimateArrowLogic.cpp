#include "UltimateArrowLogic.h"

#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
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
		_collider = _object->GetComponent<Pg::Data::StaticBoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES);
	}

	void UltimateArrowLogic::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_meshRenderer != nullptr);
	}

	void UltimateArrowLogic::Start()
	{
		_comboSystem = ComboSystem::GetInstance();
	}

	void UltimateArrowLogic::Update()
	{
		CarryOutShoot();
	}

	bool UltimateArrowLogic::GetIsNowShooting()
	{
		return _isNowShooting;
	}

	void UltimateArrowLogic::CarryOutShoot()
	{
		if (_isSkillStart)
		{
			Pg::Util::Tween* tTween = _pgTween->CreateTween();

			Pg::Math::PGFLOAT3 fallPosition = _object->_transform._position;
			fallPosition.y -= 5.f;
			//Tween 작동.
			tTween->GetData(&(_object->_transform._position))
				.DoMove(fallPosition, 1.f)
				.SetEase(Pg::Util::Enums::eEasingMode::INQUART)
				.OnComplete([this]()
					{
						//_isSkillEnd = true;
						_isSkillStart = false;
						EndShootingSelf();
					});
		}
		if (_isSkillEnd)
		{
			float dTime = 0.f;
			dTime += _pgTime->GetDeltaTime();

			if (dTime >= 1.f)
			{
				
				dTime = 0.f;

				_isSkillEnd = false;

			}
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

				int tComboIndex = std::clamp<int>(_comboSystem->GetComboCount(), 1, ComboSystem::MAXIMUM_HIT_COUNT);
				tComboIndex -= 1; //무조건 ComboCount가 1 / 2 / 3 당 0, 1, 2를 각각 반환하게 설정하는 것이다. 인덱스 이슈. 

				//실제 충돌을 한 것이니, Collider와 Renderer를 끄자!
				_meshRenderer->SetActive(false);
				_collider->SetActive(false);

				//해당 데미지를 입력, PlayerBattleBehavior로 하여금 이를 처리할 수 있게 만든다.
				_combatSystem->AddMonsterHitList(tEnemyBehaviour->ReturnBaseMonsterInfo(), -(ARROW_ATTACK_POWER * ComboSystem::DAMAGE_MULTIPLIER[tComboIndex]));
				_combatSystem->AddMonsterOnHitList(tEnemyBehaviour->ReturnBaseMonsterInfo());

				{
					std::string tComboStr = "ComboCount : ";
					tComboStr += std::to_string(_comboSystem->GetComboCount());
					tComboStr += " // ";
					PG_TRACE(tComboStr.c_str());
				}
			}
			else
			{
				//ComboSystem한테 때렸는데 몬스터가 아니었다고 전달.
				_comboSystem->HitObject(false);
			}
		}
	}
}