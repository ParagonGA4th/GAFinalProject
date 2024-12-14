#include "PlayerMovementSector.h"

#include "InGameCameraBehavior.h"
#include "PlayerHandler.h"

#include "../ParagonData/Camera.h"
#include "../ParagonData/GameObject.h"
#include "../ParagonData/LayerMask.h"
#include "../ParagonData/Scene.h"
#include "../ParagonData/AudioSource.h"
#include "../ParagonData/SkinnedMeshRenderer.h"
#include "../ParagonData/DynamicCollider.h"
#include "../ParagonData/CapsuleCollider.h"
#include "../ParagonData/BoxCollider.h"
#include "../ParagonUtil/Log.h"
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonAPI/PgTween.h"
#include "../ParagonAPI/PgGraphics.h"

#include <singleton-cpp/singleton.h>
#include <algorithm>

namespace Pg::DataScript
{
	PlayerMovementSector::PlayerMovementSector(PlayerHandler* playerHandler) : _playerHandler(playerHandler), _object(playerHandler->_object)
	{
		//Input 객체 받아오기.
		_pgInput = &singleton<Pg::API::Input::PgInput>();
		_pgTime = &singleton<Pg::API::Time::PgTime>();
		_pgRayCast = &singleton<Pg::API::Raycast::PgRayCast>();
		_pgTween = &singleton<Pg::API::Tween::PgTween>();
		_pgGraphics = &singleton<Pg::API::Graphics::PgGraphics>();
	}

	void PlayerMovementSector::GrabManagedObjects()
	{

	}

	void PlayerMovementSector::BeforePhysicsAwake()
	{
		//제거해야
		//_playerHandler->_selfCol->SetUseGravity(false);
	}

	void PlayerMovementSector::Awake()
	{
		// Height을 받아서, 반값을 기준으로 Intersection 계산할 준비 완료.
		_halfColliderHeight = _playerHandler->_selfCol->GetHeight() / 2.0f;
		_isJumping = false;
		//Camera의 Awake에서 MainCamera가 들어올 것이다. 이거 빼고 다 받자.
	}

	void PlayerMovementSector::Start()
	{
		//다른 스크립트의 Awake에서 새롭게 인게임 메인카메라를 설정해야 한다.
		_mainCam = _object->GetScene()->GetMainCamera();
		_camBehavior = _mainCam->_object->GetComponent<Pg::DataScript::InGameCameraBehavior>();
		assert(_camBehavior != nullptr);
	}

	void PlayerMovementSector::Update()
	{
		ShootRayForward();
		DetermineDirectionAndValues();

		if ((!_useUltimateSkill) && (!_useStrongSkill))
		{
			UpdateWASD();
			UpdateJump();
			UpdateFacingDirection(_currentPlaneY); //Plane Y-Level 입력해야.
			StrafeAvoidLogic();
		}
	
		if (_playerHandler->healthPoint < std::numeric_limits<float>::epsilon())
		{
			_isDead_Animation = true;
		}

		//Player Position 기록.
		_pgGraphics->RegisterPlayerPosition(_playerHandler->_object->_transform._position);
	}

	void PlayerMovementSector::LateUpdate()
	{
		PlayAdequateAnimation();
	}

	void PlayerMovementSector::OnAnimationEnd(const std::string& justEndedAnimation)
	{
		_isJumping_Animation = false;
		_isDead_Animation = false;
	}

	void PlayerMovementSector::ShootRayForward()
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

		//D3DOrigin.y 좀 올리기.
		tD3DOrigin.y += 2.f;

		_pgRayCast->MakeRay(tD3DOrigin,
			tShouldShootDir, 30.0f, outHitPoint, nullptr);
	}

	void PlayerMovementSector::DetermineDirectionAndValues()
	{
		float dt = _pgTime->GetDeltaTime();
		float tMoveSpeed = 0.f;

		if (_playerHandler->_object->GetScene()->GetSceneName() == "BossStage")
		{
			tMoveSpeed = moveSpeed * 2.5f;
		}
		else
		{
			tMoveSpeed = moveSpeed * 3.0f;
		}

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
		//원본
		//_currentPlaneY = this->_object->_transform._position.y - _halfColliderHeight;
		
		//Position Offset 반영 수정본.
		_currentPlaneY = this->_object->_transform._position.y - _halfColliderHeight - _playerHandler->_selfCol->GetPositionOffset().y;
		
		//각도에 따른 클릭 포인트 오차 발생, 해당 문제를 해결해보자, _currentPlaneY를 Offset하는 방향으로!
		{
			//둘 다 정규화되어 있으니 -1 ~ 1 반환할 것.
			//이제 절대값으로 0-1이 되었음.
			float tLookVectorSimilarity = fabs(PGFloat3Dot(_relativeForward, Pg::Math::PGFLOAT3::GlobalUp()));

			//각도에 따라 비율이 달라지겠지만...
			DirectX::XMVECTOR tXMForward = PG2XM_FLOAT3_VECTOR(_relativeForward);
			DirectX::XMVECTOR tForwardQuat = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(tXMForward));
			DirectX::XMVECTOR tCamLookQuat = DirectX::XMQuaternionNormalize(PG2XM_QUATERNION_VECTOR(_camBehavior->_object->_transform._rotation));
			float tQuatDot = fabs(DirectX::XMVectorGetX(DirectX::XMQuaternionDot(tForwardQuat, tCamLookQuat)));

			//+해야.
			_currentPlaneY = _currentPlaneY - tLookVectorSimilarity;
		}
	}

	void PlayerMovementSector::UpdateWASD()
	{
		//일단 무조건 안 움직인다고 생각하고, 움직일 떄만 Animation 적용.
		_isMoving_Animation = false;


		Pg::Math::PGFLOAT3 totalMovement = { 0.0f, 0.0f, 0.0f };
		bool isMoving = false;

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveFront)) 
		{
			totalMovement.x += _augmentedRelativeForward.x;
			totalMovement.y += _augmentedRelativeForward.y;
			totalMovement.z += _augmentedRelativeForward.z;
			isMoving = true;
		}

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveBack)) 
		{
			totalMovement.x -= _augmentedRelativeForward.x;
			totalMovement.y -= _augmentedRelativeForward.y;
			totalMovement.z -= _augmentedRelativeForward.z;
			isMoving = true;
		}

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveLeft)) 
		{
			totalMovement.x += _augmentedRelativeLeft.x;
			totalMovement.y += _augmentedRelativeLeft.y;
			totalMovement.z += _augmentedRelativeLeft.z;
			isMoving = true;
		}

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveRight)) 
		{
			totalMovement.x -= _augmentedRelativeLeft.x;
			totalMovement.y -= _augmentedRelativeLeft.y;
			totalMovement.z -= _augmentedRelativeLeft.z;
			isMoving = true;
		}

		if (isMoving) 
		{
			// 정규화
			Pg::Math::PGFLOAT3 movementDirection = Pg::Math::PGFloat3Normalize(totalMovement);

			// 속도 조절
			Pg::Math::PGFLOAT3 movement = movementDirection * moveSpeed * _pgTime->GetDeltaTime();

			// 위치 업데이트
			_object->_transform._position.x += movement.x;
			_object->_transform._position.y += movement.y;
			_object->_transform._position.z += movement.z;

			_isMoving_Animation = true;
			_isMoving = true;

			if (!_isWalkAudioPlaying) 
			{
				_playerHandler->_walkAudio->Play();
				_isWalkAudioPlaying = true;
			}
		}
		else 
		{
			_isMoving = false;
			if (_isWalkAudioPlaying) 
			{
				_playerHandler->_walkAudio->Stop();
				_isWalkAudioPlaying = false;
			}
		}

		if (_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveFront) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveBack) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveLeft) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveRight)) 
		{
			_isJustSetRestraint = true;
			_isMoving = false;
			_playerHandler->_selfCol->FreezeAxisX(true);
			_playerHandler->_selfCol->FreezeAxisY(true);
			_playerHandler->_selfCol->FreezeAxisZ(true);
		}

		//PhysX 업데이트를 1차례 거친 후, 다시 리셋.
		if (_isJustSetRestraint)
		{
			//다시 호출 안되게.
			_isJustSetRestraint = false;

			_playerHandler->_selfCol->FreezeAxisX(true);
			_playerHandler->_selfCol->FreezeAxisZ(true);
		}
	}

	void PlayerMovementSector::UpdateJump()
	{
		if (_isAbleToJump && _pgInput->GetKeyDown(Pg::API::Input::eKeyCode::Space) && (!_isJumping))
		{
			_isJumping = true;
			_isMoving = true;
			_isJumping_Animation = true;
			_recordedTimeSinceJump = 0.f;

			_playerHandler->_selfCol->AddForce(Pg::Math::PGFLOAT3::GlobalUp() * jumpPower, Pg::Data::ForceMode::eIMPULSE);

			//사운드 재생.
			_playerHandler->_jumpAudio->Play();
		}

		if (_isJumping)
		{
			//약간의 간격이 있어야 자신을 인식하지 않을 것이기에.
			const float tSmallOffset = 0.01f; //추가적으로 Y Position Offset. 
			const float tJumpCheckSmallDist = 0.1f; //밑으로 쏘는 정도
			//const float tJumpCheckSmallDist = 10.f; //밑으로 쏘는 정도
			const float tMinimalTimeBeforeRaycastCheck = 0.4f;

			//지난 시간 DeltaTime으로 점검.
			_recordedTimeSinceJump += _pgTime->GetDeltaTime();
			//PG_ERROR("RECORDING");
			if (_recordedTimeSinceJump > tMinimalTimeBeforeRaycastCheck)
			{
				//밑으로 쏜다.
				Pg::Math::PGFLOAT3 tShouldShootDir = -Pg::Math::PGFLOAT3::GlobalUp();
				Pg::Math::PGFLOAT3 tShouldShootPosition = 
				{
					_object->_transform._position.x,
					_object->_transform._position.y - _halfColliderHeight - tSmallOffset,
					_object->_transform._position.z 
				};

				//Pg::Math::PGFLOAT3 tShouldShootPosition = {
				//	_object->_transform._position.x,
				//	_object->_transform._position.y,
				//	_object->_transform._position.z };

				//로직과 상관없는 거
				Pg::Math::PGFLOAT3 outHitPoint;
				Pg::Data::Collider* tOtherCollider = _pgRayCast->MakeRay(tShouldShootPosition,
					tShouldShootDir, tJumpCheckSmallDist, outHitPoint, nullptr, false); // Trigger 이제 감지하지 마라!

				//매우 짦은 거리로 쏴야 한다. 닿으면 다시 점프를 재충전할 것이니.
				if ((tOtherCollider != nullptr) && (tOtherCollider->GetLayer() != Data::Enums::LAYER_PLAYER))
				{
					//이제 Collider의 레이어를 여기서 다시 Sort해야 할 것이나,
					//일단은 그 과정은 나중에!
					ResetJumpEnabling();
				}

				if (_recordedTimeSinceJump > MINIMAL_JUMP_ERROR_CHECK_TIME)
				{
					//이제는 밑에서 위로 쏘는 것으로 문제를 고칠 것이다. N초 이상 지나면, 위를 향해 쏘는 것으로
					Pg::Math::PGFLOAT3 tBackupCheckPos = 
						_object->_transform._position - Pg::Math::PGFLOAT3(0.1f, MINIMAL_JUMP_ERROR_CHECK_OFFSET, 0.f);
					Pg::Math::PGFLOAT3 tOutPoint;
					Pg::Data::Collider* tExcepCol = _pgRayCast->MakeRay(tBackupCheckPos,
						Pg::Math::PGFLOAT3::GlobalUp(), MINIMAL_JUMP_ERROR_CHECK_DISTANCE, tOutPoint, nullptr, false); // Trigger 이제 감지하지 마라!
						
					if ((tExcepCol != nullptr) && tExcepCol->GetLayer() != Data::Enums::eLayerMask::LAYER_PLAYER)
					{
 						ResetJumpEnabling();
					}
				}
			}
		}
	}

	void PlayerMovementSector::UpdateFacingDirection(float yLevelPlane)
	{
		///마우스의 움직임으로 판별.
		_targetPos = _mainCam->ScreenPointToWorldPlanePoint({ _pgInput->GetMouseX(), _pgInput->GetMouseY() },
			Pg::Math::PGFLOAT3::GlobalUp(), yLevelPlane);
		//Ex.
		//0,1,0 노말에 비해 -1만큼 Distance가 있어야 y=1에 위치할 수 있다.

		//뺄 때 y축 차이를 없애기 위해서.
		_targetPos.y = _object->_transform._position.y;
		Pg::Math::PGFLOAT3 tLookPos = _object->_transform._position - _targetPos;
		_targetRotation = PGLookRotation(tLookPos, Pg::Math::PGFLOAT3::GlobalUp());

		//업데이트할 값 정하고 Update 루프에서 처리하도록.
		_rotBeginRatio = 1.f;
		//유니티와의 차이.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
		_object->_transform._rotation = currentTargetRotation;
	}

	void PlayerMovementSector::StrafeAvoidLogic()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::CtrlL) && (!_isStrafeAvoiding) && (_playerHandler->GetIsStaminaReadyToUse()))
		{
			_isStrafeAvoiding = true;
			_playerHandler->_selfCol->SetActive(false);
			_playerHandler->staminaPoint--;
			_playerHandler->_meshRenderer->SetAnimation("PA_00004.pganim", false);
			_playerHandler->_avoidAudio->Play();

			//ForwardVector의 Back 방향으로 이동해야 한다.
			const float tAvoidDist = 7.0f; //실제로 이동한 거리.
			const float tAvoidBasedTotalTime = 1.0f; //Tween 시간 비율로 Cut 전에, 전체 시간.
			const float tCutShortRatio = 0.5f; //언제 빨리 끝낼지, 0-1.

			Pg::Math::PGFLOAT3 tActualForward = Pg::Math::PGReflectVectorAgainstAxis(_object->_transform.GetForward(), Pg::Math::PGFLOAT3::GlobalForward());
			Pg::Math::PGFLOAT3 tTargetPos = _object->_transform._position - (-tActualForward * tAvoidDist);

			Pg::Util::Tween* tTween = _pgTween->CreateTween();
			tTween->GetData(&(_object->_transform._position)).DoMove(tTargetPos, tAvoidBasedTotalTime).
				SetEase(Pg::Util::Enums::eEasingMode::OUTEXPO).KillEarly(tCutShortRatio).OnComplete(
					[this]()
					{
						_isStrafeAvoiding = false;
						_playerHandler->_selfCol->SetActive(true);
					});
		}
	}

	void PlayerMovementSector::PlayAdequateAnimation()
	{
		//우선, 디폴트로 출력되는 것은 Idle Animation. 

		//곧 추가되어야 하는 것 : Combat System 들어오면 IsDead까지.
		//이 부분에 대량으로 Bool 값 받자.

		//Idle 초기 상태 세팅.
		std::string tToPlayAnimationName = "PA_00001.pganim";
		bool isLooping = true;

		if (_isDead_Animation)
		{
			//사망 애니메이션.
			tToPlayAnimationName = "PA_00014.pganim";
			isLooping = false;
		}
		else if (_isJumping_Animation)
		{
			//점프 애니메이션.
			tToPlayAnimationName = "PA_00003.pganim";
			isLooping = false;
		}
		else if (_isMoving_Animation)
		{
			//걷기 애니메이션
			tToPlayAnimationName = "PA_00002.pganim";
			isLooping = true;
		}

		//만약에 전 스트링과 같지 않을 시에.
		if (_previousAnimationInput.compare(tToPlayAnimationName) != 0)
		{
			_playerHandler->_meshRenderer->SetAnimation(tToPlayAnimationName, isLooping);
		}

		//애니메이션 인풋 스트링 기록.
		_previousAnimationInput = tToPlayAnimationName;
	}

	bool PlayerMovementSector::GetIsMoving()
	{
		return _isMoving;
	}

	void PlayerMovementSector::OnStrafeAvoidComplete()
	{

	}

	void PlayerMovementSector::FixedUpdate()
	{

	}

	void PlayerMovementSector::HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2)
	{

	}

	void PlayerMovementSector::ResetAll()
	{

	}

	void PlayerMovementSector::SetIsAbleToJump(bool val)
	{
		this->_isAbleToJump = val;
	}

	void PlayerMovementSector::ResetJumpEnabling()
	{
		_isJumping_Animation = false;
		_isJumping = false;
		_isMoving = false;
		//_selfCol->SetLinearDamping(_originalLinearDampingValue);
	}

}