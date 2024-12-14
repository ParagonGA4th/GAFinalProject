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

		//�ڽ��� Scene�� ���� Player Transform�� ��������.
		PlayerHandler* tPlayer = _object->GetScene()->FindSingleComponentInScene<PlayerHandler>();
		_playerTransform = &(tPlayer->_object->_transform);
	}

	void StrongAttackLogic::BeforePhysicsAwake()
	{
		//CombatSystem �޾ƿ���.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//Collision�� ��ŷ�Ǳ� ����, ������ ��������.
		_collider->_rad = 4.0f;
		
		//������ Offset���� �ذ� ���� �� ����. 
		//Rotation Origin�� �޶����� �ϱ� ����.
		//�̸� ������ ���ϰ� �߰��ϴ� ���� ������?
		
		//_collider->SetPositionOffset({ 0, 0, -3.f });
		////MeshRenderer�� Renderer Offset�� �ʿ��ϴ�.
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
			//Position ������Ʈ. ������ ������ null �߸� �ȵ�.
			//+ Forward Direction��ŭ �տ� �־�� �Ѵ�.
			const float FORWARD_FACTOR = 4.0f;

			_object->_transform._rotation = _playerTransform->_rotation;
			Pg::Math::PGFLOAT3 tForwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);
			tForwardDir = Pg::Math::PGFloat3Normalize(tForwardDir);
			tForwardDir = tForwardDir * FORWARD_FACTOR; //�ش� ���⸸ŭ �տ� �ְ�.
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
				//���� 100���� ���������� �׸�.
				_isAppearing = false; 
			}
		}
		else if (_isDisappearing)
		{
			// ������ �ִٰ� ������� �Ѵ�.
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
		//SceneSystem�� �Լ����� �̰� �߻� �� ȣ��ȴ�. ���� �����ּ� �ϴ� �� �´�.
		for (int i = 0; i < count; i++)
		{
			//������ �ش� ����ŭ�� ���´�.
			Pg::Data::Collider* tCol = _colArr[i];

			//Physics Layer�� �˻��Ѵ�.
			//������ �� �����ϴ� ���̴�.
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MONSTER ||
				tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_BOSS)
			{
				//���� ���ȴٴ� ��.
				//�ڽ��� ���� �������� �����ϴ� ���� �ƴϴ�! 
				//����ؼ� PlayerBattleBehavior�� ó���� �� ��.
				IEnemyBehaviour* tEnemyBehaviour = tCol->_object->GetComponent<IEnemyBehaviour>();
				assert((tEnemyBehaviour != nullptr) && "������ ã�Ҿ�� �ߴ�");

				//ComboSystem���� �� ���ȴٰ� ����.
				_comboSystem->HitObject(true);

				//1. �ش� �������� �Է�, PlayerBattleBehavior�� �Ͽ��� �̸� ó���� �� �ְ� �����.
				_combatSystem->AddMonsterHitList(tEnemyBehaviour->ReturnBaseMonsterInfo(), -(STRONG_ATTACK_POWER), ePartialAttackType::eSTRONG_HIT);
				_combatSystem->AddMonsterOnHitList(tEnemyBehaviour->ReturnBaseMonsterInfo());

				//2. �˹��� Register�Ǿ��.
				// Knockback �ֱ�.
				// ������Ʈ�� ���� ����. 
				ApplyKnockback(tCol);
			}
			else
			{
				//ComboSystem���� ���ȴµ� ���Ͱ� �ƴϾ��ٰ� ����.
				_comboSystem->HitObject(false);
			}
		}
	}

	void StrongAttackLogic::ResetSelfState()
	{
		//���̰ų� ���� �Ұ��ϰ�.
		_object->_transform._position = _prisonPos;
		_collider->SetActive(false);
		_meshRenderer->SetAlphaPercentage(0.f);

		_isActivated = false;
	}

	void StrongAttackLogic::Activate(const Pg::Math::PGFLOAT3* basePos)
	{
		_isAppearing = true;
		_isDisappearing = false;

		_isActivated = true; // == ��������� �۵��ǰ� �ִ� �����ΰ�.

		_basePos = basePos; //BasePos ���.
		_collider->SetActive(true);

		//Alpha ���� ����. //0���� 100���� ���� �Ѵ�.
		_meshRenderer->SetAlphaPercentage(0.f);
	}

	void StrongAttackLogic::Deactivate()
	{
		_isDisappearing = true;
		_isAppearing = false;

		_isActivated = false; // == ��������� �۵��ǰ� �ִ� �����ΰ�.

		_basePos = nullptr;
		_previousEndedPosition = _object->_transform._position;
		
		//�ٷ� ��ġ �ٸ� ������.
		_object->_transform._position = _prisonPos;

		//Alpha ���� ����.
		// 100���� 0���� ���� �Ѵ�.
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