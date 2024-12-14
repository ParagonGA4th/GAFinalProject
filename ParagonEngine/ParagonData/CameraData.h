#pragma once
#include "../ParagonMath/PgMath.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\Builds\\x64\\Debug\\ParagonMath.lib")
#else
#pragma comment(lib, "..\\Builds\\x64\\Release\\ParagonMath.lib")
#endif // _DEBUG

//반환해줘야 할 데이터
namespace Pg::Data
{
	struct CameraData
	{
	// 엔진에서 채울 카메라 데이터
	public:
		// 카메라의 이동과 회전
		Pg::Math::PGFLOAT3 _position;
		Pg::Math::PGQuaternion _rotation;

		// 카메라 정보
		float _nearZ;
		float _farZ;
		float _fovY;
		Pg::Math::PGFLOAT4X4 _viewMatrix;
		
	// 그래픽스에서 채울 카메라 데이터 
	public: 
		Pg::Math::PGFLOAT4X4 _projMatrix;
		float _aspect;
		float _nearWindowHeight;
		float _farWindowHeight;

	public:
		//플레이어의 위치를 스크립트에서 기록할 수 있게!
		Pg::Math::PGFLOAT3 _playerPosition{ 0.f,0.f,0.f };

		//Pg::Math::PGFLOAT4X4 _mainLightPerspectiveViewProjMatrix;
	};
}
