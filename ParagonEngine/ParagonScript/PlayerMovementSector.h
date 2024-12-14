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
/// ���� �ϳ��� ������Ʈ�� �ƴ϶�, 
/// �ϳ��� Sector (Player Handler�� ��ü �����ϰ� �ȴ�.)
/// </summary>

namespace Pg::DataScript
{
	class PlayerMovementSector : public IObserver, public IScriptResettable //: public ScriptInterface<PlayerMovementSector> //, public IObserver
	{
		friend class PlayerHandler;
	public:
		PlayerMovementSector(PlayerHandler* playerHandler);

		void GrabManagedObjects();

		//��׵��� Component�� ���� �۵��Ǵ� �� X, Script ��ü���� ������ �����Ѵ�.
		void BeforePhysicsAwake();
		void Awake();
		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
	
		//Animation�� ������ �� ȣ�� �Ǵ� �Լ�
		void OnAnimationEnd(const std::string& justEndedAnimation);

	public:
		//IObserver : ��ü���� Event ������ �������� �۵��� ��.
		virtual void HandleEvents(const IEvent& e, UsedVariant usedVar1, UsedVariant usedVar2) override;

		//IScriptResettable. �ٽ� �ڱ� �ڽ��� �����ϴ� �Լ�.
		virtual void ResetAll() override;

	public:
		bool GetIsMoving();	//�÷��̾��� �̵����θ� �����Ͽ� ���ݱ����ϰ� �ؾ���.
		void SetIsAbleToJump(bool val);

		bool GetUseUltimateSkill() { return _useUltimateSkill; }
		void SetUseUltimateSkill(bool val) { _useUltimateSkill = val; }
	
		bool GetUseStrongSkill() { return _useStrongSkill; }
		void SetUseStrongSkill(bool val) { _useStrongSkill = val; }
	private:
		inline static const float MINIMAL_JUMP_ERROR_CHECK_TIME = 1.0f;
		inline static const float MINIMAL_JUMP_ERROR_CHECK_OFFSET = 7.f;
		inline static const float MINIMAL_JUMP_ERROR_CHECK_DISTANCE = 10.f;

		//������ ����. 
		float moveSpeed{ 10.0f };
		float rotateMultiplier{ 2.0f };
		float jumpPower{ 80.0f };

	private:
		//In Update Loop
		void ShootRayForward();
		void DetermineDirectionAndValues();
		void UpdateWASD(); //WASD�� ī�޶� ���� �̵�.
		void UpdateJump();
		void UpdateFacingDirection(float yLevelPlane); //Ŭ���� �� ���� �ٶ󺸱�.
		void StrafeAvoidLogic();
		void PlayAdequateAnimation(); //������ ���� �ùٸ� �ִϸ��̼��� ����ϴ� �Լ�.

	private:
		void OnStrafeAvoidComplete();
		void ResetJumpEnabling();
		//�����ϴ� ��� ���, �� ����
		//�׻� �ڽ��� �ٴںκп��� ����ĳ��Ʈ�� ���� �Ѵ�. (������ ������)]

	private:
		PlayerHandler* _playerHandler;
		Pg::Data::GameObject* _object;
	private:
		Pg::Math::PGFLOAT3 _relativeForward;
		Pg::Math::PGFLOAT3 _relativeLeft;
		Pg::Math::PGFLOAT3 _augmentedRelativeForward; // �̵� ����.
		Pg::Math::PGFLOAT3 _augmentedRelativeLeft; // �̵� ����.

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
		//���������� �ִϸ��̼� ��� ���� ����.
		//�� ������ ��, Idle ���.
		bool _isMoving_Animation = false;
		bool _isJumping_Animation = false;
		bool _isDead_Animation = false;
		
		std::string _previousAnimationInput;
	};
}



