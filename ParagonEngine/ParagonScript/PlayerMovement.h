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
	class PlayerMovement : public ScriptInterface<PlayerMovement> //, public IObserver
	{
		DEFINE_PARAGON_SCRIPT(PlayerMovement);
	public:
		PlayerMovement(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;
	

		//움직임 관련. 
		float moveSpeed{ 30.0f };
		float rotateMultiplier{ 2.0f };

	private:
		//WASD로 카메라 기준 이동.
		void UpdateWASD();

		//클릭한 곳 기준 바라보기.
		void UpdateFacingDirection(float yLevelPlane);

		//공격하는 모션 등등, 값 관리.


	private:
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

		bool _shouldRotate = false;
		float _rotBeginRatio = 0.0f;

	};
}



