#include "ArrowLogic.h"

#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/PhysicsCollision.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgTween.h"
#include "../ParagonUtil/Log.h"

#include "BaseMonster.h"
#include "PlayerBattleBehavior.h"
#include "ComboSystem.h"

#include <cassert>
#include <algorithm>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArrowLogic::ArrowLogic(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
	}

	void ArrowLogic::BeforePhysicsAwake()
	{
		//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
		_collider = _object->GetComponent<Pg::Data::BoxCollider>();
		assert(_collider != nullptr);
		_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES); // 자기 자신이 Projectile이라고 해주기.
		_collider->SetActive(false);
		_collider->SetUseGravity(false);
		_collider->FreezeAxisX(true);
		_collider->FreezeAxisZ(true);
	}

	void ArrowLogic::Awake()
	{
		//무조건 자기 자신이 소속된 오브젝트의 Tag를 "TAG_Arrow"로 바꿈.
		_object->SetTag("TAG_Arrow");

		_meshRenderer = _object->GetComponent<Pg::Data::StaticMeshRenderer>();
		assert(_meshRenderer != nullptr);
	}

	void ArrowLogic::Start()
	{
		ResetState();

		_comboSystem = ComboSystem::GetInstance();
	}

	void ArrowLogic::FixedUpdate()
	{
		IfValidActualShootLogic();
	}

	void ArrowLogic::ResetState()
	{
		//리셋이 되었으니, 다시 쏠 수 있는 상태가 되었다. 
		_isNowShooting = false;
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
	}

	void ArrowLogic::EnableDrop()
	{
		_collider->SetUseGravity(true);
	}

	bool ArrowLogic::GetIsNowShooting()
	{
		return _isNowShooting;
	}

	void ArrowLogic::ShootArrow(Pg::Math::PGFLOAT3 initialPos, Pg::Math::PGFLOAT3 shootDir)
	{
		//스스로에게 사용되는 중이라고 상태 설정.
		_isNowShooting = true;

		_initialPos = initialPos;
		_shootDir = shootDir;

		//Target Pos 기록 (tween에 활용됨)
		_targetPos = _initialPos + _shootDir * _arrowDistBeforeFall;
		_meshRenderer->SetActive(false);
		//_object->_transform._rotation = Pg::Math::PGLookRotation(PGFloat3Normalize(_targetPos - _initialPos), { 0,1,0 });
	}

	void ArrowLogic::CarryOutShoot()
	{
		//쏘는 방향으로 Rotation 변경.
		//_object->_transform._rotation = PGEulerToQuaternion(_shootDir);
		//_object->_transform._rotation = Pg::Math::PGLookRotation(PGFloat3Normalize(_targetPos - _initialPos), { 0,1,0 });

		//Pg::Math::PGFLOAT3 tRotTarget = _targetPos - _initialPos;
		//tRotTarget.y = 0;
		//_object->_transform._rotation = Pg::Math::PGEulerToQuaternion(PGFloat3Normalize(tRotTarget));

		//트윈 시스템도 손봐야 할 것 같다.
		//Tween 발동.
		Pg::Util::Tween* tTween = _pgTween->CreateTween();

		//Tween 작동.
		tTween->GetData(&(_object->_transform._position))
			.DoMove(_targetPos, _secondsBeforeGravity)
			.SetEase(Pg::Util::Enums::eEasingMode::OUTQUART)
			.OnComplete([this]()
				{
					EnableDrop();
				});
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
				// 여기다가, rigidbody의 setgravity도 꺼주어야 함!
				//Rigidbody SetGravity 끄기.
				_collider->SetUseGravity(false);

				CarryOutShoot();
			}

			_object->_transform._rotation = Pg::Math::PGLookRotation(PGFloat3Normalize(_targetPos - _initialPos), { 0,1,0 });

			if (_elapsedTime > _afterDestroySec)
			{
				ResetState();
			}


			//Elapsed Time 기록.
			_elapsedTime += _pgTime->GetDeltaTime();
		}
	}

	void ArrowLogic::OnCollisionEnter(Pg::Data::PhysicsCollision** _colArr, unsigned int count)
	{
		if (_comboSystem == nullptr)
		{
			return;
		}

		//SceneSystem의 함수들은 이거 발생 후 호출된다. 밖의 로직애서 하는 게 맞다.
		for (int i = 0; i < count; i++)
		{
			//무조건 해당 수만큼은 들어온다.
			Pg::Data::PhysicsCollision* tCol = _colArr[i];

			//{
			//	std::string tString = "ENTERED MONSTER: ";
			//	tString += std::to_string(count);
			//	PG_TRACE(tString.c_str());
			//}
			/////PhysicsCollision이 리턴될 때, thisActor, otherActor의 순서가 일정하지 않다.
			/////그러니, 실제로 다른 충돌한 Actor를 사용!
			//Pg::Data::Collider* tRealOtherActor = nullptr;
			//
			//if (tCol->_thisActor->_object != this->_object)
			//{
			//	tRealOtherActor = tCol->_thisActor;
			//}
			//else
			//{
			//	tRealOtherActor = tCol->_otherActor;
			//}
			Pg::Data::Collider* tRealOtherActor = Pg::Data::PhysicsCollision::GetActualOtherActor(tCol, this->_object);

			//Physics Layer로 검사한다.
			//몬스터일 때 설정하는 것이니.
			if (tRealOtherActor->GetLayer() == Pg::Data::Enums::eLayerMask::LAYER_MONSTER)
			{

				//몬스터 때렸다는 것.
				//자신이 직접 데미지를 연산하는 것이 아니다! 
				//기록해서 PlayerBattleBehavior가 처리해 줄 것.
				BaseMonster* tBaseMonster = tRealOtherActor->_object->GetComponent<BaseMonster>();
				assert((tBaseMonster != nullptr) && "무조건 찾았어야 했다");

				//ComboSystem한테 적 때렸다고 전달.
				_comboSystem->HitObject(true);

				int tComboIndex = std::clamp<int>(_comboSystem->GetComboCount(), 1, ComboSystem::MAXIMUM_HIT_COUNT);
				tComboIndex -= 1; //무조건 ComboCount가 1 / 2 / 3 당 0, 1, 2를 각각 반환하게 설정하는 것이다. 인덱스 이슈. 

				//실제 충돌을 한 것이니, Collider와 Renderer를 끄자!
				_meshRenderer->SetActive(false);
				_collider->SetActive(false);

				//해당 데미지를 입력, PlayerBattleBehavior로 하여금 이를 처리할 수 있게 만든다.
				_playerBattleBehavior->AddMonsterHitList(tBaseMonster, -(ARROW_ATTACK_POWER * ComboSystem::DAMAGE_MULTIPLIER[tComboIndex]));

				{
					std::string tComboStr = "ComboCount : ";
					tComboStr += std::to_string(_comboSystem->GetComboCount());
					tComboStr += " // ";
					PG_TRACE(tComboStr.c_str());
				}
			}
			else
			{
				//ComboSystem한테 때렸는데 몬스터가 아니었다고 전달.
				_comboSystem->HitObject(false);
			}
		}
	}

	//void ArrowLogic::OnCollisionExit(Pg::Data::PhysicsCollision** _colArr, unsigned int count)
	//{
	//	PG_TRACE("EXIT CALLED");
	//}

}