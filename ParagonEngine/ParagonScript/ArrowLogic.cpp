#include "ArrowLogic.h"

#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/PhysicsCollision.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgTween.h"
#include "../ParagonAPI/PgGraphics.h"

#include "../ParagonUtil/Log.h"

#include "BaseMonster.h"
#include "IEnemyBehaviour.h"
#include "PlayerHandler.h"
#include "ComboSystem.h"
#include "CombatSystem.h"

#include <cassert>
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArrowLogic::ArrowLogic(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	void ArrowLogic::GrabManagedObjects()
	{
		InitTrailObjects();
	}

	void ArrowLogic::BeforePhysicsAwake()
	{
		//CombatSystem �޾ƿ���.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//���������� Physics���� SceneSystem�� �Լ����� ���߿� ȣ���. �׷���, �̸� �� �� �ִ� ����� EngineMain-SceneSystem�� �����صξ���.
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES); // �ڱ� �ڽ��� Projectile�̶�� ���ֱ�.
		_collider->SetActive(false);
		_collider->SetUseGravity(false);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisZ(true);

		_meshRenderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_meshRenderer != nullptr);

		//������ �ڱ� �ڽ��� �Ҽӵ� ������Ʈ�� Tag�� "TAG_Arrow"�� �ٲ�.
		_object->SetTag("TAG_Arrow");

		//��� Trail ������Ʈ�� ����.
		TurnOffAllTrailObjects();
	}

	void ArrowLogic::Awake()
	{
		// ArrowType�� ���� �ٸ��� �����ؾ� �Ѵ�.
		switch (_arrowType)
		{
			case -1: { InitSelfAsIceArrow(); } break;
			case 0: { InitSelfAsNormalArrow(); } break;
			case 1: { InitSelfAsFireArrow(); } break;
			default:
			{
				PG_ERROR("Not Supported Arrow Type");
				assert(false);
			}
			break;
		}
	}

	void ArrowLogic::Start()
	{
		ResetState();

		_comboSystem = ComboSystem::GetInstance();
	}

	void ArrowLogic::FixedUpdate()
	{
		IfValidActualShootLogic();
		TrailUpdateLogic();
	}

	void ArrowLogic::ResetState()
	{
		//������ �Ǿ�����, �ٽ� �� �� �ִ� ���°� �Ǿ���. 
		_isNowShooting = false;
		_isJustInvokedShoot = false;

		_startCountingTime = false;
		_elapsedTime = 0.0f;
		_initialPos = { 0,0,0 };

		//���� �÷��̿� ������ �� �ִ� ��ġ���� �Ѵ�.
		_object->_transform._position = { 0,-100,0 };

		//Collider�� �������� ��.
		_collider->SetActive(false);

		//���⿡�ٰ�, Renderer�� ���־�� ��!
		_meshRenderer->SetActive(false);

		//RigidBody UseGravity�� ���ֱ�.
		_collider->SetUseGravity(false);

		TurnOffAllTrailObjects();
	}

	bool ArrowLogic::GetIsNowShooting()
	{
		return _isNowShooting;
	}

	void ArrowLogic::ShootArrow(Pg::Math::PGFLOAT3 initialPos, Pg::Math::PGFLOAT3 shootDir)
	{

		//�����ο��� ���Ǵ� ���̶�� ���� ����.
		_isNowShooting = true;
		_isJustInvokedShoot = true;

		_initialPos = initialPos;
		_shootDir = shootDir;

		//Target Pos ��� (tween�� Ȱ���)
		_targetPos = _initialPos + _shootDir * _arrowDistBeforeFall;
		_meshRenderer->SetActive(false);
		//_object->_transform._rotation = Pg::Math::PGLookRotation(PGFloat3Normalize(_targetPos - _initialPos), { 0,1,0 });
	}

	void ArrowLogic::CarryOutShoot()
	{
		//Ʈ�� �ý��۵� �պ��� �� �� ����.
		//Tween �ߵ�.
		Pg::Util::Tween* tTween = _pgTween->CreateTween();
		_usingTween = tTween;

		//Tween �۵�.
		tTween->GetData(&(_object->_transform._position))
			.DoMove(_targetPos, _secondsBeforeGravity)
			.SetEase(Pg::Util::Enums::eEasingMode::OUTQUART).KillEarly(0.7f)
			.OnComplete([this]()
				{
					//������ �� ������ ���� �͵� �ƴϰ�, 
					//�׳� ������� �ؾ� �Ѵ�.
					EndShootingSelf();
				});

		//Tween ���.
	}

	void ArrowLogic::EndShootingSelf()
	{
		_collider->SetActive(false);
		_meshRenderer->SetActive(false);
		TurnOffAllTrailObjects();
	}

	void ArrowLogic::IfValidActualShootLogic()
	{
		//������ Arrow���� Update���� �ʰ� ȣ��Ǿ�� ��.
		if (_isNowShooting)
		{
			if (!_startCountingTime)
			{
				_object->_transform._position = _initialPos;
				_elapsedTime = 0.0f;
				_startCountingTime = true;

				//Renderer / Collider Ű��.
				_collider->SetActive(true);
				_meshRenderer->SetActive(true);

				CarryOutShoot();
			}

			_object->_transform._rotation = Pg::Math::PGLookRotation(PGFloat3Normalize(_targetPos - _initialPos), { 0,1,0 });

			if (_elapsedTime > _afterDestroySec)
			{
				ResetState();
				TurnOffAllTrailObjects();
			}

			//Elapsed Time ���.
			_elapsedTime += _pgTime->GetDeltaTime();
		}
	}

	void ArrowLogic::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		if (_comboSystem == nullptr)
		{
			return;
		}

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

				int tComboIndex = std::clamp<int>(_comboSystem->GetComboCount(), 1, ComboSystem::MAXIMUM_HIT_COUNT);
				tComboIndex -= 1; //������ ComboCount�� 1 / 2 / 3 �� 0, 1, 2�� ���� ��ȯ�ϰ� �����ϴ� ���̴�. �ε��� �̽�. 

				//���� �浹�� �� ���̴�, Collider�� Renderer�� ����!
				_meshRenderer->SetActive(false);
				_collider->SetActive(false);
				_usingTween->Kill();
				//�浹������ Trail�� ���� �Ѵ�.
				TurnOffAllTrailObjects();
			
				//Base Info ���. OnHit ȣ���� ����.
				//���⼭�� ������ Hit ���� ����Ʈ.
				tEnemyBehaviour->ReturnBaseMonsterInfo()->OnHitEnableHitEffect(_arrowType);

				//Damage Logic ����, �޺��� ������ ������ ������� ������ ���̴�.
				_assignedDamageLogic(tEnemyBehaviour, tComboIndex);

				//{
				//	std::string tComboStr = "ComboCount : ";
				//	tComboStr += std::to_string(_comboSystem->GetComboCount());
				//	tComboStr += " // ";
				//	PG_TRACE(tComboStr.c_str());
				//}
			}
			else
			{
				//ComboSystem���� ���ȴµ� ���Ͱ� �ƴϾ��ٰ� ����.
				_comboSystem->HitObject(false);
			}
		}
	}

	void ArrowLogic::InitSelfAsNormalArrow()
	{
		//���� �Ϲ� ȭ��ó��.
		_assignedDamageLogic = std::bind(&ArrowLogic::NormalArrowDamageLogic, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ArrowLogic::InitSelfAsIceArrow()
	{
		//��� ���߰ų�, �ӵ� ������.
		_assignedDamageLogic = std::bind(&ArrowLogic::IceArrowDamageLogic, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ArrowLogic::InitSelfAsFireArrow()
	{
		//���� : ��Ʈ������ �ְ� �ؾ�.
		_assignedDamageLogic = std::bind(&ArrowLogic::FireArrowDamageLogic, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ArrowLogic::NormalArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex)
	{
		//�ش� �������� �Է�, PlayerBattleBehavior�� �Ͽ��� �̸� ó���� �� �ְ� �����.
		_combatSystem->AddMonsterHitList(behav->ReturnBaseMonsterInfo(), -(ARROW_ATTACK_POWER * ComboSystem::DAMAGE_MULTIPLIER[comboIndex]), ePartialAttackType::eNORMAL_HIT);
		_combatSystem->AddMonsterOnHitList(behav->ReturnBaseMonsterInfo());
	}

	void ArrowLogic::IceArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex)
	{
		PG_WARN("ICE Damange : {0}", ICEARROW_ATTACK_POWER);
		_combatSystem->AddMonsterHitList(behav->ReturnBaseMonsterInfo(), -(ICEARROW_ATTACK_POWER), ePartialAttackType::eICE_HIT);
		_combatSystem->AddMonsterOnHitList(behav->ReturnBaseMonsterInfo());
		_combatSystem->AddMonsterIceDamageList(behav->ReturnBaseMonsterInfo());
	}

	void ArrowLogic::FireArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex)
	{
		PG_WARN("Fire Damange : {0}", FIREARROW_ATTACK_POWER);
		_combatSystem->AddMonsterHitList(behav->ReturnBaseMonsterInfo(), -(FIREARROW_ATTACK_POWER), ePartialAttackType::eFIRE_HIT);
		_combatSystem->AddMonsterOnHitList(behav->ReturnBaseMonsterInfo());
		_combatSystem->AddMonsterFireDamageList(behav->ReturnBaseMonsterInfo());
	}

	void ArrowLogic::OnSerialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnSerializerHelper<ArrowLogic>(this, sv);
	}

	void ArrowLogic::OnDeserialize(SerializeVector& sv)
	{
		Pg::Data::SerializerHelper::OnDeserializerHelper<ArrowLogic>(this, sv);
	}

	void ArrowLogic::CleanOnSceneChange()
	{
		TurnOffAllTrailObjects();
	}

	void ArrowLogic::InitTrailObjects()
	{
		//Trail Object���� ���.
		Pg::Data::VisualEffectRenderObject* vo = new Pg::Data::VisualEffectRenderObject();

		switch (_arrowType)
		{
			case -1:
			{
				_pgGraphics->RegisterEffectObject("Effect_IceArrowTrail", vo);
			}
			break;
			case 0:
			{
				_pgGraphics->RegisterEffectObject("Effect_ArrowTrail", vo);
			}
			break;
			case 1:
			{
				_pgGraphics->RegisterEffectObject("Effect_FireArrowTrail", vo);
			}
			break;
		}
		
		_soleTrail = vo;

		TurnOffAllTrailObjects();
	}

	void ArrowLogic::TurnOffAllTrailObjects()
	{
		//PG_WARN("Turned Off");
		//TRAIL_DIVIDED_COUNT 2������ �Ѵ�.
		_soleTrail->SetActive(false);

		//for (int i = 0; i < TRAIL_DIVIDED_COUNT; i++)
		//{
		//	
		//}
	}

	void ArrowLogic::TrailUpdateLogic()
	{
		const float DIST_FACTOR = 1.75f;
		const float MINOR_DISTORTION = 0.5f;
	
		//���� ���� ��� �ִ� ���¶��
		if (_isNowShooting && (_meshRenderer->GetActive()))
		{
			if (_isJustInvokedShoot)
			{
				//�� �ߵ� ����.
				_isJustInvokedShoot = false;

				//Time ���� ���ؼ� ����.
				_trailActiveCount = 0;
				_trailActiveTime = 0.f;

				//����Ʈ �츮�� �ϳ�.
				_soleTrail->SetActive(true);
			}

			Pg::Math::PGFLOAT3 tCurArrowPos = _object->_transform._position;
			// Arrow�� ������ ���ε��ȴ�. (BindObject)
			
			// �ð��� ��������, �����ؾ� �Ѵ�. 
			float dt = _pgTime->GetDeltaTime();
			_trailActiveTime += dt;

			Pg::Math::PGFLOAT3 tForwardDirection = Pg::Math::PGFloat3Normalize(tCurArrowPos - _targetPos);
			
			float tSinOffset = (fabs(sin(_trailActiveTime)) + 3.0f) * (0.3f);

			//0��° Set.
			float tZeroOffsetFactor = DIST_FACTOR * tSinOffset;
			_soleTrail->_position = tCurArrowPos + (tForwardDirection * tZeroOffsetFactor);
			_soleTrail->_rotation = _object->_transform._rotation;
			_soleTrail->_scale = Pg::Math::PGFLOAT3(0.5f, 1.f, 2.f);

			//if ((_trailActiveTime > 0.2f) && (_trailActiveTime < 0.3f))
			//{
			//	//�ݺ� ȣ���ص� ���� X.
			//	_trailList.at(1)->SetActive(true);
			//}
			//if (_trailActiveTime > 0.2f)
			//{
			//	float tFirstOffsetFactor = 2.0f * DIST_FACTOR * tSinOffset;
			//	_trailList.at(1)->_position = tCurArrowPos + (tForwardDirection * tFirstOffsetFactor);
			//	_trailList.at(1)->_rotation = _object->_transform._rotation;
			//}

			//PG_WARN("Zero State : {0}", _trailList.at(0)->GetActive());
			//PG_WARN("First State : {0}", _trailList.at(1)->GetActive());
		}
		else
		{
			TurnOffAllTrailObjects();
		}
	}



}