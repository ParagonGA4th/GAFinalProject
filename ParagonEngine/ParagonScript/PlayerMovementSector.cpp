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
#include "../ParagonAPI/PgInput.h"
#include "../ParagonAPI/PgTime.h"
#include "../ParagonAPI/PgRayCast.h"
#include "../ParagonAPI/PgTween.h"

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
	}

	void PlayerMovementSector::GrabManagedObjects()
	{

	}

	void PlayerMovementSector::BeforePhysicsAwake()
	{
		/*for (auto& iter : _object->_transform.GetChildren())
		{
			Pg::Data::BoxCollider* sensorCol = iter->_object->GetComponent<Pg::Data::BoxCollider>();

			if (sensorCol != nullptr)
			{
				_boxColVec.push_back(sensorCol);
				sensorCol->FreezeLinearX(true);
				sensorCol->FreezeLinearX(true);
				sensorCol->FreezeLinearX(true);
				sensorCol->SetUseGravity(false);
			}
		}*/
		//_selfCapCol = _object->GetComponent<Pg::Data::CapsuleCollider>();
		//_selfCapCol->FreezeAxisY(true);
	}

	void PlayerMovementSector::Awake()
	{
		
		
	}

	void PlayerMovementSector::Start()
	{
		//다른 스크립트의 Awake에서 새롭게 인게임 메인카메라를 설정해야 한다.
		_mainCam = _object->GetScene()->GetMainCamera();
		_camBehavior = _mainCam->_object->GetComponent<Pg::DataScript::InGameCameraBehavior>();
		assert(_camBehavior != nullptr);

		_renderer = _object->GetComponent<Pg::Data::SkinnedMeshRenderer>();
		assert(_renderer != nullptr);

		_selfCol = _object->GetComponent<Pg::Data::DynamicCollider>();
		assert(_selfCol != nullptr);
		_selfCol->SetMass(5.f);
		
		// Height을 받아서, 반값을 기준으로 Intersection 계산할 준비 완료.
		_halfColliderHeight = _selfCol->GetHeight() / 2.0f;

		//AudioSource 컴포넌트 들고오기
		_playerWalkSound = _object->GetScene()->FindObjectWithName("PlayerWalkOutSound");
		_walkAudio = _playerWalkSound->GetComponent<Pg::Data::AudioSource>();

		_playerJumpSound = _object->GetScene()->FindObjectWithName("PlayerJumpSound");
		_jumpAudio = _playerJumpSound->GetComponent<Pg::Data::AudioSource>();


		//자신이 속한 Half Collider 높이 만큼 RendererOffset 설정.
		//_renderer->SetRendererOffset({ 0.f, -_halfColliderHeight, 0.f });

		_isJumping = false;
	}

	void PlayerMovementSector::Update()
	{
		ShootRayForward();
		DetermineDirectionAndValues();
		UpdateWASD();
		UpdateJump();
		UpdateFacingDirection(_currentPlaneY); //Plane Y-Level 입력해야.

		StrafeAvoidLogic();
	}

	void PlayerMovementSector::LateUpdate()
	{
		PlayAdequateAnimation();
	}

	void PlayerMovementSector::OnAnimationEnd()
	{
		_isJumping_Animation = false;
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

	void PlayerMovementSector::UpdateWASD()
	{
		//일단 무조건 안 움직인다고 생각하고, 움직일 떄만 Animation 적용.
		_isMoving_Animation = false;

		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveFront))
		{
			//_selfCol->AddForce(relativeForward, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x += _augmentedRelativeForward.x;
			_object->_transform._position.y += _augmentedRelativeForward.y;
			_object->_transform._position.z += _augmentedRelativeForward.z;

			_isMoving_Animation = true;
			_isMoving = true;

			if (!_isWalkAudioPlaying) {
				_walkAudio->Play();
				_isWalkAudioPlaying = true;
			}
			
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveBack))
		{
			//_selfCol->AddForce(-relativeForward, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x -= _augmentedRelativeForward.x;
			_object->_transform._position.y -= _augmentedRelativeForward.y;
			_object->_transform._position.z -= _augmentedRelativeForward.z;

			_isMoving_Animation = true;
			_isMoving = true;

			if (!_isWalkAudioPlaying) {
				_walkAudio->Play();
				_isWalkAudioPlaying = true;
			}
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveLeft))
		{
			//_selfCol->AddForce(relativeLeft, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x += _augmentedRelativeLeft.x;
			_object->_transform._position.y += _augmentedRelativeLeft.y;
			_object->_transform._position.z += _augmentedRelativeLeft.z;

			_isMoving_Animation = true;
			_isMoving = true;

			if (!_isWalkAudioPlaying) {
				_walkAudio->Play();
				_isWalkAudioPlaying = true;
			}
		}
		if (_pgInput->GetKey(Pg::API::Input::eKeyCode::MoveRight))
		{
			//_selfCol->AddForce(-relativeLeft, Pg::Data::ForceMode::eFORCE);
			_object->_transform._position.x -= _augmentedRelativeLeft.x;
			_object->_transform._position.y -= _augmentedRelativeLeft.y;
			_object->_transform._position.z -= _augmentedRelativeLeft.z;

			_isMoving_Animation = true;
			_isMoving = true;

			if (!_isWalkAudioPlaying) {
				_walkAudio->Play();
				_isWalkAudioPlaying = true;
			}
		}

		if (_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveFront) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveBack) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveLeft) ||
			_pgInput->GetKeyUp(Pg::API::Input::eKeyCode::MoveRight))
		{
			//멈췄다가 다시.
			_isJustSetRestraint = true;
			_isMoving = false;
			_selfCol->FreezeAxisX(true);
			_selfCol->FreezeAxisY(true);
			_selfCol->FreezeAxisZ(true);

			if (_isWalkAudioPlaying) {
				_walkAudio->Stop();
				_isWalkAudioPlaying = false;
			}
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

	void PlayerMovementSector::UpdateJump()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::Space) && (!_isJumping))
		{
			_isJumping = true;
			_isMoving = true;
			_isJumping_Animation = true;
			_recordedTimeSinceJump = 0.f;

			//아직 빨리 떨어질 LinearVelocity를 가하지 않았으니.
			_isHeadingDownwardsToggle = false;

			_selfCol->AddForce(Pg::Math::PGFLOAT3::GlobalUp() * jumpPower, Pg::Data::ForceMode::eIMPULSE);

			//사운드 재생.
			_jumpAudio->Play();
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
				if (!_isHeadingDownwardsToggle)
				{
					//한번 아래로 향하면, 계속 Downward Force를 적용할 수 있게 될 것이다.
					Pg::Math::PGFLOAT3 tLinVel = _selfCol->GetVelocity();
					if (tLinVel.y < 0)
					{
						_isHeadingDownwardsToggle = true;
					}
				}
				else
				{
					//만약 내려가는 모션이라면, SetLinearVelocity를 실행.
					//반복적으로 함수 실행의 경우, 중력의 적용을 받지 않는다.
					//Pg::Math::PGFLOAT3 tStrengthedDownForce = { 0.f, -30.f, 0.f };
					//_selfCol->SetVelocity(tStrengthedDownForce);
				}
			
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
					_isJumping_Animation = false;
					_isJumping = false;
					_isMoving = false;
					//_selfCol->SetLinearDamping(_originalLinearDampingValue);
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
		//Pg::Math::PGFLOAT3 lookPos = _targetPos - _object->_transform._position;
		//Pg::Math::PGFLOAT3 lookPos = _object->_transform._position - _targetPos;
		Pg::Math::PGFLOAT3 tLookPos = _object->_transform._position - _targetPos;
		_targetRotation = PGLookRotation(tLookPos, Pg::Math::PGFLOAT3::GlobalUp());

		//업데이트할 값 정하고 Update 루프에서 처리하도록.
		_rotBeginRatio = 1.f;

		//Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
		//유니티와의 차이.
		Pg::Math::PGQuaternion currentTargetRotation = PGQuaternionSlerp(_object->_transform._rotation, _targetRotation, std::clamp<float>(_rotBeginRatio, 0.0f, 1.0f));
		//_selfCol->MoveRotation(currentTargetRotation);
		_object->_transform._rotation = currentTargetRotation;

		//_rotBeginRatio += _pgTime->GetDeltaTime() * rotateMultiplier;
		//if (_rotBeginRatio > 1.0f)
		//{
		//	_rotBeginRatio = 0.0f;
		//	_selfCol->SetAngularVelocity({ 0,0,0 });
		//}
	}

	void PlayerMovementSector::StrafeAvoidLogic()
	{
		if (_pgInput->GetKeyDown(Pg::API::Input::eKeyCode::KeyUp) && (!_isStrafeAvoiding))
		{
			_isStrafeAvoiding = true;
			_renderer->SetAnimation("PA_00004.pganim", false);

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
						_isStrafeAvoiding = false;
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
			_renderer->SetAnimation(tToPlayAnimationName, isLooping);
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
		//이거 원래 있었는데?
		//왜 삭제되었지?
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

	

}