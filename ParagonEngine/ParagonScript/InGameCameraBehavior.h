#pragma once
#include "ScriptInterface.h"

namespace Pg::Data
{
	class Transform;
	class Camera;
}

namespace Pg::API
{
	namespace Time
	{
		class PgTime;
	}
}

namespace Pg::DataScript
{
	class InGameCameraBehavior : public ScriptInterface<InGameCameraBehavior>
	{
		DEFINE_PARAGON_SCRIPT(InGameCameraBehavior);

	public:
		InGameCameraBehavior(Pg::Data::GameObject* obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void FixedUpdate() override;

		float _speed = 2.0f;
		float _currentRotationAmt = 0.f;

		Pg::Math::PGFLOAT3 GetTargetCamPosition(); 

	private:
		void UpdateTargetTransforms();
		void LerpFollowPlayer();

	private:
		Pg::Data::Transform* _playerTransform{ nullptr };
		Pg::Data::Camera* _selfCamera{ nullptr };
		//const Pg::Math::PGFLOAT3 camOffset{ -15, 10, -15 };
		
		//직선 기준, Rotation은 별도가 될 것이다.
		const Pg::Math::PGFLOAT3 camOffset{ 0, 10, -15 };

	private:
		Pg::Math::PGFLOAT3 _targetCamPosition;
		Pg::Math::PGQuaternion _targetCamRotation;
		Pg::API::Time::PgTime* _pgTime{ nullptr };
	};
}


