#include "StubBehaviour.h"
#include "CameraShake.h"
#include "BaseEnemyHandler.h"
#include "TotalGameManager.h"

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
		ScriptInterface(obj), _distance(0.f), _attackCount(0), _isRotateFinish(false)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//���� ü�°� ����
		_stubInfo = new StubInfo(43.f, 2.f);

		///���� ��� �� �ǰ��ൿ�� CombatSystem���� ������ �޺��� ��ų�� ����
		///���Ϳ��� ���������� �����ϱ⿡ ���⼭�� ��� �� �ൿ�� ����� �ȴ�.
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

	void StubBehaviour::GrabManagedObjects()
	{
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);

		//�׷��ͱ�� ���� �����̸� �ȵǹǷ� ���� Freeze
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearX(true);
		_collider->FreezeLinearZ(true);

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_meshRenderer->SetRendererOffset(_rendererOffset);

		//�÷��̾� ����
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		//AudioSource ������Ʈ ������
		auto stubHit = _object->GetScene()->FindObjectWithName("TrentHitSound");
		_hitSound = stubHit->GetComponent<Pg::Data::AudioSource>();

		auto stubDie = _object->GetScene()->FindObjectWithName("TrentDieSound");
		_dieSound = stubDie->GetComponent<Pg::Data::AudioSource>();

		auto stubSkill = _object->GetScene()->FindObjectWithName("TrentSkillSound");
		_skillSound = stubSkill->GetComponent<Pg::Data::AudioSource>();

		auto stubAttack = _object->GetScene()->FindObjectWithName("TrentAttackSound");
		_attackSound = stubAttack->GetComponent<Pg::Data::AudioSource>();

		auto stubFind = _object->GetScene()->FindObjectWithName("TrentFindSound");
		_findSound = stubFind->GetComponent<Pg::Data::AudioSource>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
			else if (childTag == "TAG_Skill")
			{
				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);  // ���Ϳ� �߰�
					skillStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
		}
	}

	void StubBehaviour::BeforePhysicsAwake()
	{
		//_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);

		//�׷��ͱ�� ���� �����̸� �ȵǹǷ� ���� Freeze
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);
		_collider->FreezeLinearX(true);
		_collider->FreezeLinearZ(true);
		//_collider->SetCapsuleInfo(1.f, 1.f);

		_meshRenderer->SetRendererOffset(_rendererOffset);

		//clear �ʿ���.
		if (!_basicAttackCol.empty() || !_skillAttackCol.empty())
		{
			_basicAttackCol.clear();
			_skillAttackCol.clear();
		}

		for (auto& iter : _object->_transform.GetChildren())
		{
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
			else if (childTag == "TAG_Skill")
			{
				Pg::Data::StaticBoxCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);  // ���Ϳ� �߰�
					skillStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
		}
	}

	void StubBehaviour::Awake()
	{
		//ü�°� �⺻ ���ݷ��� �������ش�.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);

		{
			TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
			HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
			this->_enemyHandler = tHB->_enemyHandler;
			assert(_enemyHandler != nullptr);
		}
	}

	void StubBehaviour::Start()
	{
		_stubInfo->StartBaseMonsterLogic();
	}

	void StubBehaviour::Update()
	{
		//�ʱ⿣ Idle����.
		Idle();

		_distance = std::abs(std::sqrt(std::pow(_playerTransform->_position.x - _object->_transform._position.x, 2)
			+ std::pow(_playerTransform->_position.z - _object->_transform._position.z, 2)));

		//���� ���� �ȿ� ������ ��
		if (_distance <= _stubInfo->GetAttackRange())
		{
			_monsterHelper->_isPlayerinHitSpace = true;
			RotateToPlayer(_playerTransform->_position);

			//���� ���.
			if (!_isFindSoundPlaying)
			{
				_findSound->Play();
				_isFindSoundPlaying = true;
			}

			if (_monsterHelper->_stubFlag._stubState == Pg::Data::StubState::SKILL_ATTACK)
			{
				Skill(true); // ��ų ���
				
				if (!_isSkillSoundPlaying)
				{
					_skillSound->Play();
					_isSkillSoundPlaying = true;
				}
			}
			if (_monsterHelper->_stubFlag._stubState == Pg::Data::StubState::SKILL_COOLDOWN)
			{
				Skill(false); // ��ų ����
				_isSkillSoundPlaying = false;
			}
			if (_monsterHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_1 ||
				_monsterHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_2 ||
				_monsterHelper->_stubFlag._stubState == Pg::Data::StubState::BASIC_ATTACK_3)
			{
				//PG_TRACE("Attack!");
				Attack(_monsterHelper->_isAnimChange);

				if (!_isAttackSoundPlaying)
				{
					_attackSound->Play();
					_isAttackSoundPlaying = true;
				}

				//�ִϸ��̼��� ��
				if (!_monsterHelper->_isAnimChange) _isAnimStartInit = false;
				
				//�ִϸ��̼� ����
				if(!_isAnimStartInit && _monsterHelper->_isAnimChange)
				{
					_isAttackSoundPlaying = false;
					_isAnimStartInit = true;
				}
			}
			if (_monsterHelper->_stubFlag._stubState == Pg::Data::StubState::BASICATTACK_COOLDOWN)
			{
				//PG_TRACE("Attack CoolDown!");
				Attack(false);
			}
		}
		else
		{
			_monsterHelper->_isPlayerinHitSpace = false;
			_isFindSoundPlaying = false;
			//_isAttackSoundPlaying = false;
			//_isSkillSoundPlaying = false;
			_stubInfo->_status = StubStatus::IDLE;
			//_monsterHelper->_stubState = Pg::Data::StubState::IDLE;
			Attack(false);
		}

		//���Ͱ� ����ϴ� �ִϸ��̼��� ��µ� ��
		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{

			///RayCast���� �����ִ� Collider�� �˻簡 �Ǳ� ������, ������ ������ �����ش�.
			_monsterHelper->_isDead = false;
			_monsterHelper->_isDeadDelay = false;

			_isRotateFinish = true;

			//�׷��ͱ�� ������ �浹�� ������.
			_meshRenderer->SetActive(false);
			_collider->SetActive(false);
			_object->SetActive(false);
		}

		//PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));
		_stubInfo->UpdateBaseMonsterLogic(_object);
	}

	void StubBehaviour::Idle()
	{
		//�ִϸ��̼� ������.
	}

	void StubBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;
		_cameraShake->CauseShake(0.25f);
		_hitSound->Play();

		if (_monsterHelper->_stubFlag._stubState != Pg::Data::StubState::IDLE ||
			_monsterHelper->_stubFlag._stubState != Pg::Data::StubState::BASICATTACK_COOLDOWN ||
			_monsterHelper->_stubFlag._stubState != Pg::Data::StubState::SKILL_COOLDOWN) return;


		//�ǰ� �ִϸ��̼� ���� ��.
		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00002.pganim");

		_meshRenderer->SetAnimation(animId, false);
	}

	void StubBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// �÷��̾� ��ġ�� y���� �ޱ�.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//����ȭ.
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
		//���¸� �������� ����.
		PG_TRACE("Dead.");
		_dieSound->Play();
		_stubInfo->_status = StubStatus::DEAD;
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerinHitSpace = false;

		_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);
	}

	void StubBehaviour::ResetAll()
	{
		//���� ������ ���� ī��Ʈ
		_attackCount = 0;

		_isRotateFinish = false;
		_isFindSoundPlaying = false;

		//�浹��ü ���� �ʱ�ȭ
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

		// �ִϸ��̼� ���� ���� �ʱ�ȭ
		_monsterHelper->Reset();
	}
}