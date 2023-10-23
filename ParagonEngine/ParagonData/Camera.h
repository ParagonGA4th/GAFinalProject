#pragma once
#include "Component.h"
#include "../ParagonMath/PgMath.h"
#include "CameraData.h"

#include <memory>


/// <summary>
/// 변지상의 Camera 클래스.
/// 그래픽스 엔진에게 CameraData를 반환해주어야 한다.
/// 2023.09.19
/// </summary>
namespace Pg::Data
{
	class GameObject;
}

namespace Pg::Data
{
	using namespace Pg::Math;

	class Camera : public Pg::Data::Component
	{
	public:
		Camera(Pg::Data::GameObject* obj);
		virtual ~Camera() = default;

		//실제로 쓰이는 Camera들의 정보를 Scene의 정보와 일치시키기 위해서.
		virtual void Update() override;

		float GetNearZ() const;
		float GetFarZ() const;
		float GetFovY() const;

		PGFLOAT4X4 GetViewMatrix() const;

		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);
		void SetFovY(float fovY);

	public:
		Pg::Data::CameraData* GetCameraData();

	private:
		std::unique_ptr<Pg::Data::CameraData> _cameraData;

	private:
		float _nearZ = 0.0001f;
		float _farZ = PG_PI / static_cast<float>(4); // 가장 현실적인 시야각도를 베이스로 둔다
		float _fovY = PG_PI * 0.4f;
		Pg::Math::PGFLOAT4X4 _viewMatrix;

		void UpdateViewMatrix();
	};

}