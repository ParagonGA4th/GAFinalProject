#include "WaspBehaviour.h"
#include "CameraShake.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"
#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <limits>
#include <algorithm>

#include "TotalGameManager.h"
#include "BaseEnemyHandler.h"

namespace Pg::DataScript
{
	WaspBehaviour::WaspBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//wasp의 체력과 공격
		_waspInfo = new WaspInfo(10.f, 1.f);

		///wasp의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_waspInfo->_onDead = [this]() { Dead(); };

		_waspInfo->_onHit = [this]() { Hit(); };
	}

	void WaspBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void WaspBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void WaspBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childTag == "TAG_Skill")
			{
				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}
	}

	void WaspBehaviour::Awake()
	{

	}

	void WaspBehaviour::Start()
	{
		//플레이어 지정
		_player = _pgScene->GetCurrentScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		//AudioSource 컴포넌트 들고오기
		//_miniGolemHit = _object->GetScene()->FindObjectWithName("MiniGolemHitSound");
		//_hitSound = _miniGolemHit->GetComponent<Pg::Data::AudioSource>();

		//_miniGolemDie = _object->GetScene()->FindObjectWithName("MiniGolemDeadSound");
		//_dieSound = _miniGolemDie->GetComponent<Pg::Data::AudioSource>();

		//_miniGolemDash = _object->GetScene()->FindObjectWithName("MiniGolemDashSound");
		//_dashSound = _miniGolemDash->GetComponent<Pg::Data::AudioSource>();

		//_miniGolemAttack = _object->GetScene()->FindObjectWithName("MiniGolemAttackSound");
		//_attackSound = _miniGolemAttack->GetComponent<Pg::Data::AudioSource>();

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();
	}

	void WaspBehaviour::Update()
	{
		_distance = std::abs(std::sqrt(std::pow(_player->_transform._position.x - _object->_transform._position.x, 2)
			+ std::pow(_player->_transform._position.z - _object->_transform._position.z, 2)));


		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			//_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDead = false;
			_monsterHelper->_isDeadDelay = false;
		}

		///일반공격 로직 (무조건 제일 끝에 존재해야 함)
		UpdateAttack();
		
	}

	void WaspBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _waspInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

		//일정 사정거리 안에 들어오면
		if (_distance <= _waspInfo->GetAttackRange())
		{
			//상태 변경.
			_waspInfo->_status = WaspStatus::BASIC_ATTACK;

			//애니메이션 딜레이를 위한 델타타임 체크.
			_currentAttackTime = _currentAttackTime + _pgTime->GetDeltaTime();

			//공격
			if (_currentAttackTime >= _startAttackTime)
			{
				//if (!_isAttackSoundPlaying)
				//{
				//	_attackSound->Play();
				//	_isAttackSoundPlaying = true;
				//}

				// 공격 애니메이션 출력.
				_monsterHelper->_isPlayerinHitSpace = true;
				_monsterHelper->_isChase = false;
				//Attack(true);
			}
			if (_currentAttackTime >= _startAttackTime && _currentAttackTime >= _endAttackTime)
			{
				//Attack(false);
				//_isAttackSoundPlaying = false;
				//_currentAttackTime = 0.f;
			}
		}
		else
		{
			//상태를 Chase로 변경.
			_waspInfo->_status = WaspStatus::CHASE;

			//Attack(false);
			//사운드 초기화
			//_isAttackSoundPlaying = false;
			//_currentAttackTime = 0.f;

			// 플레이어가 시야 안에 있으면
			_monsterHelper->_isPlayerDetected = true;
			_monsterHelper->_isPlayerinHitSpace = false;
			_monsterHelper->_isChase = true;

			//사정거리 밖이면 플레이어로 계속 다가가기.
			///보간하면서 이동할 시 마지막에 느려지는 현상을 발생하기 위해 제거.
			Pg::Math::PGFLOAT3 currentPosition = _object->_transform._position;
			Pg::Math::PGFLOAT3 targetPosition = _playerTransform->_position;

			// 목표 지점까지의 방향 벡터 계산
			Pg::Math::PGFLOAT3 direction = targetPosition - currentPosition;
			direction.y = 0; // y축 이동을 막기 위해 y값을 0으로 설정

			// 방향 벡터를 정규화
			Pg::Math::PGFLOAT3 directionNorm = Pg::Math::PGFloat3Normalize(direction);

			// 일정한 속도로 이동
			Pg::Math::PGFLOAT3 movement = directionNorm * interpolation;

			currentPosition.x += movement.x;
			currentPosition.z += movement.z;

			_object->_transform._position = currentPosition;
		}
	}

	void WaspBehaviour::UpdateAttack()
	{
		// 돌풍 스킬의 이동 및 충돌 처리
		if (_isAttackStart)
		{
			_startAttackTime += _pgTime->GetDeltaTime();

			if (_startAttackTime < _waspInfo->GetAttackDuration())
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//자신이 바라보는 방향으로 쏴야하기 때문에 z축빼고 전부 고정.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				if (forwardDir.z > 0)
				{
					//돌풍 콜라이더 앞으로 전진
					for (auto& iter : _basicAttackCol)
					{
						iter->SetActive(true);
						iter->_object->_transform._position.z += forwardDir.z * _waspInfo->GetAttackSpeed() * _pgTime->GetDeltaTime();
					}
				}
				else
				{
					//돌풍 콜라이더 앞으로 전진
					for (auto& iter : _basicAttackCol)
					{
						iter->SetActive(true);
						iter->_object->_transform._position.z -= forwardDir.z * _waspInfo->GetAttackSpeed() * _pgTime->GetDeltaTime();
					}
				}
			}
			else
			{
				for (auto& iter : _basicAttackCol)
				{
					iter->SetActive(false);
					iter->_object->_transform._position = { 0.f, 0.f, 2.f };
				}

				_isAttackStart = false;

				//_waspInfo->SetStartWindBlastDurationTime(0.f);
			}
		}
	}

	void WaspBehaviour::Hit()
	{
		//std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		//animId.append("_00003.pganim");

		//_meshRenderer->SetAnimation(animId, false);

		//std::string objName = _object->GetName();
		//objName = objName.substr(0, objName.rfind("_"));
		//objName.append("_Wing");

		//auto tchild = _object->_transform.GetChild(objName);
		//auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		//animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		//animId.append("_10003.pganim");
		//tcMeshRenderer->SetAnimation(animId, false);
	}

	void WaspBehaviour::Dead()
	{
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerDetected = false;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = false;
	}
}