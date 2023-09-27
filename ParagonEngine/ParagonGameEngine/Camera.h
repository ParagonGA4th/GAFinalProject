#pragma once
#include "../ParagonGameEngine/EngineDLLExporter.h"
#include "../ParagonCore/Component.h"
#include "../ParagonAPI/PgMath.h"
#include "../ParagonCore/CameraData.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Builds\\x64\\Debug\\ParagonAPI.lib")
#else
#pragma comment(lib, "..\\Builds\\x64\\Release\\ParagonAPI.lib")
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
		PARAGON_ENGINE_DLL Camera(Pg::Core::GameObject* obj);
		PARAGON_ENGINE_DLL virtual ~Camera() = default;

		PARAGON_ENGINE_DLL float GetNearZ() const;
		PARAGON_ENGINE_DLL float GetFarZ() const;
		PARAGON_ENGINE_DLL float GetAspect() const;
		PARAGON_ENGINE_DLL float GetFovY() const;
		
		PARAGON_ENGINE_DLL PGFLOAT4X4 GetViewMatrix() const;
		PARAGON_ENGINE_DLL PGFLOAT4X4 GetProjMatrix() const;

		PARAGON_ENGINE_DLL void SetNearZ(float nearZ);
		PARAGON_ENGINE_DLL void SetFarZ(float farZ);
		PARAGON_ENGINE_DLL void SetAspect(float aspect);
		PARAGON_ENGINE_DLL void SetFovY(float fovY);

		PARAGON_ENGINE_DLL Pg::Core::CameraData GetCameraData();

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