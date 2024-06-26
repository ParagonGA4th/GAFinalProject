#include "StubBehaviour.h"
#include "CameraShake.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
#include "../ParagonData/StaticBoxCollider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
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
		,_attackCount(0), _isRotateFinish(false)
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

	void StubBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void StubBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void StubBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);

		//그루터기는 절대 움직이면 안되므로 전부 Freeze
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearX(true);
		_collider->FreezeLinearZ(true);
		//_collider->SetCapsuleInfo(1.f, 1.f);
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

		//AudioSource 컴포넌트 들고오기
		_stubHit = _object->GetScene()->FindObjectWithName("TrentHitSound");
		_hitSound = _stubHit->GetComponent<Pg::Data::AudioSource>();

		_stubDie = _object->GetScene()->FindObjectWithName("TrentDieSound");
		_dieSound = _stubDie->GetComponent<Pg::Data::AudioSource>();

		_stubSkill = _object->GetScene()->FindObjectWithName("TrentSkillSound");
		_skillSound = _stubSkill->GetComponent<Pg::Data::AudioSource>();

		_stubAttack = _object->GetScene()->FindObjectWithName("TrentAttackSound");
		_attackSound = _stubAttack->GetComponent<Pg::Data::AudioSource>();

		_stubFind = _object->GetScene()->FindObjectWithName("TrentFindSound");
		_findSound = _stubFind->GetComponent<Pg::Data::AudioSource>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			// 자식 오브젝트의 이름을 얻어옵니다.
			std::string childName = iter->_object->GetName();

			if (childName == "TrentBasicAttackRange")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // 벡터에 추가
					basicStaticCol->SetActive(false);  // 비활성화
				}
			}
			else if (childName == "TrentSkillAttackRange")
			{
				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);  // 벡터에 추가
					skillStaticCol->SetActive(false);  // 비활성화
				}
			}
		}
	}

	void StubBehaviour::Update()
	{
		//초기엔 Idle상태.
		Idle();

		_distance = std::abs(std::sqrt(std::pow(_playerTransform->_position.x - _object->_transform._position.x, 2)
			+ std::pow(_playerTransform->_position.z - _object->_transform._position.z, 2)));


		//공격 범위 안에 들어왔을 때
		if (_distance <= _stubInfo->GetAttackRange())
		{
			_monsterHelper->_isPlayerinHitSpace = true;
			RotateToPlayer(_playerTransform->_position);

			//사운드 재생.
			if (!_isFindSoundPlaying)
			{
				_findSound->Play();
				_isFindSoundPlaying = true;
			}

			if (_monsterHelper->_trentState == Pg::Data::TrentState::SKILL_ATTACK)
			{
				//PG_TRACE("Skill!");
				Skill(true); // 스킬 사용
			}
			if (_monsterHelper->_trentState == Pg::Data::TrentState::SKILL_COOLDOWN)
			{
				//PG_TRACE("Skill CoolDown!");
				Skill(false); // 스킬 종료
			}
			if (_monsterHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_1 ||
				_monsterHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_2 ||
				_monsterHelper->_trentState == Pg::Data::TrentState::BASIC_ATTACK_3)
			{
				//PG_TRACE("Attack!");
				Attack(_monsterHelper->_isAnimChange);
			}			
			if (_monsterHelper->_trentState == Pg::Data::TrentState::BASICATTACK_COOLDOWN)
			{
				//PG_TRACE("Attack CoolDown!");
				Attack(false);
			}

			//애니메이션 딜레이를 위한 델타타임 체크.
			//_currentAttackTime = _currentAttackTime + _pgTime->GetDeltaTime();

			////공격
			//switch (_stubInfo->_status)
			//{
			//	//Idle 상태일 때, 스킬 공격을 한다.
			//case StubStatus::IDLE:
			//	_stubInfo->_status = StubStatus::SKILL_ATTACK;
			//	_currentAttackTime = 0.f;
			//	break;

			//case StubStatus::SKILL_ATTACK:
			//	if (_currentAttackTime >= 1.f)
			//	{
			//		PG_TRACE("Skill!");
			//		Skill(true); // 스킬 사용
			//		_stubInfo->_status = StubStatus::SKILLCOOLDOWN;
			//		_currentAttackTime = 0.f;
			//	}
			//	break;

			//	//쿨타임
			//case StubStatus::SKILLCOOLDOWN:
			//	if (_currentAttackTime >= 1.f)
			//	{
			//		PG_TRACE("Skill CoolDown!");
			//		Skill(false); // 스킬 종료
			//		_stubInfo->_status = StubStatus::BASIC_ATTACK;
			//		_currentAttackTime = 0.f;
			//	}
			//	break;

			//	//기본 공격
			//case StubStatus::BASIC_ATTACK:
			//	if (_currentAttackTime >= 1.0f) 
			//	{
			//		PG_TRACE("Attack!");
			//		Attack(true);
			//		_attackCount++;
			//		_currentAttackTime = 0.0f;

			//		//3번 공격하면, 쿨타임
			//		if (_attackCount >= 3) {
			//			_stubInfo->_status = StubStatus::BASICCOOLDOWN;
			//			_currentAttackTime = 0.0f;
			//		}
			//	}
			//	break;

			//case StubStatus::BASICCOOLDOWN:
			//	if (_currentAttackTime >= 1.0f) 
			//	{
			//		PG_TRACE("Attack CoolDown!");
			//		Attack(false);
			//		_stubInfo->_status = StubStatus::IDLE;
			//		_currentAttackTime = 0.0f;
			//		_attackCount = 0;
			//	}
			//	break;
			//}
		}
		else
		{
			_monsterHelper->_isPlayerinHitSpace = false;
			_isFindSoundPlaying = false;
			_stubInfo->_status = StubStatus::IDLE;
			_monsterHelper->_trentState = Pg::Data::TrentState::IDLE;
			Attack(false);
		}

		//몬스터가 사망하는 애니메이션이 출력된 후
		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{

			///RayCast에는 꺼져있는 Collider도 검사가 되기 때문에, 임의의 묘지로 보내준다.
			_monsterHelper->_isDeadDelay = false;
			
			_isRotateFinish = true;

			//그루터기는 죽으면 충돌만 꺼야함.
			_collider->SetActive(false);
		}

		//PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));
	}

	void StubBehaviour::Idle()
	{
		//애니메이션 들어가야함.
	}

	void StubBehaviour::Hit()
	{
		PG_TRACE("Hit!");

		_cameraShake->CauseShake(0.25f);
		_hitSound->Play();

		//피격 애니메이션 들어가야 함.
		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00002.pganim");
		
		_meshRenderer->SetAnimation(animId, false);
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

		if (_isRotateFinish == false)
		{
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

			_object->_transform._rotation = currentTargetRotation;
		}		
	}

	void StubBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _basicAttackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void StubBehaviour::Skill(bool _isSkill)
	{
		for (auto& iter : _skillAttackCol)
		{
			iter->SetActive(_isSkill);
		}
	}

	void StubBehaviour::Dead()
	{
		//상태를 죽음으로 변경.
		PG_TRACE("Dead.");
		_dieSound->Play();
		_stubInfo->_status = StubStatus::DEAD;
		_monsterHelper->_isDead = true;
	}
}