#include "StrongAttackLogic.h"

#include "../ParagonData/Scene.h"
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
#include "EnemyDefinesAndStructs.h"

#include <cassert>
#include <limits>
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	StrongAttackLogic::StrongAttackLogic(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
	}

	void StrongAttackLogic::GrabManagedObjects()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_collider = _object->GetComponent<Pg::Data::StaticSphereCollider>();
		assert(_collider->GetTrigger());

		//자신의 Scene에 속한 Player Transform을 가져오자.
		PlayerHandler* tPlayer = _object->GetScene()->FindSingleComponentInScene<PlayerHandler>();
		_playerTransform = &(tPlayer->_object->_transform);
	}

	void StrongAttackLogic::BeforePhysicsAwake()
	{
		//CombatSystem 받아오자.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//Collision이 링킹되기 전에, 설정을 조정하자.
		_collider->_rad = 4.0f;
		
		//로직상 Offset으로 해결 못할 것 같다. 
		//Rotation Origin이 달라져야 하기 때문.
		//이를 오히려 급하게 추가하는 것이 나은가?
		
		//_collider->SetPositionOffset({ 0, 0, -3.f });
		////MeshRenderer는 Renderer Offset이 필요하다.
		//_meshRenderer->SetRendererOffset(Pg::Math::PGFLOAT3(0, 0, -3.f));
	}

	void StrongAttackLogic::Awake()
	{
		ResetSelfState();
	}

	void StrongAttackLogic::Start()
	{
		//Start.
		_comboSystem = ComboSystem::GetInstance();
	}

	void StrongAttackLogic::LateUpdate()
	{
		float dt = _pgTime->GetDeltaTime();

		if (_isActivated)
		{
			//Position 업데이트. 로직상 포인터 null 뜨면 안됨.
			//+ Forward Direction만큼 앞에 있어야 한다.
			const float FORWARD_FACTOR = 4.0f;

			_object->_transform._rotation = _playerTransform->_rotation;
			Pg::Math::PGFLOAT3 tForwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
			tForwardDir = Pg::Math::PGFloat3Normalize(tForwardDir);
			tForwardDir = tForwardDir * FORWARD_FACTOR; //해당 방향만큼 앞에 있게.
			Pg::Math::PGFLOAT3 tBasePos = (*_basePos);

			_object->_transform._position = tBasePos + tForwardDir;

			//Minus Position : Rotate Origin.
			//Pg:
			//_meshRenderer->SetRendererRotationOriginOffset(tMinusPos);
		}

		if (_isAppearing)
		{
			//Alpha Fade In.
			_meshRenderer->SetAlphaPercentage(_meshRenderer->GetAlphaPercentage() + (FADEINOUT_SPEED * dt));

			if (_meshRenderer->GetAlphaPercentage() >= 100.f)
			{
				//만약 100까지 도달했으면 그만.
				_isAppearing = false; 
			}
		}
		else if (_isDisappearing)
		{
			// 가만히 있다가 사라져야 한다.
			_meshRenderer->SetAlphaPercentage(_meshRenderer->GetAlphaPercentage() - (FADEINOUT_SPEED * dt));
			if (_meshRenderer->GetAlphaPercentage() <= std::numeric_limits<float>::epsilon())
			{
				_collider->SetActive(false);
				_isDisappearing = false;
			}
		}
		else { /**/ }
	}

	void StrongAttackLogic::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
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

				//1. 해당 데미지를 입력, PlayerBattleBehavior로 하여금 이를 처리할 수 있게 만든다.
				_combatSystem->AddMonsterHitList(tEnemyBehaviour->ReturnBaseMonsterInfo(), -(STRONG_ATTACK_POWER), ePartialAttackType::eSTRONG_HIT);
				_combatSystem->AddMonsterOnHitList(tEnemyBehaviour->ReturnBaseMonsterInfo());

				//2. 넉백이 Register되어야.
				// Knockback 주기.
				// 오브젝트를 서로 뺀다. 
				ApplyKnockback(tCol);
			}
			else
			{
				//ComboSystem한테 때렸는데 몬스터가 아니었다고 전달.
				_comboSystem->HitObject(false);
			}
		}
	}

	void StrongAttackLogic::ResetSelfState()
	{
		//보이거나 교류 불가하게.
		_object->_transform._position = _prisonPos;
		_collider->SetActive(false);
		_meshRenderer->SetAlphaPercentage(0.f);

		_isActivated = false;
	}

	void StrongAttackLogic::Activate(const Pg::Math::PGFLOAT3* basePos)
	{
		_isAppearing = true;
		_isDisappearing = false;

		_isActivated = true; // == 명시적으로 작동되고 있는 상태인가.

		_basePos = basePos; //BasePos 기록.
		_collider->SetActive(true);

		//Alpha 값도 설정. //0에서 100으로 가야 한다.
		_meshRenderer->SetAlphaPercentage(0.f);
	}

	void StrongAttackLogic::Deactivate()
	{
		_isDisappearing = true;
		_isAppearing = false;

		_isActivated = false; // == 명시적으로 작동되고 있는 상태인가.

		_basePos = nullptr;
		_previousEndedPosition = _object->_transform._position;
		
		//바로 위치 다른 곳으로.
		_object->_transform._position = _prisonPos;

		//Alpha 값도 설정.
		// 100에서 0으로 가야 한다.
		_meshRenderer->SetAlphaPercentage(100.f);
	}

	void StrongAttackLogic::ApplyKnockback(Pg::Data::Collider* col)
	{
		//Apply Knockback.
		Pg::Math::PGFLOAT3 tDirection =	Pg::Math::PGFloat3Normalize(col->_object->_transform._position - _object->_transform._position);
		tDirection = tDirection * KNOCKBACK_FACTOR;
		Pg::Data::DynamicCollider* tOther = static_cast<Pg::Data::DynamicCollider*>(col);
		tOther->AddForce(tDirection, Pg::Data::ForceMode::eIMPULSE);
	}
}

//auto& bOtherPos = col->_object->_transform._position;


		//Pg::Util::Tween* tTween = _pgTween->CreateTween();
		//tTween->GetData(&bOtherPos).
		//	DoMove(tTargetPos, KNOCKBACK_DURATION).
		//	SetEase(Pg::Util::Enums::eEasingMode::OUTQUART).
		//	KillEarly(0.6f).OnComplete([this]()
		//		{
		//
		//		});