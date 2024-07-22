#pragma once
#include "../ParagonData/GameObject.h"
#include "IObserver.h"
#include "IScriptResettable.h"
#include <limits>

namespace Pg::Data
{
	class Camera;
	class DynamicCollider;
	class BoxCollider;
	class SkinnedMeshRenderer;
	class AudioSource;
}

namespace Pg::API
{
	namespace Input { class PgInput; }
	namespace Time { class PgTime; }
	namespace Raycast { class PgRayCast; }
	namespace Tween { class PgTween; }
	namespace Graphics { class PgGraphics; }
}

namespace Pg::DataScript
{
	class InGameCameraBehavior;
	class PlayerHandler;
}

/// <summary>
/// 이제 하나의 컴포넌트가 아니라, 
/// 하나의 Sector (Player Handler가 자체 관리하게 된다.)
/// </summary>

namespace Pg::DataScript
{
	class PlayerMovementSector : public IObserver, public IScriptResettable //: public ScriptInterface<PlayerMovementSector> //, public IObserver
	{
		friend class PlayerHandler;
	public:
		PlayerMovementSector(PlayerHandler* playerHandler);

		void GrabManagedObjects();

		//얘네들은 Component에 의해 작동되는 것 X, Script 자체에서 수명을 관리한다.
		void BeforePhysicsAwake();
		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
	
		//Animation이 끝났을 때 호출 되는 함수
		void OnAnimationEnd(const std::string& justEndedAnimation);

	public:
		//IObserver : 전체적인 Event 전달을 기준으로 작동할 것.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. 다시 자기 자신을 리셋하는 함수.
		virtual void ResetAll() override;

	public:
		bool GetIsMoving();	//플레이어의 이동여부를 전달하여 공격금지하게 해야함.
		void SetIsAbleToJump(bool val);

		bool GetUseUltimateSkill() { return _useUltimateSkill; }
		void SetUseUltimateSkill(bool val) { _useUltimateSkill = val; }
	
		bool GetUseStrongSkill() { return _useStrongSkill; }
		void SetUseStrongSkill(bool val) { _useStrongSkill = val; }
	private:
		inline static const float MINIMAL_JUMP_ERROR_CHECK_TIME = 1.0f;
		inline static const float MINIMAL_JUMP_ERROR_CHECK_OFFSET = 7.f;
		inline static const float MINIMAL_JUMP_ERROR_CHECK_DISTANCE = 10.f;

		//움직임 관련. 
		float moveSpeed{ 10.0f };
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
		void ResetJumpEnabling();
		//공격하는 모션 등등, 값 관리
		//항상 자신의 바닥부분에서 레이캐스트를 쏴야 한다. (점프를 했으면)]

	private:
		PlayerHandler* _playerHandler;
		Pg::Data::GameObject* _object;
	private:
		Pg::Math::PGFLOAT3 _relativeForward;
		Pg::Math::PGFLOAT3 _relativeLeft;
		Pg::Math::PGFLOAT3 _augmentedRelativeForward; // 이동 정도.
		Pg::Math::PGFLOAT3 _augmentedRelativeLeft; // 이동 정도.

	private:
		InGameCameraBehavior* _camBehavior{ nullptr };
		Pg::Data::Camera* _mainCam{ nullptr };

		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::Raycast::PgRayCast* _pgRayCast{ nullptr };
		Pg::API::Tween::PgTween* _pgTween{ nullptr };
		Pg::API::Graphics::PgGraphics* _pgGraphics{ nullptr };
		Pg::Math::PGFLOAT3 _targetPos;
		Pg::Math::PGQuaternion _targetRotation;

		std::vector<Pg::Data::BoxCollider*> _boxColVec;

	private:
		bool _isJustSetRestraint{ false };
		bool _isJumping = false;
		bool _isMoving = false;
		bool _shouldRotate = false;
		bool _isWalkAudioPlaying = false;
		float _rotBeginRatio = 0.0f;
		float _halfColliderHeight{};
		float _currentPlaneY = 0.f;
		float _recordedTimeSinceJump = 0.f;
		bool _isStrafeAvoiding{ false };
		bool _isAbleToJump{ false };
		bool _useUltimateSkill{ false };
		bool _useStrongSkill{ false };
	private:
		//개별적으로 애니메이션 출력 로직 관리.
		//다 지나갈 시, Idle 출력.
		bool _isMoving_Animation = false;
		bool _isJumping_Animation = false;
		bool _isDead_Animation = false;
		
		std::string _previousAnimationInput;
	};
}



