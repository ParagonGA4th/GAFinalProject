#pragma once
#include "ScriptInterface.h"
#include "IObserver.h"

namespace Pg::Data
{
	class Camera;
	class DynamicCollider;
	class SkinnedMeshRenderer;
	class AudioSource;
}

//namespace Pg::DataScript
//{
//	class Camera;
//	class DynamicCollider;
//}

namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Time { class PgTime; }
	namespace Raycast { class PgRayCast; }
	namespace Tween { class PgTween; }
}

namespace Pg::DataScript
{
	class InGameCameraBehavior;
}

namespace Pg::DataScript
{
	class PlayerMovement : public ScriptInterface<PlayerMovement> //, public IObserver
	{
		DEFINE_PARAGON_SCRIPT(PlayerMovement);
	public:
		PlayerMovement(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	

		//움직임 관련. 
		float moveSpeed{ 4.0f };
		float rotateMultiplier{ 2.0f };
		float jumpPower{ 80.0f };

		

	private:
		//In Update Loop
		void ShootRayForward();
		void DetermineDirectionAndValues();
		void UpdateWASD(); //WASD로 카메라 기준 이동.
		void UpdateJump();
		void UpdateFacingDirection(float yLevelPlane); //클릭한 곳 기준 바라보기.
		void StrafeAvoidLogic();
		void PlayAdequateAnimation(); //로직에 따라 올바른 애니메이션을 재생하는 함수.


	private:
		void OnStrafeAvoidComplete();

		//공격하는 모션 등등, 값 관리
		//항상 자신의 바닥부분에서 레이캐스트를 쏴야 한다. (점프를 했으면)
	private:
		Pg::Math::PGFLOAT3 _relativeForward;
		Pg::Math::PGFLOAT3 _relativeLeft;
		Pg::Math::PGFLOAT3 _augmentedRelativeForward; // 이동 정도.
		Pg::Math::PGFLOAT3 _augmentedRelativeLeft; // 이동 정도.

	private:
		InGameCameraBehavior* _camBehavior{ nullptr };
		Pg::Data::Camera* _mainCam{ nullptr };
		Pg::Data::DynamicCollider* _selfCol{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _renderer{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::Raycast::PgRayCast* _pgRayCast{ nullptr };
		Pg::API::Tween::PgTween* _pgTween{ nullptr };
		Pg::Math::PGFLOAT3 _targetPos;
		Pg::Math::PGQuaternion _targetRotation;

	private:
		//플레이어 사운드 관련 변수
		Pg::Data::GameObject* _playerWalkSound;
		Pg::Data::GameObject* _playerJumpSound;

		Pg::Data::AudioSource* _walkAudio;
		Pg::Data::AudioSource* _jumpAudio;

	private:
		bool _isJustSetRestraint{ false };
		bool _isJumping = false;
		bool _shouldRotate = false;
		bool _isWalkAudioPlaying = false;
		float _rotBeginRatio = 0.0f;
		float _halfColliderHeight{};
		float _currentPlaneY = 0.f;
		float _recordedTimeSinceJump = 0.f;
		bool _isStrafeAvoiding{ false };
		bool _isHeadingDownwardsToggle{ false };

	private:
		//개별적으로 애니메이션 출력 로직 관리.
		//다 지나갈 시, Idle 출력.
		bool _isMoving_Animation = false;
		bool _isJumping_Animation = false;
		bool _isAvoiding_Animation = false;
		bool _isDead_Animation = false;
		
		std::string _previousAnimationInput;
	};
}



