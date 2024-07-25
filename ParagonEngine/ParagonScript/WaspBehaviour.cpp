#include "WaspBehaviour.h"
#include "CameraShake.h"
#include "WaspAttack.h"
#include "WaspSkillAttack.h"
#include "BaseEnemyHandler.h"

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
#include "../ParagonData/StaticMeshRenderer.h"
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
		_waspInfo = new WaspInfo(35.f, 1.f);

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

	void WaspBehaviour::GrabManagedObjects()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		//assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeLinearY(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		Pg::Data::GameObject* _waspMove = _object->GetScene()->FindObjectWithName("WaspMoveSound");
		_moveSound = _waspMove->GetComponent<Pg::Data::AudioSource>();

		auto waspAttack1 = _object->GetScene()->FindObjectWithName("WaspAttackSound1");
		_attackSound_1 = waspAttack1->GetComponent<Pg::Data::AudioSource>();


		_waspSkillAttack = _object->GetScene()->FindObjectWithName("WaspAttackSound2");
		_attackSound_2 = _waspSkillAttack->GetComponent<Pg::Data::AudioSource>();

		auto _waspHit = _object->GetScene()->FindObjectWithName("WaspHitSound");
		_hitSound = _waspHit->GetComponent<Pg::Data::AudioSource>();

		auto _waspDie = _object->GetScene()->FindObjectWithName("WaspDieSound");
		_dieSound = _waspDie->GetComponent<Pg::Data::AudioSource>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				_waspAttackScript = iter->_object->GetComponent<WaspAttack>();

				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childTag == "TAG_Skill")
			{
				_waspSkillAttackScript = iter->_object->GetComponent<WaspSkillAttack>();

				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}

		//코인 SetActive를 위해
		_corn = _object->GetScene()->FindObjectWithName(_cornName);
		_cornRenderer = _corn->GetComponent<Pg::Data::StaticMeshRenderer>();
		_cornRenderer->SetActive(false);

		//코인 SetActive를 위해
		_skillCorn = _object->GetScene()->FindObjectWithName(_skillCornName);
		_skillCornRenderer = _skillCorn->GetComponent<Pg::Data::StaticMeshRenderer>();
		_skillCornRenderer->SetActive(false);

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Wasp")
			{
				_wingMeshRenderer = iter->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				if (_wingMeshRenderer != nullptr)
				{
					//기본값 설정.
					_wingMeshRenderer->SetAlphaPercentage(50.f);
				}
			}
		}
	}

	void WaspBehaviour::BeforePhysicsAwake()
	{
		//_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		//assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeLinearY(true);
		_collider->FreezeAxisZ(true);

		_cornRenderer->SetActive(false);
		_skillCornRenderer->SetActive(false);

		//clear 필요함.
		if (!_basicAttackCol.empty() || !_skillAttackCol.empty())
		{
			_basicAttackCol.clear();
			_skillAttackCol.clear();
		}

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				_waspAttackScript = iter->_object->GetComponent<WaspAttack>();

				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childTag == "TAG_Skill")
			{
				_waspSkillAttackScript = iter->_object->GetComponent<WaspSkillAttack>();

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
		{
			TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
			HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
			this->_enemyHandler = tHB->_enemyHandler;
			assert(_enemyHandler != nullptr);
		}

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Wasp")
			{
				_wingMeshRenderer = iter->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				if (_wingMeshRenderer != nullptr)
				{
					//기본값 설정.
					_wingMeshRenderer->SetAlphaPercentage(50.f);
				}
			}
		}
	}

	void WaspBehaviour::Start()
	{
		//플레이어 지정
		_player = _object->GetScene()->FindObjectWithName("Player");
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
		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		_waspInfo->StartBaseMonsterLogic();
	}

	void WaspBehaviour::Update()
	{
		_distance = std::abs(std::sqrt(std::pow(_player->_transform._position.x - _object->_transform._position.x, 2)
			+ std::pow(_player->_transform._position.z - _object->_transform._position.z, 2)));

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_meshRenderer->SetActive(false);
			_collider->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDead = false;
			_monsterHelper->_isDeadDelay = false;
		}
		if (_monsterHelper->_isDead) return;

		if (_isRotateToPlayer)
		{
			RotateToPlayer(_playerTransform->_position);

			Chase();
		}

		if (_distance <= _waspInfo->GetSightRange())
		{
			_monsterHelper->_isPlayerDetected = true;
			_isRotateToPlayer = true;
		}

		///로직 (무조건 제일 끝에 존재해야 함)
		UpdateAttack();
		UpdateSkillAttack();

		_waspInfo->UpdateBaseMonsterLogic(_object);
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

			_isMoveSoundPlaying = false;

			//애니메이션 딜레이를 위한 델타타임 체크.
			//_currentAttackTime = _currentAttackTime + _pgTime->GetDeltaTime();
			_monsterHelper->_isPlayerinHitSpace = true;
			_monsterHelper->_isChase = false;

			//_isRotateToPlayer = true;

			if (_monsterHelper->_state == Pg::Data::MonsterState::IDLE)
			{
				_isAttackSoundPlaying = false;
				//_isSkillAttackSoundPlaying = false;
			}

			if (_monsterHelper->_waspFlag._attackCount <= 1)
			{
				_isAttackStart = true;
				_isSkillStart = false;
				_isSkillAttackSoundPlaying = false;

				if (!_isAttackSoundPlaying)
				{
					_attackSound_1->Play();
					_isAttackSoundPlaying = true;
				}
			}
			else
			{
				_isSkillStart = true;
				_isAttackStart = false;
				_isAttackSoundPlaying = false;

				if (!_isSkillAttackSoundPlaying)
				{
					_attackSound_2->Play();
					_isSkillAttackSoundPlaying = true;
				}
			}

			//공격
			//if (_currentAttackTime >= _startAttackTime)
			//{
			//	//if (!_isAttackSoundPlaying)
			//	//{
			//	//	_attackSound->Play();
			//	//	_isAttackSoundPlaying = true;
			//	//}

			//	//Attack(true);
			//}
			//if (_currentAttackTime >= _startAttackTime && _currentAttackTime >= _endAttackTime)
			//{
			//	//Attack(false);
			//	//_isAttackSoundPlaying = false;
			//	//_currentAttackTime = 0.f;
			//}
		}
		else
		{
			//상태를 Chase로 변경.
			_waspInfo->_status = WaspStatus::CHASE;

			_isAttackSoundPlaying = false;
			_isSkillAttackSoundPlaying = false;

			if (!_isMoveSoundPlaying)
			{
				_moveSound->Play();
				_isMoveSoundPlaying = true;
			}

			_isAttackStart = false;
			_isSkillStart = false;

			//스킬 델타타임 초기화
			_waspInfo->SetCurrentAttackTime(1.f);
			_waspInfo->SetCurrentSkillTime(0.f);
			_monsterHelper->_waspFlag._attackCount = 0;

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
		//투사체 처리
		if (_isAttackStart)
		{
			_waspInfo->SetCurrentAttackTime(_waspInfo->GetCurrentAttackTime() + _pgTime->GetDeltaTime());

			if (_waspInfo->GetCurrentAttackTime() > _waspInfo->GetStartAttackTime())
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//자신이 바라보는 방향으로 쏴야하기 때문에 z축빼고 전부 고정.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				if (_waspInfo->GetCurrentAttackTime() < _waspInfo->GetAttackDuration())
				{
					//추적 멈춤
					_isRotateToPlayer = false;

					_cornRenderer->SetActive(true);

					//자신의 rotation에 따라 날아가는 방향 맞춰서 설정.
					if (forwardDir.z > 0)
					{
						for (auto& iter : _basicAttackCol)
						{
							iter->SetActive(true);
							iter->_object->_transform._position.z += forwardDir.z * _waspInfo->GetAttackSpeed() * _pgTime->GetDeltaTime();
						}
					}
					else
					{
						for (auto& iter : _basicAttackCol)
						{
							iter->SetActive(true);
							iter->_object->_transform._position.z -= forwardDir.z * _waspInfo->GetAttackSpeed() * _pgTime->GetDeltaTime();
						}
					}

					//스킬 사용 중에 플레이어한테 맞으면
					if (_waspAttackScript->_isPlayerHit)
					{
						//다 사라져라
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(false);
							iter->_object->_transform._position = { 0.f, 0.f, 1.f };
						}

						_cornRenderer->SetActive(false);
					}
				}
				else
				{
					for (auto& iter : _basicAttackCol)
					{
						iter->SetActive(false);
						iter->_object->_transform._position = { 0.f, 0.f, 1.f };
					}

					_cornRenderer->SetActive(false);

					_isAttackStart = false;
					_isRotateToPlayer = true;
					_waspAttackScript->_isPlayerHit = false;
					_waspInfo->SetCurrentAttackTime(1.f);
				}
			}
		}
	}

	void WaspBehaviour::UpdateSkillAttack()
	{
		//투사체 처리
		if (_isSkillStart)
		{
			_waspInfo->SetCurrentSkillTime(_waspInfo->GetCurrentSkillTime() + _pgTime->GetDeltaTime());

			if (_waspInfo->GetCurrentSkillTime() > _waspInfo->GetStartSkillTime())
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//자신이 바라보는 방향으로 쏴야하기 때문에 z축빼고 전부 고정.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				if (_waspInfo->GetCurrentSkillTime() < _waspInfo->GetSkillDuration())
				{
					//추적 멈춤
					_isRotateToPlayer = false;

					_skillCornRenderer->SetActive(true);

					//자신의 rotation에 따라 날아가는 방향 맞춰서 설정.
					if (forwardDir.z > 0)
					{
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(true);
							iter->_object->_transform._position.z += forwardDir.z * _waspInfo->GetSkillSpeed() * _pgTime->GetDeltaTime();
						}
					}
					else
					{
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(true);
							iter->_object->_transform._position.z -= forwardDir.z * _waspInfo->GetSkillSpeed() * _pgTime->GetDeltaTime();
						}
					}

					//스킬 사용 중에 플레이어한테 맞으면
					if (_waspSkillAttackScript->_isPlayerHit)
					{
						//다 사라져라
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(false);
							iter->_object->_transform._position = { 0.f, 0.f, 1.f };
						}

						_skillCornRenderer->SetActive(false);
						//_waspAttackScript->_isPlayerHit = false;
					}
				}
				else
				{
					for (auto& iter : _skillAttackCol)
					{
						iter->SetActive(false);
						iter->_object->_transform._position = { 0.f, 0.f, 1.f };
					}

					_skillCornRenderer->SetActive(false);

					_isAttackStart = false;
					_isRotateToPlayer = true;
					_waspSkillAttackScript->_isPlayerHit = false;
					_waspInfo->SetCurrentSkillTime(0.f);
				}
			}
		}
	}

	void WaspBehaviour::Hit()
	{
		_hitSound->Play();
		_cameraShake->CauseShake(0.25f);
	
		if (_monsterHelper->_isDead) return;

		if (_monsterHelper->_state != Pg::Data::MonsterState::IDLE ||
			_monsterHelper->_state != Pg::Data::MonsterState::CHASE) return;

		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00003.pganim");

		_meshRenderer->SetAnimation(animId, false);

		std::string objName = _object->GetName();
		objName = objName.substr(0, objName.rfind("_"));
		objName.append("_Wing");

		auto tchild = _object->_transform.GetChild(objName);
		auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_10003.pganim");
		tcMeshRenderer->SetAnimation(animId, false);
	}

	void WaspBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///플래그를 걸어 돌진의 여부까지 계산하기 위해 세팅.
		//회전이 끝날 때 까지 돌기.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}

	void WaspBehaviour::Dead()
	{
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerDetected = false;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = false;

		_dieSound->Play();
		_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);
	}

	void WaspBehaviour::ResetAll()
	{
		//사운드 관련 변수
		bool _isAttackSoundPlaying = false;

		//공격 관련 변수
		bool _isAttackStart = false;
		bool _isSkillStart = false;
		bool _isRotateToPlayer = false;

		//충돌객체 전부 초기화
		_collider->SetActive(true);
		_meshRenderer->SetActive(true);

		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(false);
		}
		for (auto& iter : _skillAttackCol)
		{
			iter->SetActive(false);
		}

		_distance = 0.f;

		// 애니매이션 관련 전부 초기화
		_monsterHelper->Reset();
	}
}