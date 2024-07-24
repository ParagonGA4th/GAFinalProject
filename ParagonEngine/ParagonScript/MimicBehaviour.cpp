#include "MimicBehaviour.h"
#include "CameraShake.h"
#include "MimicSkillAttack.h"
#include "BaseEnemyHandler.h"
#include "TotalGameManager.h"
#include "PlayerHandler.h"

#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"

#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/PhysicsCollision.h"
#include "../ParagonData/MonsterHelper.h"

#include "../ParagonUtil/Log.h"

#include <singleton-cpp/singleton.h>
#include <limits>
#include <algorithm>

namespace Pg::DataScript
{
	MimicBehaviour::MimicBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false),
		_distance(0.f), _isDash(false), _hasDashed(false), _currentAttackTime(0.f), _startAttackTime(1.f), _endAttackTime(2.7f),
		_respawnPos(0.f, 0.f, 0.f)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//미믹의 체력과 공격
		_mimicInfo = new MimicInfo(100.f, 1.f);

		///미믹의 사망 및 피격행동은 CombatSystem에서 공격의 콤보와 스킬에 따라
		///몬스터에게 직접적으로 적용하기에 여기서는 사망 시 행동만 만들면 된다.
		_mimicInfo->_onDead = [this]() { Dead(); };

		_mimicInfo->_onHit = [this]() { Hit(); };
	}

	void MimicBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void MimicBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void MimicBehaviour::GrabManagedObjects()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		//_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearY(true);

		//플레이어 지정
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		//사운드
		_mimicMoveSound = _object->GetScene()->FindObjectWithName("MimicMoveSound");
		_moveAudio = _mimicMoveSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _mimicHitSound = _object->GetScene()->FindObjectWithName("MimicHitSound");
		_hitAudio = _mimicHitSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _mimicDieSound = _object->GetScene()->FindObjectWithName("MimicDieSound");
		_dieAudio = _mimicDieSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _mimicAttackSound = _object->GetScene()->FindObjectWithName("MimicAttackSound");
		_attackAudio = _mimicAttackSound->GetComponent<Pg::Data::AudioSource>();

		Pg::Data::GameObject* _mimicSkillSound = _object->GetScene()->FindObjectWithName("MimicSkillSound");
		_skillAudio = _mimicSkillSound->GetComponent<Pg::Data::AudioSource>();

		//코인 SetActive를 위해
		_coin = _object->GetScene()->FindObjectWithName(_coinName);
		_coinRenderer = _coin->GetComponent<Pg::Data::StaticMeshRenderer>();
		_coin->SetActive(false);
		_coinRenderer->SetActive(false);

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_meshRenderer->SetRendererOffset(_rendererOffset);
		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

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
				_mimicSkillAttack = iter->_object->GetComponent<MimicSkillAttack>();

				Pg::Data::StaticBoxCollider* skillCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillCol != nullptr)
				{
					_skillAttackCol.push_back(skillCol);
					skillCol->SetActive(false);
				}
			}
		}
	}

	void MimicBehaviour::BeforePhysicsAwake()
	{
		_mimicInfo->StartBaseMonsterLogic();

		//_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		//_collider->SetCapsuleInfo(1.f, 1.f);
		_collider->FreezeAxisX(true);
		//_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearY(true);

		//코인 SetActive를 위해
		_coin->SetActive(false);
		_coinRenderer->SetActive(false);

		_meshRenderer->SetActive(false);
		_meshRenderer->SetRendererOffset(_rendererOffset);
		_collider->SetActive(false);

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
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childTag == "TAG_Skill")
			{
				_mimicSkillAttack = iter->_object->GetComponent<MimicSkillAttack>();

				Pg::Data::StaticBoxCollider* skillCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillCol != nullptr)
				{
					_skillAttackCol.push_back(skillCol);
					skillCol->SetActive(false);
				}
			}
		}



	}

	void MimicBehaviour::Awake()
	{
		//체력과 기본 공격력을 설정해준다.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);
	}

	void MimicBehaviour::Start()
	{
		this->SetActive(false);
	}

	void MimicBehaviour::Update()
	{
		if (!_mimicInfo->IsEffectValid())
		{
			_mimicInfo->StartBaseMonsterLogic();
		}

		auto plVec = _player;
		auto plTrans = plVec->_transform;

		_distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//다 꺼짐.
			_collider->SetActive(false);
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 지정된 위치로 보내준다.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDead = true;
			_monsterHelper->_isDeadDelay = false;
		}
		if (_monsterHelper->_isDead) return;

		_monsterHelper->_isPlayerDetected = true;

		_monsterHelper->_isChase = true;

		if (_isRotateToPlayer)
		{
			RotateToPlayer(_playerTransform->_position);
		}

		Chase();

		//코인 투척 스킬
		UpdateSkill();

		//PG_TRACE(_monsterHelper->_isDistanceClose);
		_mimicInfo->UpdateBaseMonsterLogic(_object);
	}

	void MimicBehaviour::Idle()
	{

	}

	void MimicBehaviour::Chase()
	{
		//이동 속도 조절.
		float interpolation = _mimicInfo->GetMoveSpeed() * _mimicInfo->GetMonsterSpeedRatio() * _pgTime->GetDeltaTime();

		//일정 사정거리 안에 들어오면
		if (_distance <= _mimicInfo->GetAttackRange())
		{
			//상태 변경.
			_mimicInfo->_status = MimicStatus::BASIC_ATTACK;
			_moveAudio->Stop();

			//애니메이션 딜레이를 위한 델타타임 체크.
			_currentAttackTime = _currentAttackTime + _pgTime->GetDeltaTime();

			_isMoving = false;
			_isRotateToPlayer = true;
			_useCoinThrow = false;

			// 공격 애니메이션 출력.
			_monsterHelper->_isPlayerinHitSpace = true;
			_monsterHelper->_isDistanceClose = true;
			_monsterHelper->_isChase = false;

			//코인 다시 원래 위치로.
			for (auto& iter : _skillAttackCol)
			{
				iter->SetActive(false);
				iter->_object->_transform._position = { 0.f, 1.f, 2.f };
			}

			_coinRenderer->SetActive(false);

			//공격
			if (_currentAttackTime >= _startAttackTime)
			{
				Attack(true);

				if (!_isAttackSoundPlaying)
				{
					_attackAudio->Play();
					_isAttackSoundPlaying = true;
				}
			}
			if (_currentAttackTime >= _startAttackTime && _currentAttackTime >= _endAttackTime)
			{
				Attack(false);
				_isAttackSoundPlaying = false;
				_currentAttackTime = 0.f;
			}

		}
		else if (_distance >= _distance <= _mimicInfo->GetAttackRange() &&
			_distance <= _mimicInfo->GetSkillAttackRange())
		{
			_useCoinThrow = true;
			_monsterHelper->_isChase = false;
			_monsterHelper->_isDistanceClose = false;
			_moveAudio->Stop();

			Attack(false);
		}
		else
		{
			//상태를 Chase로 변경.
			_mimicInfo->_status = MimicStatus::CHASE;

			Attack(false);
			_isRotateToPlayer = true;
			_useCoinThrow = false;  
			_currentAttackTime = 0.f;

			// 플레이어가 시야 안에 있으면
			_monsterHelper->_isPlayerinHitSpace = false;
			_monsterHelper->_isChase = true;

			//사정거리 밖이면 플레이어로 계속 다가가기.
			Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
			_object->_transform._position.x = tPosition.x;
			_object->_transform._position.z = tPosition.z;

			//코인 다시 원래 위치로.
			for (auto& iter : _skillAttackCol)
			{
				iter->SetActive(false);
				iter->_object->_transform._position = { 0.f, 1.f, 2.f };
			}

			_coinRenderer->SetActive(false);

			//사운드 재생
			if (!_isMoving)
			{
				_moveAudio->Play();
				_isMoving = true;
			}
		}
	}

	void MimicBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;
		//카메라 흔들림
		_cameraShake->CauseShake(0.25f);
		_hitAudio->Play();

		if (_monsterHelper->_state != Pg::Data::MonsterState::IDLE ||
			_monsterHelper->_state != Pg::Data::MonsterState::CHASE) return;

		//피격 애니메이션 들어가야 함.
		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00003.pganim");

		_meshRenderer->SetAnimation(animId, false);
	}

	void MimicBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// 플레이어 위치의 y값만 받기.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//정규화.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		//회전이 끝날 때 까지 돌기.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

		_object->_transform._rotation = currentTargetRotation;
	}

	void MimicBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(_isAttack);
		}
	}


	void MimicBehaviour::UpdateSkill()
	{
		//스킬의 이동 및 충돌 처리
		if (_useCoinThrow)
		{
			_mimicInfo->SetCurrentSKillTime(_mimicInfo->GetCurrentSkillTime() + _pgTime->GetDeltaTime());

			if (_mimicInfo->GetCurrentSkillTime() > _mimicInfo->GetStartSkillTime())
			{
				if (!_isSkillSoundPlaying)
				{
					_skillAudio->Play();
					_isSkillSoundPlaying = true;
				}

				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//자신이 바라보는 방향으로 쏴야하기 때문에 z축빼고 전부 고정.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);


				if (_mimicInfo->GetCurrentSkillTime() < _mimicInfo->GetSkillDuration())
				{
					//추적 멈춤
					_isRotateToPlayer = false;

					//자신의 rotation에 따라 날아가는 방향 맞춰서 설정.
					if (forwardDir.z > 0)
					{
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(true);
							iter->_object->_transform._position.z += forwardDir.z * _mimicInfo->GetSkillSpeed() * _pgTime->GetDeltaTime();
						}
					}
					else
					{
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(true);
							iter->_object->_transform._position.z -= forwardDir.z * _mimicInfo->GetSkillSpeed() * _pgTime->GetDeltaTime();
						}
					}

					_coinRenderer->SetActive(true);

					//스킬 사용 중에 플레이어한테 맞으면
					if (_mimicSkillAttack->_isPlayerHit)
					{
						//다 사라져라
						for (auto& iter : _skillAttackCol)
						{
							iter->SetActive(false);
							iter->_object->_transform._position = { 0.f, 1.f, 2.f };
						}

						_coinRenderer->SetActive(false);
					}
				}
				else
				{
					for (auto& iter : _skillAttackCol)
					{
						iter->SetActive(false);
						iter->_object->_transform._position = { 0.f, 1.f, 2.f };
					}

					_coinRenderer->SetActive(false);
					_isRotateToPlayer = true;
					_isMoving = false;
					_useCoinThrow = false;
					_isSkillSoundPlaying = false;
					_mimicSkillAttack->_isPlayerHit = false;

					_mimicInfo->SetCurrentSKillTime(0.f);
				}
			}
		}
	}

	void MimicBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		_mimicInfo->_status = MimicStatus::DEAD;
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = false;
		_moveAudio->Stop();

		_dieAudio->Play();

		TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
		HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
		this->_enemyHandler = tHB->_enemyHandler;
		assert(_enemyHandler != nullptr);

		if (_object->GetName() == "Mimic1")
		{
			tHB->_playerBehavior->artifactCount++;
		}

		_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);
	}

	void MimicBehaviour::ResetAll()
	{
		//몬스터의 상태
		_isStart = false;
		_isHit = false;
		_isRotateFinish = false;

		_isDash = false;			//돌진 여부
		_hasDashed = false;		//돌진했는지 여부

		_isMoving = false;

		_useCoinThrow = false;

		_isRotateToPlayer = false;

		//충돌객체 전부 초기화
		_meshRenderer->SetActive(false);
		_collider->SetActive(false);
		this->SetActive(false);

		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(false);
		}
		for (auto& iter : _skillAttackCol)
		{
			iter->SetActive(false);
		}

		// 애니매이션 관련 전부 초기화
		_monsterHelper->Reset();
	}
}