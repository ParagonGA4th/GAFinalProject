#include "PlayerMovement.h"

#include "InGameCameraBehavior.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonAPI/PgTween.h"

#include <singleton-cpp/singleton.h>
#include <algorithm>

namespace Pg::DataScript
{
	PlayerMovement::PlayerMovement(Pg::Data::GameObject* obj) : ScriptInterface(obj)
	{
		//Input 객체 받아오기.
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
	}

	void PlayerMovement::Awake()
	{
		
		
	}

	void PlayerMovement::Start()
	{
		//다른 스크립트의 Awake에서 새롭게 인게임 메인카메라를 설정해야 한다.
		_mainCam = _object->GetScene()->GetMainCamera();
		_camBehavior = _mainCam->_object->GetComponent<Pg::DataScript::InGameCameraBehavior>();
		assert(_camBehavior != nullptr);

		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_renderer != nullptr);

		_selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		assert(_selfCol != nullptr);
		_selfCol->SetMass(5.0f);
		// Height을 받아서, 반값을 기준으로 Intersection 계산할 준비 완료.
		_halfColliderHeight = _selfCol->GetHeight() / 2.0f;

		_isJumping = false;
	}

	void PlayerMovement::Update()
	{
		ShootRayForward();
		DetermineDirectionAndValues();
		UpdateWASD();
		UpdateJump();
		UpdateFacingDirection(_currentPlaneY); //Plane Y-Level 입력해야.

		StrafeAvoidLogic();
	}

	void PlayerMovement::ShootRayForward()
	{
		//tShouldShootDir = Pg::Math::PGFloat3Normalize(tShouldShootDir);
		Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFloat3Normalize(_object->_transform.GetForward());
		tShouldShootDir = PGConvertD3DVec3RotToPhysX(tShouldShootDir);
		Pg::Math::PGFLOAT3 tBasePosition = _object->_transform._position;

		//로직과 상관없는 거
		Pg::Math::PGFLOAT3 outHitPoint;
		float tFloat = 2.0f;
		Pg::Math::PGFLOAT3 tD3DOrigin = { tBasePosition.x + tShouldShootDir.x * tFloat,
						tBasePosition.y + tShouldShootDir.y * tFloat,
						tBasePosition.z + tShouldShootDir.z * tFloat };

		_pgRayCast->MakeRay(tD3DOrigin,
			tShouldShootDir, 30.0f, outHitPoint, nullptr);
	}

	void PlayerMovement::DetermineDirectionAndValues()
	{
		float dt = _pgTime->GetDeltaTime();
		float tMoveSpeed = moveSpeed * 3.0f;

		//Camera -> GameObject를 바라보는 방향이 Forward여야 한다!
		//보간되고 있는 상황이 아니라, Target Pos를 기준으로 움직여야.
		_relativeForward = this->_object->_transform._position - _camBehavior->GetTargetCamPosition();

		//Y Vector 캔슬 + 정규화.
		_relativeForward.y = 0.0f;
		_relativeForward = Pg::Math::PGFloat3Normalize(_relativeForward);

		//Y축이 항상 Global Y-Up을 가리키고 있을 테니, Cross하면 Left Vector.
		_relativeLeft = Pg::Math::PGFloat3Cross(_relativeForward, Pg::Math::PGFLOAT3::GlobalUp());

		//기록은 다르게. 정도가 바뀌었으니.
		_augmentedRelativeForward = { _relativeForward.x * tMoveSpeed * dt, _relativeForward.y * tMoveSpeed * dt, _relativeForward.z * tMoveSpeed * dt };
		_augmentedRelativeLeft = { _relativeLeft.x * tMoveSpeed * dt, _relativeLeft.y * tMoveSpeed * dt, _relativeLeft.z * tMoveSpeed * dt };

		//Face Direction에 필요하다. 현재 발에 있는 위치!
		_currentPlaneY = this->_object->_transform._position.y - _halfColliderHeight;
	}

	void PlayerMovement::UpdateWASD()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MoveFront) ||
			_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MoveBack) ||
			_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MoveLeft) ||
			_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MoveRight))
		{
			///SetAnimation : Run
			_renderer->SetAnimation("test_run.pganim", true);
		}

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveFront))
		{
			//_selfCol->AddForce(relativeForward, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x += _augmentedRelativeForward.x;
			_object->_transform._position.y += _augmentedRelativeForward.y;
			_object->_transform._position.z += _augmentedRelativeForward.z;
			
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveBack))
		{
			//_selfCol->AddForce(-relativeForward, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x -= _augmentedRelativeForward.x;
			_object->_transform._position.y -= _augmentedRelativeForward.y;
			_object->_transform._position.z -= _augmentedRelativeForward.z;
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveLeft))
		{
			//_selfCol->AddForce(relativeLeft, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x += _augmentedRelativeLeft.x;
			_object->_transform._position.y += _augmentedRelativeLeft.y;
			_object->_transform._position.z += _augmentedRelativeLeft.z;
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveRight))
		{
			//_selfCol->AddForce(-relativeLeft, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x -= _augmentedRelativeLeft.x;
			_object->_transform._position.y -= _augmentedRelativeLeft.y;
			_object->_transform._position.z -= _augmentedRelativeLeft.z;
		}

		if (_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveFront) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveBack) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveLeft) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveRight))
		{
			///SetAnimation : Idle.
			_renderer->SetAnimation("test_idle.pganim", true);
			
			//멈췄다가 다시.
			_isJustSetRestraint = true;
			_selfCol->FreezeAxisX(true);
			_selfCol->FreezeAxisY(true);
			_selfCol->FreezeAxisZ(true);
		}

		//PhysX 업데이트를 1차례 거친 후, 다시 리셋.
		if (_isJustSetRestraint)
		{
			//다시 호출 안되게.
			_isJustSetRestraint = false;
		
			_selfCol->FreezeAxisX(true);
			_selfCol->FreezeAxisZ(true);
		}
	}

	void PlayerMovement::UpdateJump()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::Space) && (!_isJumping))
		{
			_isJumping = true;
			_recordedTimeSinceJump = 0.f;
			_selfCol->AddForce(Pg::Math::PGFLOAT3::GlobalUp() * jumpPower, Pg::Data::ForceMode::eIMPULSE);
		}

		if (_isJumping)
		{
			//약간의 간격이 있어야 자신을 인식하지 않을 것이기에.
			const float tSmallOffset = 0.05f; //추가적으로 Y Position Offset. 
			const float tJumpCheckSmallDist = 0.1f; //밑으로 쏘는 정도
			const float tMinimalTimeBeforeRaycastCheck = 0.4f;

			//지난 시간 DeltaTime으로 점검.
			_recordedTimeSinceJump += _pgTime->GetDeltaTime();

			if (_recordedTimeSinceJump > tMinimalTimeBeforeRaycastCheck)
			{
				//밑으로 쏜다.
				Pg::Math::PGFLOAT3 tShouldShootDir = -Pg::Math::PGFLOAT3::GlobalUp();
				Pg::Math::PGFLOAT3 tShouldShootPosition = {
					_object->_transform._position.x, _object->_transform._position.y - _halfColliderHeight - tSmallOffset, _object->_transform._position.z };

				//Raycast 효과적인 범위 검사를 위한 임시.
				//Pg::Math::PGFLOAT3 tShouldShootDir = Pg::Math::PGFLOAT3::GlobalUp();
				//Pg::Math::PGFLOAT3 tShouldShootPosition = {
				//	_object->_transform._position.x, _object->_transform._position.y + _halfColliderHeight + tSmallOffset, _object->_transform._position.z };

				//로직과 상관없는 거
				Pg::Math::PGFLOAT3 outHitPoint;
				Pg::Data::Collider* tOtherCollider = _pgRayCast->MakeRay(tShouldShootPosition,
					tShouldShootDir, tJumpCheckSmallDist, outHitPoint, nullptr);

				//매우 짦은 거리로 쏴야 한다. 닿으면 다시 점프를 재충전할 것이니.
				if (tOtherCollider != nullptr)
				{
					//이제 Collider의 레이어를 여기서 다시 Sort해야 할 것이나,
					//일단은 그 과정은 나중에!
					_isJumping = false;
				}
			}
		}
	}

	void PlayerMovement::UpdateFacingDirection(float yLevelPlane)
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::MouseLeft))
		{
			_targetPos = _mainCam->ScreenPointToWorldPlanePoint({ _pgInput->GetMouseX(), _pgInput->GetMouseY() },
				Pg::Math::PGFLOAT3::GlobalUp(), yLevelPlane);
			//Ex.
			//0,1,0 노말에 비해 -1만큼 Distance가 있어야 y=1에 위치할 수 있다.

			//뺄 때 y축 차이를 없애기 위해서.
			_targetPos.y = _object->_transform._position.y;
			//Pg::Math::PGFLOAT3 lookPos = _targetPos - _object->_transform._position;
			//Pg::Math::PGFLOAT3 lookPos = _object->_transform._position - _targetPos;
			Pg::Math::PGFLOAT3 tLookPos = _object->_transform._position - _targetPos;
			_targetRotation = PGLookRotation(tLookPos, Pg::Math::PGFLOAT3::GlobalUp());

			//업데이트할 값 정하고 Update 루프에서 처리하도록.
			_rotBeginRatio = 0.0f;
			_shouldRotate = true;
		}

		if (_shouldRotate)
		{
			//Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
			//유니티와의 차이.
			Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
			//_selfCol->MoveRotation(currentTargetRotation);
			_object->_transform._rotation = currentTargetRotation;

			_rotBeginRatio += _pgTime->GetDeltaTime() * rotateMultiplier;
			if (_rotBeginRatio > 1.0f)
			{
				_shouldRotate = false;
				_rotBeginRatio = 0.0f;
				_selfCol->SetAngularVelocity({ 0,0,0 });
			}
		}
	}

	void PlayerMovement::StrafeAvoidLogic()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyUp) && (!_isStrafeAvoiding))
		{
			_isStrafeAvoiding = true;

			//ForwardVector의 Back 방향으로 이동해야 한다.
			const float tAvoidDist = 3.0f; //실제로 이동한 거리.
			const float tAvoidBasedTotalTime = 1.0f; //Tween 시간 비율로 Cut 전에, 전체 시간.
			const float tCutShortRatio = 0.5f; //언제 빨리 끝낼지, 0-1.

			Pg::Math::PGFLOAT3 tActualForward = Pg::Math::PGReflectVectorAgainstAxis(_object->_transform.GetForward(), Pg::Math::PGFLOAT3::GlobalForward());
			Pg::Math::PGFLOAT3 tTargetPos = _object->_transform._position - (-tActualForward * tAvoidDist);

			Pg::Util::Tween* tTween = _pgTween->CreateTween();
			tTween->GetData(&(_object->_transform._position)).DoMove(tTargetPos, tAvoidBasedTotalTime).
				SetEase(Pg::Util::Enums::eEasingMode::OUTEXPO).KillEarly(tCutShortRatio).OnComplete(
					[this]()
					{
						OnStrafeAvoidComplete();
					});
		}
	}

	void PlayerMovement::OnStrafeAvoidComplete()
	{
		_isStrafeAvoiding = false;
	}

}