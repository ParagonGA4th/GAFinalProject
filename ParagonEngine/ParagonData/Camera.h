#pragma once
#include "Component.h"
#include "CameraData.h"
#include "../ParagonMath/PgMath.h"

#include <memory>

/// <summary>
/// 변지상의 Camera 클래스.
/// 그래픽스 엔진에게 CameraData를 반환해주어야 한다.
/// 2023.09.19
/// </summary>

namespace Pg::Data
{
	using namespace Pg::Math;

	class GameObject;
	class Camera : public Component
	{
	public:
		Camera(Pg::Data::GameObject* obj);
		virtual ~Camera() = default;

		//실제로 쓰이는 Camera들의 정보를 Scene의 정보와 일치시키기 위해서.
		virtual void Update() override;
		virtual void OnDeserialize(SerializeVector& sv) override;
		virtual void OnSerialize(SerializeVector& sv) override;

		//float GetNearZ() const;
		//float GetFarZ() const;
		//float GetFovY() const;

		//void SetNearZ(float nearZ);
		//void SetFarZ(float farZ);
		//void SetFovY(float fovY);

		PGFLOAT4X4 GetViewMatrix() const;
		Pg::Data::CameraData* GetCameraData();

	private:
		void UpdateViewMatrix();


	public:
		BEGIN_VISITABLES(Pg::Data::Camera);
		VISITABLE(float, _nearZ);
		VISITABLE(float, _farZ);
		VISITABLE(float, _fovY);
		END_VISITABLES;

	private:
		std::unique_ptr<Pg::Data::CameraData> _cameraData;

	private:
		//float _nearZ = 0.0001f;
		////float _farZ = PG_PI / static_cast<float>(4); // 가장 현실적인 시야각도를 베이스로 둔다
		//float _farZ = 1000.0f;

		//float _fovY = PG_PI * 0.4f;
		Pg::Math::PGFLOAT4X4 _viewMatrix;

	};

}