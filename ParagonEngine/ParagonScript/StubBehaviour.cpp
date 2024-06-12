#include "StubBehaviour.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <limits>
#include <algorithm>

namespace Pg::DataScript
{
	StubBehaviour::StubBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _distance(0.f), _currentAttackTime(0.f), _startAttackTime(1.f), _endAttackTime(2.7f)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//골렘의 체력과 공격
		_stubInfo = new StubInfo(10.f, 2.f);

		///골렘의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_stubInfo->_onDead = [this]() { Dead(); };

		_stubInfo->_onHit = [this]() { Hit(); };
	}

	void StubBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearY(true);
	}

	void StubBehaviour::Awake()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		//체력과 기본 공격력을 설정해준다.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);
	}

	void StubBehaviour::Start()
	{
		//플레이어 지정
		_player = _pgScene->GetCurrentScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();

			if (staticCol != nullptr)
			{
				_attackCol.push_back(staticCol);
				staticCol->SetActive(false);
			}
		}
	}

	void StubBehaviour::Update()
	{
		//초기엔 Idle상태.

		Idle();

		_distance = std::abs(std::sqrt(std::pow(_playerTransform->_position.x - _object->_transform._position.x, 2)
			+ std::pow(_playerTransform->_position.z - _object->_transform._position.z, 2)));

		// 시야 안에 들어왔을 때.
		if (_distance <= _stubInfo->GetSightRange())
		{
			_monsterHelper->_isPlayerDetected = true;
			RotateToPlayer(_playerTransform->_position);

			//공격 범위 안에 들어왔을 때
			if (_distance <= _stubInfo->GetAttackRange())
			{
				Attack(true);
			}
			else
			{
				Attack(false);
			}

			//대쉬 true면 돌진해!!
			//if (_isDash)
			//{
			//	Dash();
			//}
			//else
			//{
			//	_monsterHelper->_isChase = !_isDash;
			//}

		}
		//시야에서 벗어나면 돌진 초기화
		else
		{
			//_isDash = false;
			//_hasDashed = false;
		}


		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };
			_monsterHelper->_isDeadDelay = false;
		}

		//PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));
	}

	void StubBehaviour::Idle()
	{

	}

	void StubBehaviour::Hit()
	{
		PG_TRACE("Hit!");

		//피격 애니메이션 들어가야 함.
		//std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		//animId.append("_00003.pganim");
		//
		//_meshRenderer->SetAnimation(animId, false);
		//
		//std::string objName = _object->GetName();
		//objName = objName.substr(0, objName.rfind("_"));
		//objName.append("_Crtstal");
		////if (objName.find("Golem") != std::string::npos) objName.append("_Crtstal");
		////else objName.append("_Wing");
		//
		//auto tchild = _object->_transform.GetChild(objName);
		//auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		//
		//animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		//animId.append("_10003.pganim");
		//tcMeshRenderer->SetAnimation(animId, false);
	}

	void StubBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}

	void StubBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _attackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void StubBehaviour::Skill()
	{

	}

	void StubBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		_stubInfo->_status = StubStatus::DEAD;
		_monsterHelper->_isDead = true;
	}

	BaseMonsterInfo* StubBehaviour::ReturnBaseMonsterInfo()
	{
		return _stubInfo;
	}
}