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
		// 카메라의 이동과 회전
		Pg::Math::PGFLOAT3 _position;
		Pg::Math::PGQuaternion _rotation;

		// 카메라 정보
		float _nearZ;
		float _farZ;
		float aspect;
		float _fovY;
		Pg::Math::PGFLOAT4X4 _viewMatrix;
		Pg::Math::PGFLOAT4X4 _projMatrix;
	};
}
