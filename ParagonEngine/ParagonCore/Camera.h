#pragma once
#include "Component.h"
#include "../ParagonAPI/PgMath.h"

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

namespace Pg::Core
{
	using namespace Pg::Math;
	
	class Camera : public Component
	{
	public:
		Camera(GameObject* obj);
		virtual ~Camera() = default;


		

		CameraData GetCameraData();


		//static Camera* GetMainCamera();

	public:
		CameraData _cameraData;
	};

}

