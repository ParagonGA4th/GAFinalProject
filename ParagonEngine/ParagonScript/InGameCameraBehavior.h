#pragma once
#include "ScriptInterface.h"
#include <visit_struct/visit_struct.hpp>

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

		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		virtual void Awake() override;
		virtual void Start() override;
		virtual void FixedUpdate() override;

		Pg::Math::PGFLOAT3 GetTargetCamPosition(); 


		//스테이지 마다 시점이 다를 경우를 대비해 수정 가능하게 변경
		BEGIN_VISITABLES(Pg::DataScript::InGameCameraBehavior);
		//VISITABLE(float, _speed, 2.0f);
		//VISITABLE(float, _currentRotation, -90.f);
		//VISITABLE(float, _lookDownAngle, 60.f);
		//VISITABLE(Pg::Math::PGFLOAT3, _camOffset, Pg::Math::PGFLOAT3(0, 20, -15));
		VISITABLE(float, _speed);
		VISITABLE(float, _currentRotation);
		VISITABLE(float, _lookDownAngle);
		VISITABLE(Pg::Math::PGFLOAT3, _camOffset);
		END_VISITABLES;

	private:
		void Default_PlayerFollowMode();
		void Boss_RotateAroundMode();

		void UpdateTargetTransforms();
		void LerpFollowPlayer();

	private:
		Pg::Data::Transform* _playerTransform{ nullptr };
		Pg::Data::Camera* _selfCamera{ nullptr };
		//const Pg::Math::PGFLOAT3 camOffset{ -15, 10, -15 };
		


	private:
		Pg::Math::PGFLOAT3 _targetCamPosition;
		Pg::Math::PGQuaternion _targetCamRotation;
		Pg::API::Time::PgTime* _pgTime{ nullptr };
	};
}


