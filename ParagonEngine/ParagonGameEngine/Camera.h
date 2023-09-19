#pragma once
#include "../ParagonCore/Component.h"
#include "../ParagonAPI/PgMath.h"
#include "../ParagonCore/CameraData.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\ParagonAPI.lib")
#endif // _DEBUG

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

	class Camera : public Pg::Core::Component
	{
	public:
		Camera(Pg::Core::GameObject* obj);
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

		Pg::Core::CameraData GetCameraData();

		//static Camera* GetMainCamera();

	public:
		Pg::Core::CameraData _cameraData;

	private:

		float _nearZ;
		float _farZ;
		float _aspect;
		float _fovY;
	};

}