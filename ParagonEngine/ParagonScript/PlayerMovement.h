#pragma once
#include "ScriptInterface.h"
#include "IObserver.h"

namespace Pg::Data
{
	class Camera;
	class DynamicCollider;
	class SkinnedMeshRenderer;
}

//namespace Pg::DataScript
//{
//	class Camera;
//	class DynamicCollider;
//}

namespace Pg::API
{
	namespace Input
	{
		class PgInput;
	}

	namespace Time
	{
		class PgTime;
	}

	namespace Raycast
	{
		class PgRayCast;
	}
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
	

		//움직임 관련. 
		float moveSpeed{ 4.0f };
		float rotateMultiplier{ 2.0f };
		float jumpPower{ 45.0f };

	private:
		void ShootRayForward();
		void DetermineDirectionAndValues();
		//WASD로 카메라 기준 이동.
		void UpdateWASD();
		void UpdateJump();
		//클릭한 곳 기준 바라보기.
		void UpdateFacingDirection(float yLevelPlane);

		//공격하는 모션 등등, 값 관리
		//항상 자신의 바닥부분에서 레이캐스트를 쏴야 한다. (점프를 했으면)
	private:
		Pg::Math::PGFLOAT3 _relativeForward;
		Pg::Math::PGFLOAT3 _relativeLeft;


	private:
		InGameCameraBehavior* _camBehavior{ nullptr };
		Pg::Data::Camera* _mainCam{ nullptr };
		Pg::Data::DynamicCollider* _selfCol{ nullptr };
		Pg::Data::SkinnedMeshRenderer* _renderer{ nullptr };
		Pg::API::Input::PgInput* _pgInput{ nullptr };
		Pg::API::Time::PgTime* _pgTime{ nullptr };
		Pg::API::Raycast::PgRayCast* _pgRayCast{ nullptr };
		Pg::Math::PGFLOAT3 _targetPos;
		Pg::Math::PGQuaternion _targetRotation;

	private:
		bool _isJustSetRestraint{ false };
		bool _isJumping = false;
		bool _shouldRotate = false;
		float _rotBeginRatio = 0.0f;
		float _halfColliderHeight{};
		float _currentPlaneY = 0.f;
		float _recordedTimeSinceJump = 0.f;
	};
}



