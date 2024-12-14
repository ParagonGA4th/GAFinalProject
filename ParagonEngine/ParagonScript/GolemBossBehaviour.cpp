#include "GolemBossBehaviour.h"
#include "CameraShake.h"
#include "EventList_GameFlowRelated.h"
#include "CombatSystem.h"
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
#include "../ParagonData/StaticSphereCollider.h"
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
	GolemBossBehaviour::GolemBossBehaviour(Pg::Data::GameObject* obj) :
		ScriptInterface(obj), _isRotateFinish(false),
		_distance(0.f), _isDash(false), _hasDashed(false), _respawnPos(0.f, 0.f, 0.f), _areaIndex(0)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgScene = &singleton<Pg::API::PgScene>();

		//���� ü�°� ����
		_golBossInfo = new GolemBossInfo(MAX_GOLEM_BOSS_HEALTH, GOLEM_BOSS_ATTACK_POWER);

		///���� ��� �� �ǰ��ൿ�� CombatSystem���� ������ �޺��� ��ų�� ����
		///���Ϳ��� ���������� �����ϱ⿡ ���⼭�� ��� �� �ൿ�� ����� �ȴ�.
		_golBossInfo->_onDead = [this]() { Dead(); };

		_golBossInfo->_onHit = [this]() { Hit(); };
	}

	void GolemBossBehaviour::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper(this, sv);
	}

	void GolemBossBehaviour::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper(this, sv);
	}

	void GolemBossBehaviour::GrabManagedObjects()
	{
		_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		_meshRenderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		_meshRenderer->SetRendererOffset(_rendererOffset);
		_monsterHelper = _object->AddComponent<Pg::Data::MonsterHelper>();

		Pg::Data::GameObject* _rush = _object->GetScene()->FindObjectWithName("GolemBossRushSound");
		_rushSound = _rush->GetComponent<Pg::Data::AudioSource>();

		auto _hit = _object->GetScene()->FindObjectWithName("GolemBossHitSound");
		_hitSound = _hit->GetComponent<Pg::Data::AudioSource>();

		auto _die = _object->GetScene()->FindObjectWithName("GolemBossDieSound");
		_dieSound = _die->GetComponent<Pg::Data::AudioSource>();

		auto basicAttackSound = _object->GetScene()->FindObjectWithName("GolemBossAttackSound1");
		_basicAttackSound = basicAttackSound->GetComponent<Pg::Data::AudioSource>();

		auto spinAttackSound = _object->GetScene()->FindObjectWithName("GolemBossAttackSound2");
		_spinAttackSound = spinAttackSound->GetComponent<Pg::Data::AudioSource>();

		for (auto& iter : _object->_transform.GetChildren())
		{
			// �ڽ� ������Ʈ�� �̸��� ���ɴϴ�.
			std::string childTag = iter->_object->GetTag();

			if (childTag == "TAG_Attack")
			{
				Pg::Data::StaticBoxCollider* basicStaticCol = iter->_object->GetComponent<Pg::Data::StaticBoxCollider>();
				if (basicStaticCol != nullptr)
				{
					_attackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
			else if (childTag == "TAG_Skill")
			{
				Pg::Data::StaticSphereCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticSphereCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}
	}

	void GolemBossBehaviour::BeforePhysicsAwake()
	{
		//_collider = _object->GetComponent<Pg::Data::CapsuleCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_MONSTER);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisY(true);
		_collider->FreezeAxisZ(true);

		_meshRenderer->SetRendererOffset(_rendererOffset);

		//clear �ʿ���.
		if (!_attackCol.empty() || !_skillAttackCol.empty())
		{
			_attackCol.clear();
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
					_attackCol.push_back(basicStaticCol);  // ���Ϳ� �߰�
					basicStaticCol->SetActive(false);  // ��Ȱ��ȭ
				}
			}
			else if (childTag == "TAG_Skill")
			{
				Pg::Data::StaticSphereCollider* skillStaticCol = iter->_object->GetComponent<Pg::Data::StaticSphereCollider>();
				if (skillStaticCol != nullptr)
				{
					_skillAttackCol.push_back(skillStaticCol);
					skillStaticCol->SetActive(false);
				}
			}
		}
	}

	void GolemBossBehaviour::Awake()
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

		_combatSystem = CombatSystem::GetInstance(nullptr);
	}

	void GolemBossBehaviour::Start()
	{
		//�÷��̾� ����
		_player = _object->GetScene()->FindObjectWithName("Player");
		_playerTransform = _player->GetComponent<Pg::Data::Transform>();

		//AudioSource ������Ʈ ������

		_cameraShake = _object->GetScene()->FindSingleComponentInScene<Pg::DataScript::CameraShake>();

		_golBossInfo->StartBaseMonsterLogic();
	}

	void GolemBossBehaviour::Update()
	{
		//PG_TRACE(_monsterHelper->_bGolemFlag._bossStateListByEnum[_monsterHelper->_bGolemFlag._bossState]);

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

		Down();
		if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::DOWN) return;

		// �þ� �ȿ� ������ �� �Ѿư���.
		if (_distance <= _golBossInfo->GetSightRange())
		{
			if (_rotateToPlayer) RotateToPlayer(_playerTransform->_position);
			_monsterHelper->_isPlayerDetected = true;
			_rotateToPlayer = true;

			if (_distance <= _golBossInfo->GetDashRange() && _isDash == false && _hasDashed == false)
			{
				_isDash = true;
				_monsterHelper->_mGolemFlag._isDash = _isDash;
				_golBossInfo->SetCurrentDashTime(0.f);
			}

			//�뽬 true�� ������!!
			if (_isDash)
			{
				_rotateToPlayer = false;
				_monsterHelper->_isChase = false;
				Dash();
			}
			else
			{
				//���� �����Ÿ� �ȿ� ������
				if (_distance <= _golBossInfo->GetAttackRange())
				{
					//���� ����.
					_golBossInfo->_status = GolemBossStatus::BASIC_ATTACK;

					//_meshRenderer->_animBlendFactor = 0.0f;

					_monsterHelper->_isChase = false;
					_monsterHelper->_isPlayerinHitSpace = true;

					if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_ATTACK_1 ||
						_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_ATTACK_2)
					{
						if (!_isSpinAttackSoundPlaying)
						{
							_spinAttackSound->Play();
							_isSpinAttackSoundPlaying = true;
						}

						Skill(_monsterHelper->_isAnimChange); // ��ų ���
						if (!_monsterHelper->_isAnimChange) _isSpinAttackSoundPlaying = false;

						Attack(false);
						_isAttackSoundPlaying = false;
					}
					if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::BASIC_ATTACK)
					{
						if (!_isAttackSoundPlaying)
						{
							_basicAttackSound->Play();
							_isAttackSoundPlaying = true;
						}

						Attack(true);
					}
				}

				if (_monsterHelper->_bGolemFlag._bossState == Pg::Data::GolemBossState::SKILL_DASH_ATTACK)
				{
					if (!_isSpinAttackSoundPlaying)
					{
						_spinAttackSound->Play();
						_isSpinAttackSoundPlaying = true;
						_isSpinAttackSoundPlaying = true;
					}

					Skill(true);
					if (_monsterHelper->_isAnimationEnd)
					{
						Skill(false);
						_isSpinAttackSoundPlaying = false;
						_monsterHelper->_isAnimationEnd = false;
					}
				}

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

		_golBossInfo->UpdateBaseMonsterLogic(_object);


		if (_isAnimChange)
		{
			Pg::Data::GameObject* tchild = _object->_transform.GetChild("GolemBoss_Crtstal")->_object;
			Pg::Data::SkinnedMeshRenderer* cskinned = nullptr;

			if (tchild != nullptr) cskinned = tchild->GetComponent<Pg::Data::SkinnedMeshRenderer>();
			if (cskinned != nullptr) cskinned->SetAnimation(animId, animLoop);
		}
	}

	void GolemBossBehaviour::Idle()
	{

	}

	void GolemBossBehaviour::Chase()
	{
		//�̵� �ӵ� ����.
		float interpolation = _golBossInfo->GetMoveSpeed() * _golBossInfo->GetMonsterSpeedRatio() * _pgTime->GetDeltaTime();

		Attack(false);
		//���� �ʱ�ȭ
		_isAttackSoundPlaying = false;

		// ������ 2 �϶��� spin�ϸ鼭 �i�ư�����
		if (_monsterHelper->_bGolemFlag._bossPase == Pg::Data::BossPase::PASE_1)
		{
			_golBossInfo->_status = GolemBossStatus::CHASE;
			_monsterHelper->_isPlayerinHitSpace = false;
			_monsterHelper->_isChase = true;
		}

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

	void GolemBossBehaviour::Dash()
	{
		// ���� ���� �ð� ���� ����
		if (_golBossInfo->GetCurrentDashTime() < _golBossInfo->GetDashDuration())
		{
			_golBossInfo->_status = GolemBossStatus::DASH;

			if (!_isDashSoundPlaying)
			{
				_rushSound->Play();
				_isDashSoundPlaying = true;
			}

			float interpolation = _golBossInfo->GetDashSpeed() * _pgTime->GetDeltaTime();
			_golBossInfo->SetCurrentDashTime(_golBossInfo->GetCurrentDashTime() + _pgTime->GetDeltaTime());

			// ���� �� �غ� ���� �ִϸ��̼� ����
			if (_golBossInfo->GetCurrentDashTime() >= 0.4f)
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

	void GolemBossBehaviour::Hit()
	{
		if (_monsterHelper->_isDead) return;
		_cameraShake->CauseShake(0.25f);
		_hitSound->Play();

		if (_monsterHelper->_bGolemFlag._bossState != Pg::Data::GolemBossState::IDLE ||
			_monsterHelper->_bGolemFlag._bossState != Pg::Data::GolemBossState::CHASE) return;

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

	void GolemBossBehaviour::RotateToPlayer(Pg::Math::PGFLOAT3& targetPos)
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

	void GolemBossBehaviour::Down()
	{
		if (_monsterHelper->_bGolemFlag._bossPase == Pg::Data::BossPase::PASE_1 &&
			_golBossInfo->GetMonsterHp() <= 50.f)
		{
			_golBossInfo->SetCurrentDown(_golBossInfo->GetCurrentDown() + _pgTime->GetDeltaTime());

			if (_isDownInit && _golBossInfo->GetCurrentDown() >= _golBossInfo->GetEndDown())
			{
				//����ȭ ����.
				_isDown = false;
				_monsterHelper->_bGolemFlag._isDown = false;
				_monsterHelper->_bGolemFlag._bossPase = Pg::Data::BossPase::PASE_2;
				//_isRotatingToPlayer = true;
				//_isDownSoundPlaying = false;
				_golBossInfo->SetCurrentDown(0.f);
			}

			if (!_isDownInit)
			{
				_monsterHelper->_bGolemFlag._isDown = true;
				_monsterHelper->_isChase = false;
				_monsterHelper->_isDistanceClose = false;
				_isDownInit = true;
			}
		}
	}

	void GolemBossBehaviour::Attack(bool _isAttack)
	{
		for (auto& iter : _attackCol)
		{
			iter->SetActive(_isAttack);
		}
	}

	void GolemBossBehaviour::Skill(bool _isSkill)
	{
		for (auto& iter : _skillAttackCol)
		{
			iter->SetActive(_isSkill);
		}
	}

	void GolemBossBehaviour::Dead()
	{
		//���¸� �������� ����.
		_golBossInfo->_status = GolemBossStatus::DEAD;
		_dieSound->Play();

		//�߰��� ���尡 �Ȳ��� ��츦 ����� �� �� ����.
		_hitSound->Stop();
		_rushSound->Stop();
		_basicAttackSound->Stop();
		_spinAttackSound->Stop();

		_collider->SetActive(false);
		_monsterHelper->_isDead = true;
		_monsterHelper->_isPlayerDetected = false;
		_monsterHelper->_isPlayerinHitSpace = false;
		_monsterHelper->_isChase = false;
		_monsterHelper->_mGolemFlag._isDash = false;
		_isRotateFinish = true;

		//����, Handler���� �ڽ��� �׾��ٴ� ���� �˷�����.
		_enemyHandler->FromEnemyNotifyDead(_object->GetTag(), this);

		// == Event_EnableJump()�� ���� �Ŷ� ����.
		_combatSystem->Post(Event_EnableJump(), NULL, NULL);
	}

	void GolemBossBehaviour::ResetAll()
	{
		_isRotateFinish = false;
		_isDash = false;			//���� ����
		_hasDashed = false;		//�����ߴ��� ����

		_isDashSoundPlaying = false; //���� �Ҹ�
		_isAttackSoundPlaying = false; //���� �Ҹ�

		_isDownInit = false;

		//�浹��ü ���� �ʱ�ȭ
		_collider->SetActive(true);
		_meshRenderer->SetActive(true);

		for (auto& iter : _attackCol)
		{
			iter->SetActive(false);
		}
		for (auto& iter : _skillAttackCol)
		{
			iter->SetActive(false);
		}

		_distance = 0.f;

		_monsterHelper->Reset();
	}
}