#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Camera;
	class DynamicCollider;
}

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
	class PlayerBehavior : public ScriptInterface<PlayerBehavior>
	{
		DEFINE_PARAGON_SCRIPT(PlayerBehavior);

	public:
		PlayerBehavior(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

		float moveSpeed{ 30.0f };
		float rotateMultiplier{ 2.0f };
		float healthPoint{ 100.0f };

	private:

		//WASD로 카메라 기준 이동.
		void UpdateWASD();
		//클릭한 곳 기준 바라보기.
		void UpdateFacingDirection(float yLevelPlane);



	private:
		Pg::Data::Camera* _mainCam{ nullptr };
		Pg::Data::DynamicCollider* _selfCol{ nullptr };
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



