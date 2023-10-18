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
namespace Pg::Core
{
	class GameObject;
}

namespace Pg::Engine
{
	using namespace Pg::Math;

	class Camera : public Pg::Data::Component
	{
	public:
		Camera(Pg::Data::GameObject* obj);
		virtual ~Camera() = default;

		float GetNearZ() const;
		float GetFarZ() const;
		float GetAspect() const;
		float GetFovY() const;

		PGFLOAT4X4 GetViewMatrix() const;
		PGFLOAT4X4 GetProjMatrix() const;

		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);
		void SetAspect(float aspect);
		void SetFovY(float fovY);


	public:
		Pg::Data::CameraData* GetCameraData();

	public:
		std::unique_ptr<Pg::Data::CameraData> _cameraData;

	public:
		// Yaw, pitch, roll (모든 방향으로의 회전이 필요한가?)
		// 

	private:

		float _nearZ;
		float _farZ = PG_PI / 4; // 가장 현실적인 시야각도를 베이스로 둔다
		float _aspect;
		float _fovY;
	};

}