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

		//wasp�� ü�°� ����
		_waspInfo = new WaspInfo(35.f, 1.f);

		///wasp�� ��� �� �ǰ��ൿ�� CombatSystem���� ������ �޺��� ��ų�� ����
		///���Ϳ��� ���������� �����ϱ⿡ ���⼭�� ��� �� �ൿ�� ����� �ȴ�.
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
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				_waspAttackScript = iter->_object->GetComponent<WaspAttack>();

				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
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

		//���� SetActive�� ����
		_corn = _object->GetScene()->FindObjectWithName(_cornName);
		_cornRenderer = _corn->GetComponent<Pg::Data::StaticMeshRenderer>();
		_cornRenderer->SetActive(false);

		//���� SetActive�� ����
		_skillCorn = _object->GetScene()->FindObjectWithName(_skillCornName);
		_skillCornRenderer = _skillCorn->GetComponent<Pg::Data::StaticMeshRenderer>();
		_skillCornRenderer->SetActive(false);

		for (auto& iter : _object->_transform.GetChildren())
		{
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Wasp")
			{
				_wingMeshRenderer = iter->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				if (_wingMeshRenderer != nullptr)
				{
					//�⺻�� ����.
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
				_waspAttackScript = iter->_object->GetComponent<WaspAttack>();

				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_basicAttackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
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
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Wasp")
			{
				_wingMeshRenderer = iter->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
				if (_wingMeshRenderer != nullptr)
				{
					//�⺻�� ����.
					_wingMeshRenderer->SetAlphaPercentage(50.f);
				}
			}
		}
	}

	void WaspBehaviour::Start()
	{
		//�÷��̾� ����
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		//AudioSource ������Ʈ ������
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
			//�� ����.
			_meshRenderer->SetActive(false);
			_collider->SetActive(false);
			_object->SetActive(false);

			///RayCast���� �����ִ� Collider�� �˻簡 �Ǳ� ������, ������ ������ ������ ��ġ�� �����ش�.
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

		///���� (������ ���� ���� �����ؾ� ��)
		UpdateAttack();
		UpdateSkillAttack();

		_waspInfo->UpdateBaseMonsterLogic(_object);
	}

	void WaspBehaviour::Chase()
	{
		//�̵� �ӵ� ����.
		float interpolation = _waspInfo->GetMoveSpeed() * _pgTime->GetDeltaTime();

		//���� �����Ÿ� �ȿ� ������
		if (_distance <= _waspInfo->GetAttackRange())
		{
			//���� ����.
			_waspInfo->_status = WaspStatus::BASIC_ATTACK;

			_isMoveSoundPlaying = false;

			//�ִϸ��̼� �����̸� ���� ��ŸŸ�� üũ.
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

			//����
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
			//���¸� Chase�� ����.
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

			//��ų ��ŸŸ�� �ʱ�ȭ
			_waspInfo->SetCurrentAttackTime(1.f);
			_waspInfo->SetCurrentSkillTime(0.f);
			_monsterHelper->_waspFlag._attackCount = 0;

			//Attack(false);
			//���� �ʱ�ȭ
			//_isAttackSoundPlaying = false;
			//_currentAttackTime = 0.f;

			// �÷��̾ �þ� �ȿ� ������
			_monsterHelper->_isPlayerDetected = true;
			_monsterHelper->_isPlayerinHitSpace = false;
			_monsterHelper->_isChase = true;

			//�����Ÿ� ���̸� �÷��̾�� ��� �ٰ�����.
			///�����ϸ鼭 �̵��� �� �������� �������� ������ �߻��ϱ� ���� ����.
			Pg::Math::PGFLOAT3 currentPosition = _object->_transform._position;
			Pg::Math::PGFLOAT3 targetPosition = _playerTransform->_position;

			// ��ǥ ���������� ���� ���� ���
			Pg::Math::PGFLOAT3 direction = targetPosition - currentPosition;
			direction.y = 0; // y�� �̵��� ���� ���� y���� 0���� ����

			// ���� ���͸� ����ȭ
			Pg::Math::PGFLOAT3 directionNorm = Pg::Math::PGFloat3Normalize(direction);

			// ������ �ӵ��� �̵�
			Pg::Math::PGFLOAT3 movement = directionNorm * interpolation;

			currentPosition.x += movement.x;
			currentPosition.z += movement.z;

			_object->_transform._position = currentPosition;
		}
	}

	void WaspBehaviour::UpdateAttack()
	{
		//����ü ó��
		if (_isAttackStart)
		{
			_waspInfo->SetCurrentAttackTime(_waspInfo->GetCurrentAttackTime() + _pgTime->GetDeltaTime());

			if (_waspInfo->GetCurrentAttackTime() > _waspInfo->GetStartAttackTime())
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//�ڽ��� �ٶ󺸴� �������� �����ϱ� ������ z�໩�� ���� ����.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				if (_waspInfo->GetCurrentAttackTime() < _waspInfo->GetAttackDuration())
				{
					//���� ����
					_isRotateToPlayer = false;

					_cornRenderer->SetActive(true);

					//�ڽ��� rotation�� ���� ���ư��� ���� ���缭 ����.
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

					//��ų ��� �߿� �÷��̾����� ������
					if (_waspAttackScript->_isPlayerHit)
					{
						//�� �������
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
		//����ü ó��
		if (_isSkillStart)
		{
			_waspInfo->SetCurrentSkillTime(_waspInfo->GetCurrentSkillTime() + _pgTime->GetDeltaTime());

			if (_waspInfo->GetCurrentSkillTime() > _waspInfo->GetStartSkillTime())
			{
				Pg::Math::PGFLOAT3 forwardDir = Pg::Math::GetForwardVectorFromQuat(_object->_transform._rotation);

				//�ڽ��� �ٶ󺸴� �������� �����ϱ� ������ z�໩�� ���� ����.
				forwardDir.y = 0;
				forwardDir.x = 0;
				forwardDir = Pg::Math::PGFloat3Normalize(forwardDir);

				if (_waspInfo->GetCurrentSkillTime() < _waspInfo->GetSkillDuration())
				{
					//���� ����
					_isRotateToPlayer = false;

					_skillCornRenderer->SetActive(true);

					//�ڽ��� rotation�� ���� ���ư��� ���� ���缭 ����.
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

					//��ų ��� �߿� �÷��̾����� ������
					if (_waspSkillAttackScript->_isPlayerHit)
					{
						//�� �������
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
		// �÷��̾� ��ġ�� y���� �ޱ�.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//����ȭ.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///�÷��׸� �ɾ� ������ ���α��� ����ϱ� ���� ����.
		//ȸ���� ���� �� ���� ����.
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
		//���� ���� ����
		bool _isAttackSoundPlaying = false;

		//���� ���� ����
		bool _isAttackStart = false;
		bool _isSkillStart = false;
		bool _isRotateToPlayer = false;

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