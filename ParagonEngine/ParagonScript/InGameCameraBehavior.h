#pragma once
#include "ScriptInterface.h"
#include <visit_struct/visit_struct.hpp>
#include <functional>

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
		inline static const Pg::Math::PGFLOAT3 CENTER_OF_BOSS_STAGE_CIRCLE = { 0.04f, 2.95f, 0.24f };

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
		void FixCameraMode();

		void UpdateTargetTransforms();
		void LerpFollowPlayer();

	private:
		Pg::Data::Transform* _playerTransform{ nullptr };
		Pg::Data::Camera* _selfCamera{ nullptr };
		std::function<void()> _cameraUpdateMainFunc;

	private:
		Pg::Math::PGFLOAT3 _targetCamPosition;
		Pg::Math::PGQuaternion _targetCamRotation;
		Pg::API::Time::PgTime* _pgTime{ nullptr };

		//카메라 고정 수치
		Pg::Math::PGFLOAT3 _fixedRot = { 0.f,90.f,-40.f };
		Pg::Math::PGFLOAT3 _fixedPos = { 15.f,15.f,0.f };
	};
}


