#include "ArrowLogic.h"

#include "../ParagonData/StaticMeshRenderer.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonData/LayerMask.h"

#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgTween.h"

#include <cassert>
#include <singleton-cpp/singleton.h>

namespace Pg::DataScript
{
	ArrowLogic::ArrowLogic(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
	}

	void ArrowLogic::BeforePhysicsUpdate()
	{
		if (!_alreadyCalledBPU)
		{
			//내부적으로 Physics보다 SceneSystem의 함수들이 나중에 호출됨. 그러니, 미리 할 수 있는 방법을 EngineMain-SceneSystem에 연결해두었다.
			_collider = _object->GetComponent<Pg::Data::BoxCollider>();
			assert(_collider != nullptr);
			_collider->SetLayer(Pg::Data::Enums::eLayerMask::LAYER_PROJECTILES); // 자기 자신이 Projectile이라고 해주기.
			_collider->SetActive(false);
			_collider->SetUseGravity(false);

			//Debouncer.
			_alreadyCalledBPU = true;
		}
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
		_shootDir = PGFloat3Normalize(_shootDir); //외적인 Normalize.

		//Target Pos 기록 (tween에 활용됨)
		_targetPos = _initialPos + _shootDir * _arrowDistBeforeFall;
	}

	void ArrowLogic::CarryOutShoot()
	{
		//쏘는 방향으로 Rotation 변경.
		_object->_transform._rotation = PGEulerToQuaternion(_shootDir);

		//_object->_transform._rotation = Pg::Math::PGLookRotation(_targetPos, { 0,1,0 });

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

			if (_elapsedTime > _afterDestroySec)
			{
				ResetState();
			}


			//Elapsed Time 기록.
			_elapsedTime += _pgTime->GetDeltaTime();
		}
	}
	

}