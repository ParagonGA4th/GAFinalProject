#include "MiniGolemBehaviour.h"
#include "CameraShake.h"
#include "../ParagonMath/PgMath.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgScene.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/Transform.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Collider.h"
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
	MiniGolemBehaviour::MiniGolemBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false),
		_distance(0.f), _isDash(false), _hasDashed(false), _currentAttackTime(0.f),
		_respawnPos(0.f, 0.f, 0.f), _areaIndex(0)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//���� ü�°� ����
		_miniGolInfo = new MiniGolemInfo(25.f, 1.f);

		///���� ��� �� �ǰ��ൿ�� CombatSystem���� ������ �޺��� ��ų�� ����
		///���Ϳ��� ���������� �����ϱ⿡ ���⼭�� ��� �� �ൿ�� ����� �ȴ�.
		_miniGolInfo->_onDead = [this]() { Dead(); };

		_miniGolInfo->_onHit = [this]() { Hit(); };
	}

	void MiniGolemBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void MiniGolemBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void MiniGolemBehaviour::GrabManagedObjects()
	{
		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::StaticBoxCollider* staticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();

			if (staticCol != nullptr)
			{
				_attackCol.push_back(staticCol);
				staticCol->SetActive(false);
			}
		}

		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();
	}

	void MiniGolemBehaviour::BeforePhysicsAwake()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);


		//clear �ʿ���.
		if (!_attackCol.empty())
		{
			_attackCol.clear();
		}

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

	void MiniGolemBehaviour::Awake()
	{
		//ü�°� �⺻ ���ݷ��� �������ش�.
		//_miniGolInfo->SetMonsterHp(5.f);
		//_miniGolInfo->SetMonsterDamage(1.f);

		//�ڽ��� ���� ���� AreaHandler / EnemyHandler�� �޾ƿ���.
		//�� ���� ���� � ���� ��.
		{
			TotalGameManager* tTotalGameManager = TotalGameManager::GetInstance(nullptr);
			HandlerBundle3D* tHB = tTotalGameManager->GetHandlerBundleByScene(_object->GetScene());
			this->_enemyHandler = tHB->_enemyHandler;
			assert(_enemyHandler != nullptr);
		}
	}

	void MiniGolemBehaviour::Start()
	{
		//�÷��̾� ����
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		//AudioSource ������Ʈ ������
		auto miniGolemHit = _object->GetScene()->FindObjectWithName("MiniGolemHitSound");
		_hitSound = miniGolemHit->GetComponent<Pg::Data::AudioSource>();

		auto miniGolemDie = _object->GetScene()->FindObjectWithName("MiniGolemDeadSound");
		_dieSound = miniGolemDie->GetComponent<Pg::Data::AudioSource>();

		auto miniGolemDash = _object->GetScene()->FindObjectWithName("MiniGolemDashSound");
		_dashSound = miniGolemDash->GetComponent<Pg::Data::AudioSource>();

		auto miniGolemAttack = _object->GetScene()->FindObjectWithName("MiniGolemAttackSound");
		_attackSound = miniGolemAttack->GetComponent<Pg::Data::AudioSource>();

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		_miniGolInfo->StartBaseMonsterLogic();
	}

	void MiniGolemBehaviour::Update()
	{
		auto plVec = _player;
		auto plTrans = plVec->_transform;

		_distance = std::abs(std::sqrt(std::pow(plTrans._position.x - _object->_transform._position.x, 2)
			+ std::pow(plTrans._position.z - _object->_transform._position.z, 2)));

		if (_monsterHelper->_isDeadDelay && _monsterHelper->_isDead)
		{
			//�� ����.
			_meshRenderer->SetActive(false);
			_object->SetActive(false);

			///RayCast���� �����ִ� Collider�� �˻簡 �Ǳ� ������, ������ ������ ������ ��ġ�� �����ش�.
			_object->_transform._position = { 0, -1000, 0 };

			_monsterHelper->_isDeadDelay = false;
			_monsterHelper->_isDead = true;
		}

		// �þ� �ȿ� ������ �� �Ѿư���.
		if (_distance <= _miniGolInfo->GetSightRange())
		{
			_monsterHelper->_isPlayerDetected = true;
			RotateToPlayer(_playerTransform->_position);

			if (_distance <= _miniGolInfo->GetDashRange() && _isDash == false && _hasDashed == false)
			{
				_isDash = true;
				_monsterHelper->_mGolemFlag._isDash = _isDash;
				_monsterHelper->_isChase = !_isDash;
				_miniGolInfo->SetCurrentDashTime(0.f);
			}

			//�뽬 true�� ������!!
			if (_isDash)
			{
				Dash();
			}
			else
			{
				_monsterHelper->_isChase = !_isDash;
				Chase();
			}

		}
		//�þ߿��� ����� ���� �ʱ�ȭ
		else
		{
			_isDash = false;
			_hasDashed = false;
		}
		//PG_TRACE(std::to_string(_miniGolInfo->GetMonsterHp()));

		_miniGolInfo->UpdateBaseMonsterLogic(_object);
	}

	void MiniGolemBehaviour::Idle()
	{

	}

	void MiniGolemBehaviour::Chase()
	{
		//�̵� �ӵ� ����.
		float interpolation = _miniGolInfo->GetMoveSpeed() * _miniGolInfo->GetMonsterSpeedRatio() * _pgTime->GetDeltaTime();

		//���� �����Ÿ� �ȿ� ������
		if (_distance <= _miniGolInfo->GetAttackRange())
		{
			//���� ����.
			_miniGolInfo->_status = MiniGolemStatus::BASIC_ATTACK;

			//�ִϸ��̼� �����̸� ���� ��ŸŸ�� üũ.
			_currentAttackTime = _currentAttackTime + _pgTime->GetDeltaTime();

			//����
			if (_currentAttackTime >= START_ATTACK_TIME)
			{
				if (!_isAttackSoundPlaying) 
				{
					_attackSound->Play();
					_isAttackSoundPlaying = true;
				}

				// ���� �ִϸ��̼� ���.
				_monsterHelper->_isChase = false;
				_monsterHelper->_isPlayerinHitSpace = true;
				Attack(true);
			}
			if (_currentAttackTime >= START_ATTACK_TIME && _currentAttackTime >= END_ATTACK_TIME)
			{
				Attack(false);
				_isAttackSoundPlaying = false;
				_currentAttackTime = 0.f;
			}
		}
		else
		{
			//���¸� Chase�� ����.
			_miniGolInfo->_status = MiniGolemStatus::CHASE;

			Attack(false);
			_currentAttackTime = 0.f;
			//���� �ʱ�ȭ
			_isAttackSoundPlaying = false;

			// �÷��̾ �þ� �ȿ� ������
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

			//Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
			//tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
			//_object->_transform._position.x = tPosition.x;
			//_object->_transform._position.z = tPosition.z;
		}
	}

	void MiniGolemBehaviour::Dash()
	{
		// ���� ���� �ð� ���� ����
		if (_miniGolInfo->GetCurrentDashTime() < _miniGolInfo->GetDashDuration())
		{
			_miniGolInfo->_status = MiniGolemStatus::DASH;

			if (!_isDashSoundPlaying) {
				_dashSound->Play();
				_isDashSoundPlaying = true;
			}

			float interpolation = _miniGolInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_miniGolInfo->SetCurrentDashTime(_miniGolInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			// ���� �� �غ� ���� �ִϸ��̼� ����
			if (_miniGolInfo->GetCurrentDashTime() >= 0.4f)
			{
				Pg::Math::PGFLOAT3 tPosition = _object->_transform._position;
				tPosition = Pg::Math::PGFloat3Lerp(_object->_transform._position, _playerTransform->_position, interpolation);
				_object->_transform._position.x = tPosition.x;
				_object->_transform._position.z = tPosition.z;
			}
		}
		// ������ ������ ���¸� ����
		else
		{
			_isDash = false;
			_hasDashed = true;
			_isDashSoundPlaying = false;
			_monsterHelper->_mGolemFlag._isDash = _isDash;
			_monsterHelper->_isChase = !_isDash;
		}
	}

	void MiniGolemBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;

		_cameraShake->CauseShake(0.25f);
		_hitSound->Play();

		if (_monsterHelper->_state != Pg::Data::MonsterState::IDLE ||
			_monsterHelper->_state != Pg::Data::MonsterState::CHASE) return;

		//�ǰ� �ִϸ��̼� ���� ��.
		std::string animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_00003.pganim");

		_meshRenderer->SetAnimation(animId, false);

		std::string objName = _object->GetName();
		objName = objName.substr(0, objName.rfind("_"));
		objName.append("_Crtstal");

		auto tchild = _object->_transform.GetChild(objName);
		auto tcMeshRenderer = tchild->_object->GetComponent<Pg::Data::SkinnedMeshRenderer>();

		animId = _meshRenderer->GetAnimation().substr(0, _meshRenderer->GetAnimation().find("_"));
		animId.append("_10003.pganim");
		tcMeshRenderer->SetAnimation(animId, false);
	}

	void MiniGolemBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
	{
		// �÷��̾� ��ġ�� y���� �ޱ�.
		Pg::Math::PGFLOAT3 tRotBasePos = targetPos;
		tRotBasePos.y = _object->_transform._position.y;

		Pg::Math::PGFLOAT3 rotatePos = _object->_transform._position - tRotBasePos;

		//����ȭ.
		Pg::Math::PGFLOAT3 rotatePosNorm = Pg::Math::PGFloat3Normalize(rotatePos);

		Pg::Math::PGQuaternion rotateQuat = PGLookRotation(rotatePosNorm, Pg::Math::PGFLOAT3::GlobalUp());

		///�÷��׸� �ɾ� ������ ���α��� ����ϱ� ���� ����.
		if (_isRotateFinish == false)
		{
			//ȸ���� ���� �� ���� ����.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, rotateQuat, std::clamp<float>(0.1f, 0.0f, 1.0f));

			_object->_transform._rotation = currentTargetRotation;
		}
	}

	void MiniGolemBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _attackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void MiniGolemBehaviour::Dead()
	{
		//���¸� �������� ����.
		_miniGolInfo->_status = MiniGolemStatus::DEAD;
		_dieSound->Play();

		//�߰��� ���尡 �Ȳ��� ��츦 ����� �� �� ����.
		_hitSound->Stop();
		_attackSound->Stop();
		_dashSound->Stop();

		_collider->SetActive(false);
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerDetected = false;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = false;
		_monsterHelper->_mGolemFlag._isDash = false;
		_isRotateFinish = true;

		//����, Handler���� �ڽ��� �׾��ٴ� ���� �˷�����.
		_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);
	}

	void MiniGolemBehaviour::ResetAll()
	{
		//������ ����
		_isRotateFinish = false;
		_isDash = false;			//���� ����
		_hasDashed = false;		//�����ߴ��� ����

		_isDashSoundPlaying = false; //���� �Ҹ�
		_isAttackSoundPlaying = false; //���� �Ҹ�

		//�浹��ü ���� �ʱ�ȭ
		_collider->SetActive(true);		
		_meshRenderer->SetActive(true);

		for (auto& iter : _attackCol)
		{
			iter->SetActive(false);
		}

		_distance = 0.f;
		_currentAttackTime = 0.f;

		// �ִϸ��̼� ���� ���� �ʱ�ȭ
		_monsterHelper->Reset();
	}
}