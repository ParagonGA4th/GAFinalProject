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
		//CombatSystem 받아오자.
		_combatSystem = CombatSystem::GetInstance(nullptr);

		//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES); // 자기 자신이 Projectile이라고 해주기.
		_collider->SetActive(false);
		_collider->SetUseGravity(false);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisZ(true);

		_meshRenderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_meshRenderer != nullptr);

		//무조건 자기 자신이 소속된 오브젝트의 Tag를 "TAG_Arrow"로 바꿈.
		_object->SetTag("TAG_Arrow");

		//모든 Trail 오브젝트들 끄기.
		TurnOffAllTrailObjects();
	}

	void ArrowLogic::Awake()
	{
		// ArrowType에 따라서 다르게 세팅해야 한다.
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
		//리셋이 되었으니, 다시 쏠 수 있는 상태가 되었다. 
		_isNowShooting = false;
		_isJustInvokedShoot = false;

		_startCountingTime = false;
		_elapsedTime = 0.0f;
		_initialPos = { 0,0,0 };

		//게임 플레이에 영향을 안 주는 위치여야 한다.
		_object->_transform._position = { 0,-100,0 };

		//Collider도 꺼버려야 함.
		_collider->SetActive(false);

		//여기에다가, Renderer도 꺼주어야 함!
		_meshRenderer->SetActive(false);

		//RigidBody UseGravity도 꺼주기.
		_collider->SetUseGravity(false);

		TurnOffAllTrailObjects();
	}

	bool ArrowLogic::GetIsNowShooting()
	{
		return _isNowShooting;
	}

	void ArrowLogic::ShootArrow(Pg::Math::PGFLOAT3 initialPos, Pg::Math::PGFLOAT3 shootDir)
	{

		//스스로에게 사용되는 중이라고 상태 설정.
		_isNowShooting = true;
		_isJustInvokedShoot = true;

		_initialPos = initialPos;
		_shootDir = shootDir;

		//Target Pos 기록 (tween에 활용됨)
		_targetPos = _initialPos + _shootDir * _arrowDistBeforeFall;
		_meshRenderer->SetActive(false);
		//_object->_transform._rotation = Pg::Math::PGLookRotation(PGFloat3Normalize(_targetPos - _initialPos), { 0,1,0 });
	}

	void ArrowLogic::CarryOutShoot()
	{
		//트윈 시스템도 손봐야 할 것 같다.
		//Tween 발동.
		Pg::Util::Tween* tTween = _pgTween->CreateTween();
		_usingTween = tTween;

		//Tween 작동.
		tTween->GetData(&(_object->_transform._position))
			.DoMove(_targetPos, _secondsBeforeGravity)
			.SetEase(Pg::Util::Enums::eEasingMode::OUTQUART).KillEarly(0.7f)
			.OnComplete([this]()
				{
					//이제는 막 물리를 끄는 것도 아니고, 
					//그냥 사라지게 해야 한다.
					EndShootingSelf();
				});

		//Tween 기록.
	}

	void ArrowLogic::EndShootingSelf()
	{
		_collider->SetActive(false);
		_meshRenderer->SetActive(false);
		TurnOffAllTrailObjects();
	}

	void ArrowLogic::IfValidActualShootLogic()
	{
		//무조건 Arrow관리 Update보다 늦게 호출되어야 함.
		if (_isNowShooting)
		{
			if (!_startCountingTime)
			{
				_object->_transform._position = _initialPos;
				_elapsedTime = 0.0f;
				_startCountingTime = true;

				//Renderer / Collider 키기.
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

			//Elapsed Time 기록.
			_elapsedTime += _pgTime->GetDeltaTime();
		}
	}

	void ArrowLogic::OnTriggerEnter(Pg::Data::Collider** _colArr, unsigned int count)
	{
		if (_comboSystem == nullptr)
		{
			return;
		}

		//SceneSystem의 함수들은 이거 발생 후 호출된다. 밖의 로직애서 하는 게 맞다.
		for (int i = 0; i < count; i++)
		{
			//무조건 해당 수만큼은 들어온다.
			Pg::Data::Collider* tCol = _colArr[i];

			//Physics Layer로 검사한다.
			//몬스터일 때 설정하는 것이니.
			if (tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MONSTER ||
				tCol->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_BOSS)
			{
				//몬스터 때렸다는 것.
				//자신이 직접 데미지를 연산하는 것이 아니다! 
				//기록해서 PlayerBattleBehavior가 처리해 줄 것.
				IEnemyBehaviour* tEnemyBehaviour = tCol->_object->GetComponent<IEnemyBehaviour>();
				assert((tEnemyBehaviour != nullptr) && "무조건 찾았어야 했다");

				//ComboSystem한테 적 때렸다고 전달.
				_comboSystem->HitObject(true);

				int tComboIndex = std::clamp<int>(_comboSystem->GetComboCount(), 1, ComboSystem::MAXIMUM_HIT_COUNT);
				tComboIndex -= 1; //무조건 ComboCount가 1 / 2 / 3 당 0, 1, 2를 각각 반환하게 설정하는 것이다. 인덱스 이슈. 

				//실제 충돌을 한 것이니, Collider와 Renderer를 끄자!
				_meshRenderer->SetActive(false);
				_collider->SetActive(false);
				_usingTween->Kill();
				//충돌했으면 Trail도 꺼야 한다.
				TurnOffAllTrailObjects();
			
				//Base Info 기록. OnHit 호출을 위해.
				//여기서는 실제로 Hit 했을 포인트.
				tEnemyBehaviour->ReturnBaseMonsterInfo()->OnHitEnableHitEffect(_arrowType);

				//Damage Logic 실행, 콤보는 공격의 종류와 상관없이 유지될 것이니.
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
				//ComboSystem한테 때렸는데 몬스터가 아니었다고 전달.
				_comboSystem->HitObject(false);
			}
		}
	}

	void ArrowLogic::InitSelfAsNormalArrow()
	{
		//현재 일반 화살처럼.
		_assignedDamageLogic = std::bind(&ArrowLogic::NormalArrowDamageLogic, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ArrowLogic::InitSelfAsIceArrow()
	{
		//잠깐 멈추거나, 속도 느리게.
		_assignedDamageLogic = std::bind(&ArrowLogic::IceArrowDamageLogic, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ArrowLogic::InitSelfAsFireArrow()
	{
		//독뎀 : 도트데미지 있게 해야.
		_assignedDamageLogic = std::bind(&ArrowLogic::FireArrowDamageLogic, this, std::placeholders::_1, std::placeholders::_2);
	}

	void ArrowLogic::NormalArrowDamageLogic(IEnemyBehaviour* behav, int comboIndex)
	{
		//해당 데미지를 입력, PlayerBattleBehavior로 하여금 이를 처리할 수 있게 만든다.
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
		//Trail Object들을 등록.
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
		//TRAIL_DIVIDED_COUNT 2개여야 한다.
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
	
		//만약 지금 쏘고 있는 상태라면
		if (_isNowShooting && (_meshRenderer->GetActive()))
		{
			if (_isJustInvokedShoot)
			{
				//막 발동 시작.
				_isJustInvokedShoot = false;

				//Time 세기 위해서 리셋.
				_trailActiveCount = 0;
				_trailActiveTime = 0.f;

				//이펙트 살리기 하나.
				_soleTrail->SetActive(true);
			}

			Pg::Math::PGFLOAT3 tCurArrowPos = _object->_transform._position;
			// Arrow는 눕혀서 바인딩된다. (BindObject)
			
			// 시간이 지날수록, 스폰해야 한다. 
			float dt = _pgTime->GetDeltaTime();
			_trailActiveTime += dt;

			Pg::Math::PGFLOAT3 tForwardDirection = Pg::Math::PGFloat3Normalize(tCurArrowPos - _targetPos);
			
			float tSinOffset = (fabs(sin(_trailActiveTime)) + 3.0f) * (0.3f);

			//0번째 Set.
			float tZeroOffsetFactor = DIST_FACTOR * tSinOffset;
			_soleTrail->_position = tCurArrowPos + (tForwardDirection * tZeroOffsetFactor);
			_soleTrail->_rotation = _object->_transform._rotation;
			_soleTrail->_scale = Pg::Math::PGFLOAT3(0.5f, 1.f, 2.f);

			//if ((_trailActiveTime > 0.2f) && (_trailActiveTime < 0.3f))
			//{
			//	//반복 호출해도 부하 X.
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