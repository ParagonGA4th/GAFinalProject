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
		//Input ��ü �޾ƿ���.
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
		//�����ؾ�
		//_playerHandler->_selfCol->SetUseGravity(false);
	}

	void PlayerMovementSector::Awake()
	{
		// Height�� �޾Ƽ�, �ݰ��� �������� Intersection ����� �غ� �Ϸ�.
		_halfColliderHeight = _playerHandler->_selfCol->GetHeight() / 2.0f;
		_isJumping = false;
		//Camera�� Awake���� MainCamera�� ���� ���̴�. �̰� ���� �� ����.
	}

	void PlayerMovementSector::Start()
	{
		//�ٸ� ��ũ��Ʈ�� Awake���� ���Ӱ� �ΰ��� ����ī�޶� �����ؾ� �Ѵ�.
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
			UpdateFacingDirection(_currentPlaneY); //Plane Y-Level �Է��ؾ�.
			StrafeAvoidLogic();
		}
	
		if (_playerHandler->healthPoint < std::numeric_limits<float>::epsilon())
		{
			_isDead_Animation = true;
		}

		//Player Position ���.
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

		//������ ������� ��
		Pg::Math::PGFLOAT3 outHitPoint;
		float tFloat = 2.0f;
		Pg::Math::PGFLOAT3 tD3DOrigin = { tBasePosition.x + tShouldShootDir.x * tFloat,
						tBasePosition.y + tShouldShootDir.y * tFloat,
						tBasePosition.z + tShouldShootDir.z * tFloat };

		//D3DOrigin.y �� �ø���.
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

		//Camera -> GameObject�� �ٶ󺸴� ������ Forward���� �Ѵ�!
		//�����ǰ� �ִ� ��Ȳ�� �ƴ϶�, Target Pos�� �������� ��������.
		_relativeForward = this->_object->_transform._position - _camBehavior->GetTargetCamPosition();

		//Y Vector ĵ�� + ����ȭ.
		_relativeForward.y = 0.0f;
		_relativeForward = Pg::Math::PGFloat3Normalize(_relativeForward);

		//Y���� �׻� Global Y-Up�� ����Ű�� ���� �״�, Cross�ϸ� Left Vector.
		_relativeLeft = Pg::Math::PGFloat3Cross(_relativeForward, Pg::Math::PGFLOAT3::GlobalUp());

		//����� �ٸ���. ������ �ٲ������.
		_augmentedRelativeForward = { _relativeForward.x * tMoveSpeed * dt, _relativeForward.y * tMoveSpeed * dt, _relativeForward.z * tMoveSpeed * dt };
		_augmentedRelativeLeft = { _relativeLeft.x * tMoveSpeed * dt, _relativeLeft.y * tMoveSpeed * dt, _relativeLeft.z * tMoveSpeed * dt };

		//Face Direction�� �ʿ��ϴ�. ���� �߿� �ִ� ��ġ!
		//����
		//_currentPlaneY = this->_object->_transform._position.y - _halfColliderHeight;
		
		//Position Offset �ݿ� ������.
		_currentPlaneY = this->_object->_transform._position.y - _halfColliderHeight - _playerHandler->_selfCol->GetPositionOffset().y;
		
		//������ ���� Ŭ�� ����Ʈ ���� �߻�, �ش� ������ �ذ��غ���, _currentPlaneY�� Offset�ϴ� ��������!
		{
			//�� �� ����ȭ�Ǿ� ������ -1 ~ 1 ��ȯ�� ��.
			//���� ���밪���� 0-1�� �Ǿ���.
			float tLookVectorSimilarity = fabs(PGFloat3Dot(_relativeForward, Pg::Math::PGFLOAT3::GlobalUp()));

			//������ ���� ������ �޶���������...
			DirectX::XMVECTOR tXMForward = PG2XM_FLOAT3_VECTOR(_relativeForward);
			DirectX::XMVECTOR tForwardQuat = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(tXMForward));
			DirectX::XMVECTOR tCamLookQuat = DirectX::XMQuaternionNormalize(PG2XM_QUATERNION_VECTOR(_camBehavior->_object->_transform._rotation));
			float tQuatDot = fabs(DirectX::XMVectorGetX(DirectX::XMQuaternionDot(tForwardQuat, tCamLookQuat)));

			//+�ؾ�.
			_currentPlaneY = _currentPlaneY - tLookVectorSimilarity;
		}
	}

	void PlayerMovementSector::UpdateWASD()
	{
		//�ϴ� ������ �� �����δٰ� �����ϰ�, ������ ���� Animation ����.
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
			// ����ȭ
			Pg::Math::PGFLOAT3 movementDirection = Pg::Math::PGFloat3Normalize(totalMovement);

			// �ӵ� ����
			Pg::Math::PGFLOAT3 movement = movementDirection * moveSpeed * _pgTime->GetDeltaTime();

			// ��ġ ������Ʈ
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

		//PhysX ������Ʈ�� 1���� ��ģ ��, �ٽ� ����.
		if (_isJustSetRestraint)
		{
			//�ٽ� ȣ�� �ȵǰ�.
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

			//���� ���.
			_playerHandler->_jumpAudio->Play();
		}

		if (_isJumping)
		{
			//�ణ�� ������ �־�� �ڽ��� �ν����� ���� ���̱⿡.
			const float tSmallOffset = 0.01f; //�߰������� Y Position Offset. 
			const float tJumpCheckSmallDist = 0.1f; //������ ��� ����
			//const float tJumpCheckSmallDist = 10.f; //������ ��� ����
			const float tMinimalTimeBeforeRaycastCheck = 0.4f;

			//���� �ð� DeltaTime���� ����.
			_recordedTimeSinceJump += _pgTime->GetDeltaTime();
			//PG_ERROR("RECORDING");
			if (_recordedTimeSinceJump > tMinimalTimeBeforeRaycastCheck)
			{
				//������ ���.
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

				//������ ������� ��
				Pg::Math::PGFLOAT3 outHitPoint;
				Pg::Data::Collider* tOtherCollider = _pgRayCast->MakeRay(tShouldShootPosition,
					tShouldShootDir, tJumpCheckSmallDist, outHitPoint, nullptr, false); // Trigger ���� �������� ����!

				//�ſ� �F�� �Ÿ��� ���� �Ѵ�. ������ �ٽ� ������ �������� ���̴�.
				if ((tOtherCollider != nullptr) && (tOtherCollider->GetLayer() != Data::Enums::LAYER_PLAYER))
				{
					//���� Collider�� ���̾ ���⼭ �ٽ� Sort�ؾ� �� ���̳�,
					//�ϴ��� �� ������ ���߿�!
					ResetJumpEnabling();
				}

				if (_recordedTimeSinceJump > MINIMAL_JUMP_ERROR_CHECK_TIME)
				{
					//������ �ؿ��� ���� ��� ������ ������ ��ĥ ���̴�. N�� �̻� ������, ���� ���� ��� ������
					Pg::Math::PGFLOAT3 tBackupCheckPos = 
						_object->_transform._position - Pg::Math::PGFLOAT3(0.1f, MINIMAL_JUMP_ERROR_CHECK_OFFSET, 0.f);
					Pg::Math::PGFLOAT3 tOutPoint;
					Pg::Data::Collider* tExcepCol = _pgRayCast->MakeRay(tBackupCheckPos,
						Pg::Math::PGFLOAT3::GlobalUp(), MINIMAL_JUMP_ERROR_CHECK_DISTANCE, tOutPoint, nullptr, false); // Trigger ���� �������� ����!
						
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
		///���콺�� ���������� �Ǻ�.
		_targetPos = _mainCam->ScreenPointToWorldPlanePoint({ _pgInput->GetMouseX(), _pgInput->GetMouseY() },
			Pg::Math::PGFLOAT3::GlobalUp(), yLevelPlane);
		//Ex.
		//0,1,0 �븻�� ���� -1��ŭ Distance�� �־�� y=1�� ��ġ�� �� �ִ�.

		//�� �� y�� ���̸� ���ֱ� ���ؼ�.
		_targetPos.y = _object->_transform._position.y;
		Pg::Math::PGFLOAT3 tLookPos = _object->_transform._position - _targetPos;
		_targetRotation = PGLookRotation(tLookPos, Pg::Math::PGFLOAT3::GlobalUp());

		//������Ʈ�� �� ���ϰ� Update �������� ó���ϵ���.
		_rotBeginRatio = 1.f;
		//����Ƽ���� ����.
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

			//ForwardVector�� Back �������� �̵��ؾ� �Ѵ�.
			const float tAvoidDist = 7.0f; //������ �̵��� �Ÿ�.
			const float tAvoidBasedTotalTime = 1.0f; //Tween �ð� ������ Cut ����, ��ü �ð�.
			const float tCutShortRatio = 0.5f; //���� ���� ������, 0-1.

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
		//�켱, ����Ʈ�� ��µǴ� ���� Idle Animation. 

		//�� �߰��Ǿ�� �ϴ� �� : Combat System ������ IsDead����.
		//�� �κп� �뷮���� Bool �� ����.

		//Idle �ʱ� ���� ����.
		std::string tToPlayAnimationName = "PA_00001.pganim";
		bool isLooping = true;

		if (_isDead_Animation)
		{
			//��� �ִϸ��̼�.
			tToPlayAnimationName = "PA_00014.pganim";
			isLooping = false;
		}
		else if (_isJumping_Animation)
		{
			//���� �ִϸ��̼�.
			tToPlayAnimationName = "PA_00003.pganim";
			isLooping = false;
		}
		else if (_isMoving_Animation)
		{
			//�ȱ� �ִϸ��̼�
			tToPlayAnimationName = "PA_00002.pganim";
			isLooping = true;
		}

		//���࿡ �� ��Ʈ���� ���� ���� �ÿ�.
		if (_previousAnimationInput.compare(tToPlayAnimationName) != 0)
		{
			_playerHandler->_meshRenderer->SetAnimation(tToPlayAnimationName, isLooping);
		}

		//�ִϸ��̼� ��ǲ ��Ʈ�� ���.
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