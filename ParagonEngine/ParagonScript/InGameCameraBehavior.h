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


	private:
		void LerpFollowPlayer();

	private:
		Pg::Data::Transform* _playerTransform{ nullptr };
		Pg::Data::Camera* _selfCamera{ nullptr };
		const Pg::Math::PGFLOAT3 camOffset{ 0, 10, -15 };

	private:
		Pg::Math::PGFLOAT3 _targetCamPosition;
		Pg::API::Time::PgTime* _pgTime{ nullptr };
	};
}


